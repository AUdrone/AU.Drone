// MESSAGE ACK PACKING

#define MAVLINK_MSG_ID_ACK 151

typedef struct __mavlink_ack_t
{
 uint8_t drone_number; ///< The number of the drone that the message that is about to be acknowledged was sent to.
 uint8_t seq_number; ///< The sequence number of the message to be acknowledged.
 uint8_t map_number; ///< Map Number.
} mavlink_ack_t;

#define MAVLINK_MSG_ID_ACK_LEN 3
#define MAVLINK_MSG_ID_151_LEN 3

#define MAVLINK_MSG_ID_ACK_CRC 112
#define MAVLINK_MSG_ID_151_CRC 112



#define MAVLINK_MESSAGE_INFO_ACK { \
	"ACK", \
	3, \
	{  { "drone_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 0, offsetof(mavlink_ack_t, drone_number) }, \
         { "seq_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 1, offsetof(mavlink_ack_t, seq_number) }, \
         { "map_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 2, offsetof(mavlink_ack_t, map_number) }, \
         } \
}


/**
 * @brief Pack a ack message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param map_number Map Number.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ack_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t drone_number, uint8_t seq_number, uint8_t map_number)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_ACK_LEN];
	_mav_put_uint8_t(buf, 0, drone_number);
	_mav_put_uint8_t(buf, 1, seq_number);
	_mav_put_uint8_t(buf, 2, map_number);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ACK_LEN);
#else
	mavlink_ack_t packet;
	packet.drone_number = drone_number;
	packet.seq_number = seq_number;
	packet.map_number = map_number;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ACK_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_ACK;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ACK_LEN, MAVLINK_MSG_ID_ACK_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ACK_LEN);
#endif
}

/**
 * @brief Pack a ack message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param map_number Map Number.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_ack_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t drone_number,uint8_t seq_number,uint8_t map_number)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_ACK_LEN];
	_mav_put_uint8_t(buf, 0, drone_number);
	_mav_put_uint8_t(buf, 1, seq_number);
	_mav_put_uint8_t(buf, 2, map_number);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ACK_LEN);
#else
	mavlink_ack_t packet;
	packet.drone_number = drone_number;
	packet.seq_number = seq_number;
	packet.map_number = map_number;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ACK_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_ACK;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ACK_LEN, MAVLINK_MSG_ID_ACK_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ACK_LEN);
#endif
}

/**
 * @brief Encode a ack struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param ack C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ack_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_ack_t* ack)
{
	return mavlink_msg_ack_pack(system_id, component_id, msg, ack->drone_number, ack->seq_number, ack->map_number);
}

/**
 * @brief Encode a ack struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param ack C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_ack_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_ack_t* ack)
{
	return mavlink_msg_ack_pack_chan(system_id, component_id, chan, msg, ack->drone_number, ack->seq_number, ack->map_number);
}

/**
 * @brief Send a ack message
 * @param chan MAVLink channel to send the message
 *
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param map_number Map Number.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_ack_send(mavlink_channel_t chan, uint8_t drone_number, uint8_t seq_number, uint8_t map_number)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_ACK_LEN];
	_mav_put_uint8_t(buf, 0, drone_number);
	_mav_put_uint8_t(buf, 1, seq_number);
	_mav_put_uint8_t(buf, 2, map_number);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ACK, buf, MAVLINK_MSG_ID_ACK_LEN, MAVLINK_MSG_ID_ACK_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ACK, buf, MAVLINK_MSG_ID_ACK_LEN);
#endif
#else
	mavlink_ack_t packet;
	packet.drone_number = drone_number;
	packet.seq_number = seq_number;
	packet.map_number = map_number;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ACK, (const char *)&packet, MAVLINK_MSG_ID_ACK_LEN, MAVLINK_MSG_ID_ACK_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ACK, (const char *)&packet, MAVLINK_MSG_ID_ACK_LEN);
#endif
#endif
}

#endif

// MESSAGE ACK UNPACKING


/**
 * @brief Get field drone_number from ack message
 *
 * @return The number of the drone that the message that is about to be acknowledged was sent to.
 */
static inline uint8_t mavlink_msg_ack_get_drone_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  0);
}

/**
 * @brief Get field seq_number from ack message
 *
 * @return The sequence number of the message to be acknowledged.
 */
static inline uint8_t mavlink_msg_ack_get_seq_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  1);
}

/**
 * @brief Get field map_number from ack message
 *
 * @return Map Number.
 */
static inline uint8_t mavlink_msg_ack_get_map_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  2);
}

/**
 * @brief Decode a ack message into a struct
 *
 * @param msg The message to decode
 * @param ack C-struct to decode the message contents into
 */
static inline void mavlink_msg_ack_decode(const mavlink_message_t* msg, mavlink_ack_t* ack)
{
#if MAVLINK_NEED_BYTE_SWAP
	ack->drone_number = mavlink_msg_ack_get_drone_number(msg);
	ack->seq_number = mavlink_msg_ack_get_seq_number(msg);
	ack->map_number = mavlink_msg_ack_get_map_number(msg);
#else
	memcpy(ack, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_ACK_LEN);
#endif
}
