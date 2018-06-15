#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

class CreateSocketException : public exception
{
    string reason;
public:
    CreateSocketException(const char* why) : reason(why) {}

    virtual const char* what() const throw()
    {
        return reason.c_str();
    }
};

class BindSocketException : public exception
{
    string reason;
public:
    BindSocketException(const char* why) : reason(why) {}

    virtual const char* what() const throw()
    {
        return reason.c_str();
    }
};

class ListenSocketException : public exception
{
    string reason;
public:
    ListenSocketException(const char* why) : reason(why) {}

    virtual const char* what() const throw()
    {
        return reason.c_str();
    }
};

class Server
{
public:
    Server(const in_addr& addr, uint16_t port, const char* dir);
    ~Server();
    void run() const;
    void stop() const;

private:
    static const int BACKLOG = 32;
    const in_addr local_address;
    const uint16_t local_port;
    const char* doc_dir;
    sockaddr_in socket_address;
    int server_socket;
};

#endif // SERVER_H
