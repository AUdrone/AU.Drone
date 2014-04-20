// MESSAGE VIDSTREAM PACKING

#define MAVLINK_MSG_ID_VIDSTREAM 154

typedef struct __mavlink_vidstream_t
{
 uint8_t seq_number; ///< The sequence number of the message to be acknowledged.
 uint8_t drone_number; ///< The number of the drone that the message that is about to be acknowledged was sent to.
 uint8_t vidstream[128]; ///< The video stream buffer.
} mavlink_vidstream_t;

#define MAVLINK_MSG_ID_VIDSTREAM_LEN 130
#define MAVLINK_MSG_ID_154_LEN 130

#define MAVLINK_MSG_ID_VIDSTREAM_CRC 194
#define MAVLINK_MSG_ID_154_CRC 194

#define MAVLINK_MSG_VIDSTREAM_FIELD_VIDSTREAM_LEN 128

#define MAVLINK_MESSAGE_INFO_VIDSTREAM { \
	"VIDSTREAM", \
	3, \
	{  { "seq_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_vidstream_t, seq_number) }, \
         { "drone_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_vidstream_t, drone_number) }, \
         { "vidstream", NULL, MAVLINK_TYPE_UINT8_T, 128, 2, offsetof(mavlink_vidstream_t, vidstream) }, \
         } \
}


/**
 * @brief Pack a vidstream message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param vidstream The video stream buffer.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_vidstream_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t seq_number, uint8_t drone_number, const uint8_t *vidstream)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_VIDSTREAM_LEN];
	_mav_put_uint8_t(buf, 0, seq_number);
	_mav_put_uint8_t(buf, 1, drone_number);
	_mav_put_uint8_t_array(buf, 2, vidstream, 128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_VIDSTREAM_LEN);
#else
	mavlink_vidstream_t packet;
	packet.seq_number = seq_number;
	packet.drone_number = drone_number;
	mav_array_memcpy(packet.vidstream, vidstream, sizeof(uint8_t)*128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_VIDSTREAM_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_VIDSTREAM;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_VIDSTREAM_LEN, MAVLINK_MSG_ID_VIDSTREAM_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_VIDSTREAM_LEN);
#endif
}

/**
 * @brief Pack a vidstream message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param vidstream The video stream buffer.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_vidstream_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t seq_number,uint8_t drone_number,const uint8_t *vidstream)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_VIDSTREAM_LEN];
	_mav_put_uint8_t(buf, 0, seq_number);
	_mav_put_uint8_t(buf, 1, drone_number);
	_mav_put_uint8_t_array(buf, 2, vidstream, 128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_VIDSTREAM_LEN);
#else
	mavlink_vidstream_t packet;
	packet.seq_number = seq_number;
	packet.drone_number = drone_number;
	mav_array_memcpy(packet.vidstream, vidstream, sizeof(uint8_t)*128);
        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_VIDSTREAM_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_VIDSTREAM;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_VIDSTREAM_LEN, MAVLINK_MSG_ID_VIDSTREAM_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_VIDSTREAM_LEN);
#endif
}

/**
 * @brief Encode a vidstream struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param vidstream C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_vidstream_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_vidstream_t* vidstream)
{
	return mavlink_msg_vidstream_pack(system_id, component_id, msg, vidstream->seq_number, vidstream->drone_number, vidstream->vidstream);
}

/**
 * @brief Encode a vidstream struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param vidstream C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_vidstream_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_vidstream_t* vidstream)
{
	return mavlink_msg_vidstream_pack_chan(system_id, component_id, chan, msg, vidstream->seq_number, vidstream->drone_number, vidstream->vidstream);
}

/**
 * @brief Send a vidstream message
 * @param chan MAVLink channel to send the message
 *
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param vidstream The video stream buffer.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_vidstream_send(mavlink_channel_t chan, uint8_t seq_number, uint8_t drone_number, const uint8_t *vidstream)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_VIDSTREAM_LEN];
	_mav_put_uint8_t(buf, 0, seq_number);
	_mav_put_uint8_t(buf, 1, drone_number);
	_mav_put_uint8_t_array(buf, 2, vidstream, 128);
#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_VIDSTREAM, buf, MAVLINK_MSG_ID_VIDSTREAM_LEN, MAVLINK_MSG_ID_VIDSTREAM_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_VIDSTREAM, buf, MAVLINK_MSG_ID_VIDSTREAM_LEN);
#endif
#else
	mavlink_vidstream_t packet;
	packet.seq_number = seq_number;
	packet.drone_number = drone_number;
	mav_array_memcpy(packet.vidstream, vidstream, sizeof(uint8_t)*128);
#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_VIDSTREAM, (const char *)&packet, MAVLINK_MSG_ID_VIDSTREAM_LEN, MAVLINK_MSG_ID_VIDSTREAM_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_VIDSTREAM, (const char *)&packet, MAVLINK_MSG_ID_VIDSTREAM_LEN);
#endif
#endif
}

#endif

// MESSAGE VIDSTREAM UNPACKING


/**
 * @brief Get field seq_number from vidstream message
 *
 * @return The sequence number of the message to be acknowledged.
 */
static inline uint8_t mavlink_msg_vidstream_get_seq_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field drone_number from vidstream message
 *
 * @return The number of the drone that the message that is about to be acknowledged was sent to.
 */
static inline uint8_t mavlink_msg_vidstream_get_drone_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field vidstream from vidstream message
 *
 * @return The video stream buffer.
 */
static inline uint16_t mavlink_msg_vidstream_get_vidstream(const mavlink_message_t* msg, uint8_t *vidstream)
{
	return _MAV_RETURN_uint8_t_array(msg, vidstream, 128,  2);
}

/**
 * @brief Decode a vidstream message into a struct
 *
 * @param msg The message to decode
 * @param vidstream C-struct to decode the message contents into
 */
static inline void mavlink_msg_vidstream_decode(const mavlink_message_t* msg, mavlink_vidstream_t* vidstream)
{
#if MAVLINK_NEED_BYTE_SWAP
	vidstream->seq_number = mavlink_msg_vidstream_get_seq_number(msg);
	vidstream->drone_number = mavlink_msg_vidstream_get_drone_number(msg);
	mavlink_msg_vidstream_get_vidstream(msg, vidstream->vidstream);
#else
	memcpy(vidstream, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_VIDSTREAM_LEN);
#endif
}
