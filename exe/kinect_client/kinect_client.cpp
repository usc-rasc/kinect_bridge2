#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>

#include <Poco/Net/SocketAddress.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>

#include <atomics/binary_stream.h>

#include <messages/kinect_messages.h>
#include <messages/binary_codec.h>
#include <messages/message_coder.h>

#include <messages/input_tcp_device.h>

#include <ros/ros.h>

#include <kinect_bridge2/KinectSpeech.h>

class KinectBridge2Client
{
public:
    typedef kinect_bridge2::KinectSpeech _KinectSpeechMsg;
    typedef kinect_bridge2::KinectSpeechPhrase _KinectSpeechPhraseMsg;

    ros::NodeHandle nh_rel_;

    ros::Publisher kinect_speech_pub_;

    InputTCPDevice kinect_bridge_client_;

    MessageCoder<BinaryCodec<> > binary_message_coder_;

    KinectBridge2Client( ros::NodeHandle & nh_rel )
    :
        nh_rel_( nh_rel ),
        kinect_speech_pub_( nh_rel_.advertise<_KinectSpeechMsg>( "speech", 10 ) ),
        kinect_bridge_client_( "localhost", 5903 )
    {
        //
    }

    void spin()
    {
        while( ros::ok() && kinect_bridge_client_.input_socket_.impl()->initialized() )
        {
            while( !kinect_bridge_client_.input_socket_.available() )
            {
                std::cout << "waiting for content..." << std::endl;
                std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
                ros::spinOnce();
            }

            CodedMessage<> binary_coded_message;
            kinect_bridge_client_.pull( binary_coded_message );
            processKinectMessage( binary_coded_message );

            ros::spinOnce();
        }
    }

    void processKinectMessage( CodedMessage<> & coded_message )
    {
        auto & coded_header = coded_message.header_;
        if( coded_header.payload_type_ == "KinectSpeechMessage" )
        {
            auto kinect_speech_message = binary_message_coder_.decode<KinectSpeechMessage>( coded_message );

            auto & header = kinect_speech_message.header_;
            auto & payload = kinect_speech_message.payload_;

            _KinectSpeechMsg ros_kinect_speech_message;

            for( size_t i = 0; i < payload.size(); ++i )
            {
                _KinectSpeechPhraseMsg ros_kinect_speech_phrase_message;
                ros_kinect_speech_phrase_message.tag = payload[i].tag_;
                ros_kinect_speech_phrase_message.confidence = payload[i].confidence_;
                ros_kinect_speech_message.phrases.emplace_back( std::move( ros_kinect_speech_phrase_message ) );
            }

            kinect_speech_pub_.publish( ros_kinect_speech_message );
        }
    }
};

int main( int argc, char ** argv )
{
    ros::init( argc, argv, "kinect_client" );
    ros::NodeHandle nh_rel( "~" );

    KinectBridge2Client kinect_bridge2_client( nh_rel );

    kinect_bridge2_client.spin();

    return 0;
}
