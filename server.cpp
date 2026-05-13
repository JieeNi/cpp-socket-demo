#include <socket/socket.h>
using namespace jieeni::socket;

int main()
{
    Socket server;
    server.bind(8080);
    server.listen(1024);
    while(true)
    {
        int connfd = server.accept();
        if(connfd < 0)
        {
            return 1;
        }

        Socket client(connfd);

        char buffer[1024] = {0};
        
        size_t len = client.recv(buffer,sizeof(buffer));
        printf("recv: connfd = %d msg = %s\n",connfd,buffer);

        client.send(buffer,len);
    }

    server.close();
    return 0;
}