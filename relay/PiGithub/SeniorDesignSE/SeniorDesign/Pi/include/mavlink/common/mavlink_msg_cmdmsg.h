// MESSAGE CMDMSG PACKING

#define MAVLINK_MSG_ID_CMDMSG 152

typedef struct __mavlink_cmdmsg_t
{
 float param1; ///< Param 1.
 float param2; ///< Param 2.
 float param3; ///< Param 3.
 uint8_t seq_number; ///< The sequence number of the message to be acknowledged.
 uint8_t command_id; ///< The command id.
 uint8_t drone_number; ///< The number of the drone that the message that is about to be acknowledged was sent to.
} mavlink_cmdmsg_t;

#define MAVLINK_MSG_ID_CMDMSG_LEN 15
#define MAVLINK_MSG_ID_152_LEN 15

#define MAVLINK_MSG_ID_CMDMSG_CRC 118
#define MAVLINK_MSG_ID_152_CRC 118



#define MAVLINK_MESSAGE_INFO_CMDMSG { \
	"CMDMSG", \
	6, \
	{  { "param1", NULL, MAVLINK_TYPE_FLOAT, 0, 0, offsetof(mavlink_cmdmsg_t, param1) }, \
         { "param2", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_cmdmsg_t, param2) }, \
         { "param3", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_cmdmsg_t, param3) }, \
         { "seq_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 12, offsetof(mavlink_cmdmsg_t, seq_number) }, \
         { "command_id", NULL, MAVLINK_TYPE_UINT8_T, 0, 13, offsetof(mavlink_cmdmsg_t, command_id) }, \
         { "drone_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 14, offsetof(mavlink_cmdmsg_t, drone_number) }, \
         } \
}


/**
 * @brief Pack a cmdmsg message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param command_id The command id.
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param param1 Param 1.
 * @param param2 Param 2.
 * @param param3 Param 3.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cmdmsg_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t seq_number, uint8_t command_id, uint8_t drone_number, float param1, float param2, float param3)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_CMDMSG_LEN];
	_mav_put_float(buf, 0, param1);
	_mav_put_float(buf, 4, param2);
	_mav_put_float(buf, 8, param3);
	_mav_put_uint8_t(buf, 12, seq_number);
	_mav_put_uint8_t(buf, 13, command_id);
	_mav_put_uint8_t(buf, 14, drone_number);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CMDMSG_LEN);
#else
	mavlink_cmdmsg_t packet;
	packet.param1 = param1;
	packet.param2 = param2;
	packet.param3 = param3;
	packet.seq_number = seq_number;
	packet.command_id = command_id;
	packet.drone_number = drone_number;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CMDMSG_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_CMDMSG;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CMDMSG_LEN, MAVLINK_MSG_ID_CMDMSG_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_CMDMSG_LEN);
#endif
}

/**
 * @brief Pack a cmdmsg message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param command_id The command id.
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param param1 Param 1.
 * @param param2 Param 2.
 * @param param3 Param 3.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_cmdmsg_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t seq_number,uint8_t command_id,uint8_t drone_number,float param1,float param2,float param3)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_CMDMSG_LEN];
	_mav_put_float(buf, 0, param1);
	_mav_put_float(buf, 4, param2);
	_mav_put_float(buf, 8, param3);
	_mav_put_uint8_t(buf, 12, seq_number);
	_mav_put_uint8_t(buf, 13, command_id);
	_mav_put_uint8_t(buf, 14, drone_number);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_CMDMSG_LEN);
#else
	mavlink_cmdmsg_t packet;
	packet.param1 = param1;
	packet.param2 = param2;
	packet.param3 = param3;
	packet.seq_number = seq_number;
	packet.command_id = command_id;
	packet.drone_number = drone_number;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_CMDMSG_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_CMDMSG;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CMDMSG_LEN, MAVLINK_MSG_ID_CMDMSG_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_CMDMSG_LEN);
#endif
}

/**
 * @brief Encode a cmdmsg struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param cmdmsg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cmdmsg_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_cmdmsg_t* cmdmsg)
{
	return mavlink_msg_cmdmsg_pack(system_id, component_id, msg, cmdmsg->seq_number, cmdmsg->command_id, cmdmsg->drone_number, cmdmsg->param1, cmdmsg->param2, cmdmsg->param3);
}

/**
 * @brief Encode a cmdmsg struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param cmdmsg C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_cmdmsg_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_cmdmsg_t* cmdmsg)
{
	return mavlink_msg_cmdmsg_pack_chan(system_id, component_id, chan, msg, cmdmsg->seq_number, cmdmsg->command_id, cmdmsg->drone_number, cmdmsg->param1, cmdmsg->param2, cmdmsg->param3);
}

/**
 * @brief Send a cmdmsg message
 * @param chan MAVLink channel to send the message
 *
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param command_id The command id.
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param param1 Param 1.
 * @param param2 Param 2.
 * @param param3 Param 3.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_cmdmsg_send(mavlink_channel_t chan, uint8_t seq_number, uint8_t command_id, uint8_t drone_number, float param1, float param2, float param3)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_CMDMSG_LEN];
	_mav_put_float(buf, 0, param1);
	_mav_put_float(buf, 4, param2);
	_mav_put_float(buf, 8, param3);
	_mav_put_uint8_t(buf, 12, seq_number);
	_mav_put_uint8_t(buf, 13, command_id);
	_mav_put_uint8_t(buf, 14, drone_number);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CMDMSG, buf, MAVLINK_MSG_ID_CMDMSG_LEN, MAVLINK_MSG_ID_CMDMSG_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CMDMSG, buf, MAVLINK_MSG_ID_CMDMSG_LEN);
#endif
#else
	mavlink_cmdmsg_t packet;
	packet.param1 = param1;
	packet.param2 = param2;
	packet.param3 = param3;
	packet.seq_number = seq_number;
	packet.command_id = command_id;
	packet.drone_number = drone_number;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CMDMSG, (const char *)&packet, MAVLINK_MSG_ID_CMDMSG_LEN, MAVLINK_MSG_ID_CMDMSG_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_CMDMSG, (const char *)&packet, MAVLINK_MSG_ID_CMDMSG_LEN);
#endif
#endif
}

#endif

// MESSAGE CMDMSG UNPACKING


/**
 * @brief Get field seq_number from cmdmsg message
 *
 * @return The sequence number of the message to be acknowledged.
 */
