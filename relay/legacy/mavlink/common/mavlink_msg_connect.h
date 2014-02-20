// MESSAGE CONNECT PACKING

#define MAVLINK_MSG_ID_CONNECT 150

typedef struct __mavlink_connect_t
{
 uint8_t seq_number; ///< The sequence number of the message to be acknowledged.
} mavlink_connect_t;

#define MAVLINK_MSG_ID_CONNECT_LEN 1
#define MAVLINK_MSG_ID_150_LEN 1

#define MAVLINK_MSG_ID_CONNECT_CRC 163
#define MAVLINK_MSG_ID_150_CRC 163



#define MAVLINK_MESSAGE_INFO_CONNECT { \
	"CONNECT", \
	1, \
	{  { "seq_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_connect_t, seq_number) }, \
         } \
}


/**
 * @brief Pack a connect message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param seq_number The sequence number of the message to be acknowledged.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_connect_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t seq_number)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_CONNECT_LEN];
	_mav_put_uint8_t(buf, 0, seq_number);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CONNECT_LEN);
#else
	mavlink_connect_t packet;
	packet.seq_number = seq_number;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CONNECT_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_CONNECT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CONNECT_LEN, MAVLINK_MSG_ID_CONNECT_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CONNECT_LEN);
#endif
}

/**
 * @brief Pack a connect message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param seq_number The sequence number of the message to be acknowledged.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_connect_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t seq_number)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_CONNECT_LEN];
	_mav_put_uint8_t(buf, 0, seq_number);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CONNECT_LEN);
#else
	mavlink_connect_t packet;
	packet.seq_number = seq_number;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CONNECT_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_CONNECT;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CONNECT_LEN, MAVLINK_MSG_ID_CONNECT_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CONNECT_LEN);
#endif
}

/**
 * @brief Encode a connect struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param connect C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_connect_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_connect_t* connect)
{
	return mavlink_msg_connect_pack(system_id, component_id, msg, connect->seq_number);
}

/**
 * @brief Encode a connect struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param connect C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_connect_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_connect_t* connect)
{
	return mavlink_msg_connect_pack_chan(system_id, component_id, chan, msg, connect->seq_number);
}

/**
 * @brief Send a connect message
 * @param chan MAVLink channel to send the message
 *
 * @param seq_number The sequence number of the message to be acknowledged.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_connect_send(mavlink_channel_t chan, uint8_t seq_number)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_CONNECT_LEN];
	_mav_put_uint8_t(buf, 0, seq_number);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CONNECT, buf, MAVLINK_MSG_ID_CONNECT_LEN, MAVLINK_MSG_ID_CONNECT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CONNECT, buf, MAVLINK_MSG_ID_CONNECT_LEN);
#endif
#else
	mavlink_connect_t packet;
	packet.seq_number = seq_number;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CONNECT, (const char *)&packet, MAVLINK_MSG_ID_CONNECT_LEN, MAVLINK_MSG_ID_CONNECT_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CONNECT, (const char *)&packet, MAVLINK_MSG_ID_CONNECT_LEN);
#endif
#endif
}

#endif

// MESSAGE CONNECT UNPACKING


/**
 * @brief Get field seq_number from connect message
 *
 * @return The sequence number of the message to be acknowledged.
 */
static inline uint8_t mavlink_msg_connect_get_seq_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Decode a connect message into a struct
 *
 * @param msg The message to decode
 * @param connect C-struct to decode the message contents into
 */
static inline void mavlink_msg_connect_decode(const mavlink_message_t* msg, mavlink_connect_t* connect)
{
#if MAVLINK_NEED_BYTE_SWAP
	connect->seq_number = mavlink_msg_connect_get_seq_number(msg);
#else
	memcpy(connect, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_CONNECT_LEN);
#endif
}
