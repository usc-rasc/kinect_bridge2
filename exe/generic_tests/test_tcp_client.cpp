#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>

#include <iostream>
#include <string>
#include <sstream>

int main()
{
    Poco::Net::SocketAddress socket_address( "localhost", 9284 );
    Poco::Net::StreamSocket socket( socket_address );
    Poco::Net::SocketStream socket_stream( socket );

    std::stringstream response;

    while( socket_stream )
    {
        socket_stream >> response.rdbuf();
    }

    std::cout << response.str() << std::endl;

    return 0;
}
