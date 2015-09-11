#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>

#include <iostream>
#include <chrono>
#include <thread>

int main()
{
    Poco::Net::SocketAddress socket_address( "localhost", 9284 );
    Poco::Net::ServerSocket server_socket( socket_address );

    for (;;)
    {
        std::cout << "waiting for client..." << std::endl;
        Poco::Net::StreamSocket stream_socket = server_socket.acceptConnection();
        std::cout << "got client; sending data" << std::endl;
        Poco::Net::SocketStream stream( stream_socket );

        std::cout << "preparing data..." << std::endl;
        std::this_thread::sleep_for( std::chrono::seconds( 2 ) );
        std::cout << "data ready" << std::endl;
        stream << "hello, world!" << std::endl;
        stream.flush();
        std::cout << "done" << std::endl;
    }

    return 0;
}
