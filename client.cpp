#include <socket/socket.h>
using namespace jieeni::socket;

int main()
{
    Socket client;

    client.connect("127.0.0.1",8080);
    string data = "hello world";
    client.send(data.c_str(),data.size());

    char buffer[1024] = {0};
    client.recv(buffer,sizeof(buffer));
    printf("recv: %s\n",buffer);

    client.close();

    return 0;
}