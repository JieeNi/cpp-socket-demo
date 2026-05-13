#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <string>
using std::string;

namespace jieeni
{
    namespace socket
    {
        class Socket
        {
            public:
                Socket();
                Socket(int sockfd);
                ~Socket();

                bool bind(int port);
                bool listen(int backlog);
                bool connect(const string &ip,int port);
                int accept();
                int send(const char* buffer,int len);
                int recv(char* buffer,int len);
                void close();
            protected:
                string m_ip;
                int m_port;
                int m_sockfd;
        };
    }
}