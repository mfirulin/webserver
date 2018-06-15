#include <syslog.h>
#include "server.h"

Server::Server(const in_addr& addr, uint16_t port, const char* dir) :
    local_address(addr), local_port(port), doc_dir(dir)
{
    if((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        throw CreateSocketException("Cannot create socket");
        return;
    }

    memset(&socket_address, 0, sizeof(socket_address));
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = local_port;
    socket_address.sin_addr = local_address;

    if(bind(server_socket, reinterpret_cast<const sockaddr*>(&socket_address), sizeof(socket_address))) {
        throw BindSocketException("Cannot bind socket");
        return;
    }

    if(listen(server_socket, BACKLOG)) {
        throw ListenSocketException("Cannot listen socket");
        return;
    }
}

Server::~Server()
{
    if(server_socket >= 0)
        close(server_socket);
}

void Server::run() const
{
    for(;;) {
        sockaddr_in remote_address;
        socklen_t address_length = sizeof(remote_address);
        int connection;

        if((connection = accept(server_socket, reinterpret_cast<sockaddr*>(&remote_address), &address_length)) > 0) {

        }
        else {
            syslog(LOG_WARNING, "Accept failed for connection %d", connection);
        }
    }
}

void Server::stop() const
{
    if(server_socket >= 0)
        close(server_socket);
}
