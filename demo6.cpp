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
        cout << "Using:.demo6\n";
        cout << "5005\n";
        cout << "if\n";
        return -1;
    }
    //step 1
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd == -1)
    {
        perror("socet");
        return -1;
    }
    //step 2
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[1]));
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listenfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) == -1)
    {
        perror("bind");
        close(listenfd);
        return -1;
    }
    //step3
    if(listen(listenfd,5) == -1)
    {
        perror("listen");
        close(listenfd);
        return -1;
    }
    //step 4
    int clientfd = accept(listenfd,0,0);
    if(clientfd == -1)
    {
        perror("accept");
        close(listenfd);
        return -1;
    }
    cout << "connected.\n";
    //step5
    char buffer[1024];
    while(true)
    {
        int iret;
        memset(buffer,0,sizeof(buffer));
        if((iret = recv(clientfd,buffer,sizeof(buffer),0)) <= 0)
        {
            cout << "iret=" <<iret <<endl;
            break;
        }
        cout << "received" << buffer << endl;
        strcpy(buffer,"ok");
        if((iret = send(clientfd,buffer,strlen(buffer),0)) <= 0)
        {
            perror("send");
            break;
        }
        cout << "send:" << buffer << endl;
    }
    //step6
    close(listenfd);
    close(clientfd);
}