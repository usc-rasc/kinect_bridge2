#ifndef _MESSAGES_KINECTMESSAGES_H_
#define _MESSAGES_KINECTMESSAGES_H_

#include <messages/container_messages.h>
#include <messages/utility_messages.h>
#include <messages/image_message.h>
#include <messages/audio_message.h>
#include <messages/geometry_messages.h>

// ####################################################################################################
template<class __ImageMessage = ImageMessage<> >
class KinectColorImageMessage : public MultiMessage<__ImageMessage, TimeStampMessage>
{
public:
    typedef MultiMessage<__ImageMessage, TimeStampMessage> _Message;

    // ====================================================================================================
    template<class... __Args>
    KinectColorImageMessage( __Args&&... args )
    :
        _Message( std::forward<__Args>( args )... )
    {
        //
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectColorImageMessage )
};

// ####################################################################################################
class KinectDepthImageInfoMessage : public SerializableInterface
{
public:
    uint16_t min_reliable_distance_;
    uint16_t max_reliable_distance_;

    // ====================================================================================================
    KinectDepthImageInfoMessage()
    :
        min_reliable_distance_( 0 ),
        max_reliable_distance_( 0 )
    {
        //
    }

    // ====================================================================================================
    template<class __Archive>
    void pack( __Archive & archive ) const
    {
        archive << min_reliable_distance_;
        archive << max_reliable_distance_;
    }

    // ====================================================================================================
    template<class __Archive>
    void unpack( __Archive & archive )
    {
        archive >> min_reliable_distance_;
        archive >> max_reliable_distance_;
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectDepthImageInfoMessage )
};

// ####################################################################################################
template<class __ImageMessage = ImageMessage<> >
class KinectDepthImageMessage : public MultiMessage<__ImageMessage, KinectDepthImageInfoMessage, TimeStampMessage>
{
public:
    typedef MultiMessage<__ImageMessage, KinectDepthImageInfoMessage, TimeStampMessage> _Message;

    // ====================================================================================================
    template<class... __Args>
    KinectDepthImageMessage( __Args&&... args )
    :
        _Message( std::forward<__Args>( args )... )
    {
        //
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectDepthImageMessage )
};

// ####################################################################################################
template<class __ImageMessage = ImageMessage<> >
class KinectInfraredImageMessage : public MultiMessage<__ImageMessage, TimeStampMessage>
{
public:
    typedef MultiMessage<__ImageMessage, TimeStampMessage> _Message;

    // ====================================================================================================
    template<class... __Args>
    KinectInfraredImageMessage( __Args&&... args )
    :
        _Message( std::forward<__Args>( args )... )
    {
        //
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectInfraredImageMessage )
};

// ####################################################################################################
class KinectAudioInfoMessage : public SerializableInterface
{
public:
    float beam_angle_;
    float beam_angle_confidence_;

    // ====================================================================================================
    KinectAudioInfoMessage( float beam_angle = 0, float beam_angle_confidence = 0 )
    :
        beam_angle_( beam_angle ),
        beam_angle_confidence_( beam_angle_confidence )
    {
        //
    }

    // ====================================================================================================
    template<class __Archive>
    void pack( __Archive & archive ) const
    {
        archive << beam_angle_;
        archive << beam_angle_confidence_;
    }

    // ====================================================================================================
    template<class __Archive>
    void unpack( __Archive & archive )
    {
        archive >> beam_angle_;
        archive >> beam_angle_confidence_;
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectAudioInfoMessage )
};

// ####################################################################################################
template<class __AudioMessage = AudioMessage<> >
class KinectAudioMessage : public MultiMessage<__AudioMessage, KinectAudioInfoMessage, TimeStampMessage>
{
public:
    typedef MultiMessage<__AudioMessage, KinectAudioInfoMessage, TimeStampMessage> _Message;

    // ====================================================================================================
    template<class... __Args>
    KinectAudioMessage( __Args&&... args )
    :
        _Message( std::forward<__Args>( args )... )
    {
        //
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectAudioMessage )
};

// ####################################################################################################
class KinectJointMessage : public SerializableInterface
{
public:
    enum class JointType
    {
        SPINE_BASE = 0,
        SPINE_MID = 1,
        NECK = 2,
        HEAD = 3,
        SHOULDER_LEFT = 4,
        ELBOW_LEFT = 5,
        WRIST_LEFT = 6,
        HAND_LEFT = 7,
        SHOULDER_RIGHT = 8,
        ELBOW_RIGHT = 9,
        WRIST_RIGHT = 10,
        HAND_RIGHT = 11,
        HIP_LEFT = 12,
        KNEE_LEFT = 13,
        ANKLE_LEFT = 14,
        FOOT_LEFT = 15,
        HIP_RIGHT = 16,
        KNEE_RIGHT = 17,
        ANKLE_RIGHT = 18,
        FOOT_RIGHT = 19,
        SPINE_SHOULDER = 20,
        HANDTIP_LEFT = 21,
        THUMB_LEFT = 22,
        HANDTIP_RIGHT = 23,
        THUMB_RIGHT = 24
    };

