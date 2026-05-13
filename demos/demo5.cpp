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
    if(argc != 3)
    {
        cout << "Using:./demo5 \n" ;
        return -1;
    }
    //step 1
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd == -1)
    {
        perror("socket");
        return -1;
    }
    //step 2
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    struct hostent*h;
    if((h = gethostbyname(argv[1])) == nullptr)
    {
        cout << "gethostbyname failed.\n" << endl;
        close(sockfd);
        return -1;
    }
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
    if(connect(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        perror("connect");
        close(sockfd);
        return -1;
    }
    //step 3
    char buffer[1024];
    for (int ii = 0;ii < 10;ii++)
    {
        int iret;
        memset(buffer,0,sizeof(buffer));
        sprintf(buffer,"this is the%d , num %03d",ii+1,ii+1);
        if((iret = send(sockfd,buffer,strlen(buffer), 0 )) <= 0)
        {
            perror("send");
            break;
        }
        cout << "fason" << buffer << endl;
        memset(buffer,0,sizeof(buffer));
        if((iret = recv(sockfd,buffer,sizeof(buffer),0)) <= 0)
        {
            cout << "iret=" << iret << endl;
            break;
        }
        cout << "received" << buffer << endl;
        sleep(1);
    }
    //step4
    close(sockfd);
}
