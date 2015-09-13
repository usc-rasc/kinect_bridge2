#ifndef _MESSAGES_OUTPUTTCPDEVICE_H_
#define _MESSAGES_OUTPUTTCPDEVICE_H_

#include <string>
#include <fstream>

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>

#include <messages/container_messages.h>
#include <messages/binary_message.h>
#include <messages/exceptions.h>

#include <atomics/binary_stream.h>

// analagous to server; will start up at the given address:port and wait for a client it can stream data to

class OutputTCPDeviceMessageHeader : public RecursiveMessageHeader
{
public:
    std::string destination_address_;
    uint16_t destination_port_;

    OutputTCPDeviceMessageHeader( std::string const & destination_address = "localhost", uint16_t destination_port = 0 )
    :
        destination_address_( destination_address ),
        destination_port_( destination_port )
    {
        //
    }

    template<class __Archive>
    void pack( __Archive & archive )
    {
        RecursiveMessageHeader::pack( archive );
        archive << destination_address_;
        archive << destination_port_;
    }

    template<class __Archive>
    void unpack( __Archive & archive )
    {
        RecursiveMessageHeader::unpack( archive );
        archive >> destination_address_;
        archive >> destination_port_;
    }
};

template<class __Payload = BinaryMessage<> >
class OutputTCPDeviceMessage : public SerializableMessageInterface<OutputTCPDeviceMessageHeader, __Payload>
{
public:
    typedef __Payload _Payload;
    typedef SerializableMessageInterface<OutputTCPDeviceMessageHeader, __Payload> _Message;

    template<class... __Args>
    OutputTCPDeviceMessage( __Args&&... args )
    :
        _Message( std::forward<__Args>( args )... )
    {
        //
    }

    std::string const & name() const
    {
        static std::string const name ( "OutputTCPDeviceMessage" );
        return name;
    }

    virtual std::string const & vName() const
    {
        return name();
    }
};

class OutputTCPDevice
{
public:
    typedef Poco::Net::SocketStream _OutputStream;

    Poco::Net::ServerSocket server_socket_;
    Poco::Net::StreamSocket output_socket_;

    OutputTCPDeviceMessageHeader output_state_;

    OutputTCPDevice() = default;

    // if we've been passed at least one argument, open the socket and record the state
    template<class __Head, class... __Args>
    OutputTCPDevice( __Head && head, __Args&&... args )
    :
        server_socket_( Poco::Net::SocketAddress( std::forward<__Head>( head ), std::forward<__Args>( args )... ) )
    {
        std::cout << "server listening on " << server_socket_.address().toString() << std::endl;
        updateOutputState( std::forward<__Head>( head ), std::forward<__Args>( args )... );
    }

    template<class... __Args>
    void openOutput( __Args&&... args )
    {
        // open the socket and update our state
        server_socket_ = Poco::Net::ServerSocket( Poco::Net::SocketAddress( std::forward<__Args>( args )... ) );
        updateOutputState( std::forward<__Args>( args )... );
    }

    template<class... __Args>
    void updateOutputState( __Args&&... args )
    {
        // record our state
        auto args_tuple = std::make_tuple( std::forward<__Args>( args )... );
        output_state_.destination_address_ = std::get<0>( args_tuple );
        output_state_.destination_port_ = std::get<1>( args_tuple );
    }

    void closeOutput()
    {
        // close the socket to the client
        output_socket_.close();
        // close the server socket
        server_socket_.close();
    }

    template<class __Serializable>
    void push( __Serializable & serializable )
    {
        // make sure stream is open
//        if( !server_stream_ )
//        {
//            // if not, check whether the server socket is ready
//            if( server_socket_.impl().initialized() )
//            {
//                // if it is, we'll wait for new clients
//                server_stream_ = server_socket_.acceptConnection();
//            }
//            // if the server isn't ready, then we can't run this operation right now; bail
//            else throw messages::MessageException( "Failed to serialize message; TCPOutputDevice not initialized" );
//        }

        // check whether the output socket is ready
        if( !output_socket_.impl()->initialized() )
        {
            // if it isn't, we'll wait for new clients; otherwise we already have a connection to the client
//            std::cout << "can't push; waiting for client connection" << std::endl;
            Poco::Net::SocketAddress client_address;
            output_socket_ = server_socket_.acceptConnection( client_address );
            std::cout << "accepted connection from client " << client_address.toString() << std::endl;
        }

        _OutputStream output_stream( output_socket_ );
        atomics::BinaryOutputStream binary_stream( output_stream, atomics::BinaryOutputStream::NETWORK_BYTE_ORDER );
        serializable.pack( binary_stream );
        binary_stream.flush();
        output_stream.flush();
//        output_stream.close();
    }

    template<class __Payload>
    void push( OutputTCPDeviceMessage<__Payload> & device_message )
    {
        auto & header = device_message.header_;
        auto & payload = device_message.payload_;

        openOutput( header.destination_address_, header.destination_port_ );

        push( payload );
    }
};

#endif // _MESSAGES_OUTPUTTCPDEVICE_H_
