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

class ctcpserver
{
public:
    int m_clientfd;
    string m_ip;
    unsigned short m_port;
    int m_listenfd;
    ctcpserver(): m_clientfd(-1){}


    bool initserver(const unsigned short in_port)
    {
    m_port = in_port;
    m_listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(m_listenfd == -1)
    {
        return false;
    }
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(m_port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(m_listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        perror("bind");
        close(m_listenfd);
        return false;
    }
    return true;
    }


    bool listen()
    {
        if(::listen(m_listenfd,5) == -1)
    {
        perror("listen");
        close(m_listenfd);
        return false;
    }
    return true;
    }


    bool accept()
    {
    m_clientfd = ::accept(m_listenfd,0,0);
    if(m_clientfd == -1)
    {
        perror("accept");
        close(m_listenfd);
        return false;
    }
    return true;
    }

    int recv(string &buffer,int maxlen)
    {
        char buf[maxlen + 1];
        memset(buf,0,sizeof(buf));
        int iret = ::recv(m_clientfd,buf,maxlen,0);
        if(iret < 0) buffer = buf;
        return iret;
    }
    int send(const string &buffer)
    {
        return ::send(m_clientfd,buffer.data(),buffer.size(),0);
    }
    ~ctcpserver(){
        if(m_listenfd != -1) close(m_listenfd);
        if(m_clientfd != -1) close(m_clientfd);
    }
};

int main(int argc,char*argv[])
{
    if(argc != 2)
    {
        cout << "Using:.demo8 5005\n";
        return -1;
    }
    ctcpserver server;
    if(server.initserver(stoi(argv[1])) == false)
    {
        return -1;
    }
    if(server.listen() == false)
    {
        return -1;
    }
    if(server.accept() == false)
    {
        return -1;
    }
    cout << "connect successful" << endl;

    string buffer;
    while(true)
    {
        if(server.recv(buffer,1024) <= 0) break;
        cout << "received:" << buffer << endl;
        if(server.send("ok") <= 0) break;
        cout << "huifu: ok" << endl;
    }
    return 0;
}