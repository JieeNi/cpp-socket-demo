#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/fcntl.h>

//初始化服务端的监听端口
int initserver(int port);
int main(int argc,char*argv[])
{
    if(argc != 2){printf("usage:./tcpselect port\n");return -1;}
    int listensock = initserver(atoi(argv[1]));
    printf("listensock = %d\n",listensock);

    if(listensock < 0){printf("initserver()failed.\n");return -1;}

    fd_set readfds;  //需要监视读事件的socket的集合，大小为16字节(1024位)的bitmap
    FD_ZERO(&readfds);
    FD_SET(listensock,&readfds); //把用于监听的socket加入readfds

    int maxfd = listensock;  //readfds中socket的最大值

    while(true)
    {
        //用于表示超时时间的结构体
        struct timeval timeout;
        timeout.tv_sec = 10; //秒
        timeout.tv_usec = 0;  // 微秒

        fd_set tmpfds = readfds; //创建一个和监听读事件相同的结构体来作为临时的结构体传给select()

        //调用select()等待事件发生
        int infds = select(maxfd+1,&tmpfds,NULL,NULL,0);
        //标准模型int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

        //如果infds < 0，表示调用select()失败
        if(infds < 0){
            perror("select()failed");break;
        }

        //如果infds = 0，表示select()超时
        if(infds == 0){
            printf("select()timeout.\n");continue;
        }

        //如果infds > 0,表示有事件发生，infds存放了已发生事件的个数

        for (int eventfd=0;eventfd<=maxfd;eventfd++){
            if(FD_ISSET(eventfd,&tmpfds)==0)continue;//如果eventfd在bitmap中的标志为0,表示他没有事件,continue

            //如果发生事件的是listensock,表示已连接对的队列中有准备好的socket
            if(eventfd==listensock)
            {
                struct sockaddr_in client;
                socklen_t len = sizeof(client);
                int clientsock = accept(listensock,(struct sockaddr*)&client,&len);
                if(clientsock<0){perror("accept()failed");continue;}

                printf("accept client(socket+%d)ok.\n",clientsock);

                FD_SET(clientsock,&readfds); //把bitmap中新连接的客户端标志位置为1

                if(maxfd<clientsock){
                    maxfd=clientsock;
                }else{
                    //如果是客户端已连接的socket有事件，则是接收缓存区可读或者有客户端断开连接
                    char buffer[1024];
                    memset(buffer,0,sizeof(buffer));
                    if(recv(eventfd,buffer,sizeof(buffer),0) <= 0){
                        //如果客户端的连接已断开
                        printf("client(eventfd=%d)diconnected.\n",eventfd);

                        close(eventfd);//关闭客户端的socket

                        FD_CLR(eventfd,&readfds);//把bitmap中已关闭的的客户端的标志位标零

                        if(eventfd == maxfd)  //重新计算maxfd的值
                        {
                            for(int ii=maxfd;ii>0;ii--){
                                if(FD_ISSET(ii,&readfds)){
                                    maxfd = ii;break;
                                }
                            }
                        }else{
                            //如果客户端有报文发过来
                            printf("recv(eventfd=%d):%s\n",eventfd,buffer);

                            //把接收到的报文内容发送回去
                            send(eventfd,buffer,strlen(buffer),0);
                        }
                    }
                }
            }
        }

    }
return 0;
}

int iniserver(int port){
    int sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock<0)
    {
        perror("socket()failed");return-1;
    }

    int opt=1;
    unsigned int len = sizeof(opt);
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,len);

    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    if(bind(sock,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0){
        perror("bind()failed");close(sock);return -1;
    }

    if(listen(sock,5) != 0){
        perror("listen()failed");close(sock);return -1;
    }
    return sock;
}