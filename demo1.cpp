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

int main(int argc,char*argv[])
{
    if (argc != 3)
    {
        cout << "Using:./demo1 fwqd ip fwdddk\nExample:./demo1 192.168.101.139 5005\n\n";
        return -1;
    }
    //step1
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        perror("socket");
        return -1;
    }
    //step2
    struct hostent*h;
    if((h = gethostbyname(argv[1])) == 0)
    {
        cout << "gethostbyname failed.\n" << endl;
        close(sockfd);
        return -1;
    }
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family  = AF_INET;
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
    servaddr.sin_port = htons(atoi(argv[2]));

    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))!=0)
    {
        perror("connect");
        close(sockfd);
        return -1;
    }
    //step3
    char buffer[1024];
    for(int ii=0;ii<3;ii++)
    {
        int iret;
        memset(buffer,0,sizeof(buffer));
        sprintf(buffer,"num %d,idx %03d",ii+1,ii+1);
        if((iret=send(sockfd,buffer,strlen(buffer),0))<=0)
        {
            perror("send");
            break;
        }
        cout << "send:" << buffer << endl;

        memset(buffer,0,sizeof(buffer));
        if((iret=recv(sockfd,buffer,sizeof(buffer),0))<=0)
        {
            cout << "iret=" << iret << endl;
            break;
        }
        cout << "received:" << buffer << endl;
        sleep(1);
    }
    //step4
    close(sockfd);
}