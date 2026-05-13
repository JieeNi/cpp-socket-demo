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
    if(argc != 2)
    {
        cout << "Using:./demo2 txdk\nExample:./demo2 5005\n\n";
        cout << "5005.\n"; 
        cout << "if\n\n";
        return -1;
    }
    //step1
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1)
    {
        perror("socket");
        return -1;
    }
    //step2
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));

    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr))!=0)
    {
        perror("bind");
        close(listenfd);
        return -1;
    }
    //step3
    if(listen(listenfd,5)!=0)
    {
        perror("listen");
        close(listenfd);
        return -1;
    }
    //step4
    int clientfd = accept(listenfd,0,0);
    if(clientfd == -1)
    {
        perror("accept");
        close(listenfd);
        return -1;
    }
    cout << "linked\n";
    //step5
    char buffer[1024];
    while(true)
    {
        int iret;
        memset(buffer,0,sizeof(buffer));
        if((iret=recv(clientfd,buffer,sizeof(buffer),0))<=0)
        {
            cout << "iret=" << iret << endl;
            break;
        }
        cout << "accepted" << buffer << endl;
        strcpy(buffer,"ok");
        if((iret=send(clientfd,buffer,strlen(buffer),0))<=0)
        {
            perror("send");
            break;
        }
        cout << "fason :" << buffer << endl;
        close(listenfd);
        close(clientfd);
    }
}
