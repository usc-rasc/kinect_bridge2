#ifndef _MESSAGES_INPUTTCPDEVICE_H_
#define _MESSAGES_INPUTTCPDEVICE_H_

#include <string>

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>

#include <atomics/binary_stream.h>

#include <messages/container_messages.h>
#include <messages/binary_message.h>
#include <messages/exceptions.h>

class InputTCPDeviceMessageHeader : public RecursiveMessageHeader
{
public:
    std::string source_address_;
    uint16_t source_port_;

    InputTCPDeviceMessageHeader( std::string const & source_address = "localhost", uint16_t source_port = 0 )
    :
        source_address_( source_address ),
        source_port_( source_port )
    {
        //
    }

    template<class __Archive>
    void pack( __Archive & archive )
    {
        RecursiveMessageHeader::pack( archive );
        archive << source_address_;
        archive << source_port_;
    }

    template<class __Archive>
    void unpack( __Archive & archive )
    {
        RecursiveMessageHeader::unpack( archive );
        archive >> source_address_;
        archive >> source_port_;
    }
};

template<class __Payload = BinaryMessage<> >
class InputTCPDeviceMessage : public SerializableMessageInterface<InputTCPDeviceMessageHeader, __Payload>
{
public:
    typedef __Payload _Payload;
    typedef SerializableMessageInterface<InputTCPDeviceMessageHeader, __Payload> _Message;

    template<class... __Args>
    InputTCPDeviceMessage( __Args&&... args )
    :
        _Message( std::forward<__Args>( args )... )
    {
        //
    }

    std::string const & name() const
    {
        static std::string const name( "InputTCPDeviceMessage" );
        return name;
    }

    virtual std::string const & vName() const
    {
        return name();
    }
};

class InputTCPDevice
{
public:
    typedef Poco::Net::SocketStream _InputStream;

    Poco::Net::StreamSocket input_socket_;
    InputTCPDeviceMessageHeader input_state_;

    InputTCPDevice() = default;

    // open the client socket and update our state
    template<class __Head, class... __Args>
    InputTCPDevice( __Head && head, __Args&&... args )
    :
        input_socket_( Poco::Net::SocketAddress( std::forward<__Head>( head ), std::forward<__Args>( args )... ) )
    {
        updateInputState( std::forward<__Head>( head ), std::forward<__Args>( args )... );
    }

    template<class... __Args>
    void updateInputState( __Args&&... args )
    {
        auto args_tuple = std::make_tuple( std::forward<__Args>( args )... );
        input_state_.source_address_ = std::get<0>( args_tuple );
        input_state_.source_port_ = std::get<1>( args_tuple );
    }

    template<class... __Args>
    void openInput( __Args&&... args )
    {
        input_socket_ = Poco::Net::StreamSocket( Poco::Net::SocketAddress( std::forward<__Args>( args )... ) );
        updateInputState( std::forward<__Args>( args )... );
    }

    void closeInput()
    {
        input_socket_.close();
    }

    template<class __Serializable>
    void pull( __Serializable & serializable )
    {
//        // make sure input stream is ready to receive
//        if( !client_stream_ )
//        {
//            // if not, see if the client socket is connected
//            if( input_socket_.impl()->initialized() )
//            {
//                // if the client is connected then we just need to connect the stream
//                client_stream_ = Poco::Net::SocketStream( input_socket_ );
//            }
//            // if the client isn't connected, we can't do anything; bail
//            else throw messages::MessageException( "Failed to deserialize message; TCPInputDevice not initialized" );
//        }

        _InputStream input_stream( input_socket_ );
        atomics::BinaryInputStream binary_stream( input_stream, atomics::BinaryInputStream::NETWORK_BYTE_ORDER );
//        std::cout << "TCPInputDevice unpacking serializable from input stream" << std::endl;
        serializable.unpack( binary_stream );
        input_stream.flush();
//        input_stream.close();
    }

    template<class __Serializable>
    __Serializable pullAs()
    {
        __Serializable serializable;
        pull( serializable );
        return serializable;
    }

    template<class __Payload>
    InputTCPDeviceMessage<__Payload> pullDeviceMessage()
    {
        return InputTCPDeviceMessage<__Payload>( InputTCPDeviceMessageHeader( input_state_ ), pull<__Payload>() );
    }

    template<class __Payload>
    __Payload pullFrom( InputTCPDeviceMessage<__Payload> const & device_message )
    {
        openInput( device_message.header_.source_address_, device_message.header_.source_port_ );
        return pullAs<__Payload>();
    }

    template<class __Payload>
    void pullDeviceMessage( InputTCPDeviceMessage<__Payload> & device_message )
    {
        openInput( device_message.header_.source_address_, device_message.header_.source_port_ );
        device_message.header_ = input_state_;
        pull( device_message.payload_ );
    }
};

#endif // _MESSAGES_INPUTTCPDEVICE_H_
