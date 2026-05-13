#include <socket/socket.h>
using namespace jieeni::socket;

Socket::Socket() : m_ip(""),m_port(0),m_sockfd(0)
{
    m_sockfd = ::socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (m_sockfd < 0)
    {
        perror("socket");
    }
}

Socket::Socket(int sockfd) : m_ip(""),m_port(0),m_sockfd(sockfd)
{

}

Socket::~Socket()
{
    close();
}

bool Socket::bind(int port)
{
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons(port);
    if(::bind(m_sockfd,(struct sockaddr *)&sockaddr,sizeof(sockaddr)) < 0)
    {
        perror("bind");
        return false;
    }
    m_ip = "0.0.0.0";
    m_port = port;
    return true;
}

bool Socket::listen(int backlog)
{
    if(::listen(m_sockfd,backlog) < 0)
    {
        perror("listen");
        return false;
    }
    return true;
}

bool Socket::connect(const string &ip,int port)
{
    struct sockaddr_in sockaddr;
    std::memset(&sockaddr,0,sizeof(sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = inet_addr(ip.c_str());
    sockaddr.sin_port = htons(port);
    if(::connect(m_sockfd,(struct sockaddr*)&sockaddr,sizeof(sockaddr)) < 0)
    {
        perror("connect");
        return false;
    }
    m_ip = ip;
    m_port = port;
    return true;
}

int Socket::accept()
{
    int connfd = ::accept(m_sockfd,nullptr,nullptr);
    if(connfd < 0)
    {
        perror("accept");
        return -1;
    }
    return connfd;
}

int Socket::send(const char* buffer,int len)
{
    return ::send(m_sockfd,buffer,len,0);
}

int Socket::recv(char* buffer,int len)
{
    return ::recv(m_sockfd,buffer,len,0);
}

void Socket::close()
{
    if(m_sockfd > 0)
    {
        ::close(m_sockfd);
        m_sockfd = 0;
    }
}