    enum class TrackingState
    {
        NOT_TRACKED = 0,
        INFERRED = 1,
        TRACKED = 2
    };

    PointMessage<float, 3> position_;
    PointMessage<float, 4> orientation_;

    JointType joint_type_;
    TrackingState tracking_state_;

    // ====================================================================================================
    KinectJointMessage()
    {
        //
    }

    // ====================================================================================================
    template<class __Archive>
    void pack( __Archive & archive )
    {
        archive << static_cast<uint8_t>( joint_type_ );
        archive << static_cast<uint8_t>( tracking_state_ );
        position_.pack( archive );
        orientation_.pack( archive );
    }

    // ====================================================================================================
    template<class __Archive>
    void unpack( __Archive & archive )
    {
        uint8_t joint_type;
        archive >> joint_type;
        joint_type_ = static_cast<JointType>( joint_type );

        uint8_t tracking_state;
        archive >> tracking_state;
        tracking_state_ = static_cast<TrackingState>( tracking_state );

        position_.unpack( archive );
        orientation_.unpack( archive );
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectJointMessage )
};

// ####################################################################################################
class KinectBodyMessage : public VectorMessage<KinectJointMessage>
{
public:
    typedef VectorMessage<KinectJointMessage> _Message;

    enum class HandState
    {
        UNKNOWN = 0,
        NOT_TRACKED = 1,
        OPEN = 2,
        CLOSED = 3,
        LASSO = 4
    };

    std::vector<KinectJointMessage> & joints_;
    uint8_t is_tracked_;
    HandState hand_state_left_;
    HandState hand_state_right_;

    // ====================================================================================================
    KinectBodyMessage()
    :
        _Message(),
        joints_( this->payload_ ),
        is_tracked_( 0 ),
        hand_state_left_( HandState::UNKNOWN ),
        hand_state_right_( HandState::UNKNOWN )
    {
        //
    }

    // ====================================================================================================
    template<class __Archive>
    void pack( __Archive & archive )
    {
        archive << is_tracked_;
        archive << static_cast<uint8_t>( hand_state_left_ );
        archive << static_cast<uint8_t>( hand_state_right_ );
        _Message::pack( archive );
    }

    // ====================================================================================================
    template<class __Archive>
    void unpack( __Archive & archive )
    {
        archive >> is_tracked_;

        uint8_t hand_state_left;
        archive >> hand_state_left;
        hand_state_left_ = static_cast<HandState>( hand_state_left );

        uint8_t hand_state_right;
        archive >> hand_state_right;
        hand_state_right_ = static_cast<HandState>( hand_state_right );

        _Message::unpack( archive );
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectBodyMessage )
};

// ####################################################################################################
class KinectBodiesMessage : public MultiMessage<VectorMessage<KinectBodyMessage>, TimeStampMessage>
{
public:
    typedef MultiMessage<VectorMessage<KinectBodyMessage>, TimeStampMessage> _Message;

    // ====================================================================================================
    template<class... __Args>
    KinectBodiesMessage( __Args&&... args )
    :
        _Message( std::forward<__Args>( args )... )
    {
        //
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectBodiesMessage )
};

// ####################################################################################################
class KinectSpeechPhraseMessage : public SerializableInterface
{
public:
    std::string tag_;
    float confidence_;

    // ====================================================================================================
    KinectSpeechPhraseMessage()
    {
        //
    }

    // ====================================================================================================
    template<class __Archive>
    void pack( __Archive & archive )
    {
        archive << tag_;
        archive << static_cast<float>( confidence_ );
    }

    // ====================================================================================================
    template<class __Archive>
    void unpack( __Archive & archive )
    {
        archive >> tag_;
        archive >> confidence_;
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectSpeechPhraseMessage )
};

// ####################################################################################################
class KinectSpeechMessage : public MultiMessage<VectorMessage<KinectSpeechPhraseMessage>, TimeStampMessage>
{
public:
    typedef MultiMessage<VectorMessage<KinectSpeechPhraseMessage>, TimeStampMessage> _Message;

    // ====================================================================================================
    template<class... __Args>
    KinectSpeechMessage( __Args&&... args )
    :
        _Message( std::forward<__Args>( args )... )
    {
        //
    }

    // ====================================================================================================
    DECLARE_MESSAGE_INFO( KinectSpeechMessage )
};

#endif // _MESSAGES_KINECTMESSAGES_H_
