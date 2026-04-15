#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

class ctcpclient
{
public:
    int m_clientfd;
    string m_ip;
    unsigned short m_port;

    ctcpclient(): m_clientfd(-1){}


    bool connect(const string &in_ip,const unsigned short in_port)
    {
    if (m_clientfd != -1) return false;
    m_ip = in_ip;
    m_port = in_port;
        //step 1
    if((m_clientfd = socket(AF_INET,SOCK_STREAM,0)) == -1) return false;
    //step 2
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(m_port);

    struct hostent*h;
    if((h = gethostbyname(m_ip.c_str())) == nullptr)
    {
        ::close(m_clientfd);
        m_clientfd = -1;
        return false;
    }
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
    if(::connect(m_clientfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        ::close(m_clientfd);
        m_clientfd = -1;
        return false;
    }
    return true;
    }
    bool send(const string &buffer)
    {   
        if(m_clientfd == -1) return false;

        if((::send(m_clientfd,buffer.data(),buffer.size(), 0 )) <= 0) return false;
        return true;
    }
    bool recv(string &buffer,const size_t maxlen)
    {
        buffer.clear();
        buffer.resize(maxlen);
        int readn = ::recv(m_clientfd,&buffer[0],buffer.size(),0);
        if(readn <= 0) {buffer.clear();return false;}
        buffer.resize(readn);
        return true;
    }
    bool close()
    {
        if (m_clientfd == -1) return false;
        ::close(m_clientfd);
        m_clientfd = -1;
        return true;
    }
    ~ctcpclient(){ close(); }
};

int main(int argc,char*argv[])
{
    if(argc != 3)
    {
        cout << "Using:./demo5 \n" ;
        return -1;
    }
    ctcpclient tcpclient;
    if(tcpclient.connect(argv[1],stoi(argv[2])) == false)
    {
        perror("connect()");
        return -1;
    }
    //step 3
    string buffer;
    for (int ii = 0;ii < 10;ii++)
    {
        buffer = "zhe shi di" + to_string(ii + 1) +"ge,num" + to_string(ii + 1);
        if(tcpclient.send(buffer) == false)
        {
            perror("send");
            break;
        }
        cout << "fason" << buffer << endl;

        if(tcpclient.recv(buffer,1024) == false)
        {
            perror("recv()");
            break;
        }
        cout << "received" << buffer << endl;
        sleep(1);
    }
    //step4
    close(tcpclient.m_clientfd);
}