static inline uint8_t mavlink_msg_cmdmsg_get_seq_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  12);
}

/**
 * @brief Get field command_id from cmdmsg message
 *
 * @return The command id.
 */
static inline uint8_t mavlink_msg_cmdmsg_get_command_id(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  13);
}

/**
 * @brief Get field drone_number from cmdmsg message
 *
 * @return The number of the drone that the message that is about to be acknowledged was sent to.
 */
static inline uint8_t mavlink_msg_cmdmsg_get_drone_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  14);
}

/**
 * @brief Get field param1 from cmdmsg message
 *
 * @return Param 1.
 */
static inline float mavlink_msg_cmdmsg_get_param1(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  0);
}

/**
 * @brief Get field param2 from cmdmsg message
 *
 * @return Param 2.
 */
static inline float mavlink_msg_cmdmsg_get_param2(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field param3 from cmdmsg message
 *
 * @return Param 3.
 */
static inline float mavlink_msg_cmdmsg_get_param3(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Decode a cmdmsg message into a struct
 *
 * @param msg The message to decode
 * @param cmdmsg C-struct to decode the message contents into
 */
static inline void mavlink_msg_cmdmsg_decode(const mavlink_message_t* msg, mavlink_cmdmsg_t* cmdmsg)
{
#if MAVLINK_NEED_BYTE_SWAP
	cmdmsg->param1 = mavlink_msg_cmdmsg_get_param1(msg);
	cmdmsg->param2 = mavlink_msg_cmdmsg_get_param2(msg);
	cmdmsg->param3 = mavlink_msg_cmdmsg_get_param3(msg);
	cmdmsg->seq_number = mavlink_msg_cmdmsg_get_seq_number(msg);
	cmdmsg->command_id = mavlink_msg_cmdmsg_get_command_id(msg);
	cmdmsg->drone_number = mavlink_msg_cmdmsg_get_drone_number(msg);
#else
	memcpy(cmdmsg, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_CMDMSG_LEN);
#endif
}
