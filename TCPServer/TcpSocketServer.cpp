//
// Created by vanya on 21.06.15.
//

#include <netdb.h>
#include <unistd.h>
#include "TcpSocketServer.h"
#include "MyEpoll.h"
#include "debug.h"
#include "Executor.h"
#include "TcpSocketClient.h"

namespace
{
    FileDescriptor create_socket(int port, string ipAddress, function<void(shared_ptr < TcpSocketClient >) > onAccept,
                      function<void(int, u_int32_t )> onReceive, Executor *executor)
    {
        addrinfo hints;
        addrinfo *result;

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
        hints.ai_socktype = SOCK_STREAM; /* Datagram socket */

        assertMy(getaddrinfo(ipAddress.data(), to_string(port).data(), &hints, &result) == 0);

        assertMy(result != NULL);
        FileDescriptor socketDescriptor(socket(result->ai_family, result->ai_socktype, result->ai_protocol));
        db2("socket client created", socketDescriptor.get());
        assertMy(socketDescriptor.get() != -1);

        int one = 1;
        assertMy(setsockopt(socketDescriptor.get(), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int)) == 0);
        assertMy(bind(socketDescriptor.get(), result->ai_addr, result->ai_addrlen) == 0);

        assertMy(listen(socketDescriptor.get(), BACK_LOG) == 0);
        freeaddrinfo(result);           /* No longer needed */
        
        return socketDescriptor;
    }
}

TcpSocketServer::TcpSocketServer(int port, string ipAddress, function<void(shared_ptr < TcpSocketClient >) > onAccept,
                                 function<void(int, u_int32_t )> onReceive, Executor *executor)
        : executor(executor)
        , socketDescriptor(create_socket(port, ipAddress, onAccept, onReceive, executor))
        , smartSocket(socketDescriptor.get(), [=](int fd, u_int32_t) {
            sockaddr_storage sockAddrStorage;
            socklen_t sockLen = sizeof(sockAddrStorage);
            int newSocketDescriptor = accept(socketDescriptor.get(), (sockaddr *) &sockAddrStorage, &sockLen);

            shared_ptr<TcpSocketClient> client(new TcpSocketClient(newSocketDescriptor, executor));

            onAccept(client);
            executor->add(newSocketDescriptor, [=](u_int32_t mask) { onReceive(newSocketDescriptor, mask); }, EPOLLIN);
        }, EPOLLIN | EPOLLOUT, executor)
{}
