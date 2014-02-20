// MESSAGE NAVDATA PACKING

#define MAVLINK_MSG_ID_NAVDATA 153

typedef struct __mavlink_navdata_t
{
 uint32_t drone_state; ///< The state of the drone.
 float vx; ///< X velocity.
 float vy; ///< Y velocity.
 float altitude; ///< Altitude.
 float heading; ///< Heading.
 uint32_t battery; ///< Battery Life.
 uint8_t seq_number; ///< The sequence number of the message to be acknowledged.
 uint8_t drone_number; ///< The number of the drone that the message that is about to be acknowledged was sent to.
} mavlink_navdata_t;

#define MAVLINK_MSG_ID_NAVDATA_LEN 26
#define MAVLINK_MSG_ID_153_LEN 26

#define MAVLINK_MSG_ID_NAVDATA_CRC 146
#define MAVLINK_MSG_ID_153_CRC 146



#define MAVLINK_MESSAGE_INFO_NAVDATA { \
	"NAVDATA", \
	8, \
	{  { "drone_state", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_navdata_t, drone_state) }, \
         { "vx", NULL, MAVLINK_TYPE_FLOAT, 0, 4, offsetof(mavlink_navdata_t, vx) }, \
         { "vy", NULL, MAVLINK_TYPE_FLOAT, 0, 8, offsetof(mavlink_navdata_t, vy) }, \
         { "altitude", NULL, MAVLINK_TYPE_FLOAT, 0, 12, offsetof(mavlink_navdata_t, altitude) }, \
         { "heading", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_navdata_t, heading) }, \
         { "battery", NULL, MAVLINK_TYPE_UINT32_T, 0, 20, offsetof(mavlink_navdata_t, battery) }, \
         { "seq_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_navdata_t, seq_number) }, \
         { "drone_number", NULL, MAVLINK_TYPE_UINT8_T, 0, 25, offsetof(mavlink_navdata_t, drone_number) }, \
         } \
}


/**
 * @brief Pack a navdata message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param drone_state The state of the drone.
 * @param vx X velocity.
 * @param vy Y velocity.
 * @param altitude Altitude.
 * @param heading Heading.
 * @param battery Battery Life.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_navdata_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
						       uint8_t seq_number, uint8_t drone_number, uint32_t drone_state, float vx, float vy, float altitude, float heading, uint32_t battery)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_NAVDATA_LEN];
	_mav_put_uint32_t(buf, 0, drone_state);
	_mav_put_float(buf, 4, vx);
	_mav_put_float(buf, 8, vy);
	_mav_put_float(buf, 12, altitude);
	_mav_put_float(buf, 16, heading);
	_mav_put_uint32_t(buf, 20, battery);
	_mav_put_uint8_t(buf, 24, seq_number);
	_mav_put_uint8_t(buf, 25, drone_number);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_NAVDATA_LEN);
#else
	mavlink_navdata_t packet;
	packet.drone_state = drone_state;
	packet.vx = vx;
	packet.vy = vy;
	packet.altitude = altitude;
	packet.heading = heading;
	packet.battery = battery;
	packet.seq_number = seq_number;
	packet.drone_number = drone_number;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_NAVDATA_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_NAVDATA;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_NAVDATA_LEN, MAVLINK_MSG_ID_NAVDATA_CRC);
#else
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_NAVDATA_LEN);
#endif
}

/**
 * @brief Pack a navdata message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param drone_state The state of the drone.
 * @param vx X velocity.
 * @param vy Y velocity.
 * @param altitude Altitude.
 * @param heading Heading.
 * @param battery Battery Life.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_navdata_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
							   mavlink_message_t* msg,
						           uint8_t seq_number,uint8_t drone_number,uint32_t drone_state,float vx,float vy,float altitude,float heading,uint32_t battery)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_NAVDATA_LEN];
	_mav_put_uint32_t(buf, 0, drone_state);
	_mav_put_float(buf, 4, vx);
	_mav_put_float(buf, 8, vy);
	_mav_put_float(buf, 12, altitude);
	_mav_put_float(buf, 16, heading);
	_mav_put_uint32_t(buf, 20, battery);
	_mav_put_uint8_t(buf, 24, seq_number);
	_mav_put_uint8_t(buf, 25, drone_number);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_NAVDATA_LEN);
#else
	mavlink_navdata_t packet;
	packet.drone_state = drone_state;
	packet.vx = vx;
	packet.vy = vy;
	packet.altitude = altitude;
	packet.heading = heading;
	packet.battery = battery;
	packet.seq_number = seq_number;
	packet.drone_number = drone_number;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_NAVDATA_LEN);
#endif

	msg->msgid = MAVLINK_MSG_ID_NAVDATA;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_NAVDATA_LEN, MAVLINK_MSG_ID_NAVDATA_CRC);
#else
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_NAVDATA_LEN);
#endif
}

/**
 * @brief Encode a navdata struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param navdata C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_navdata_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_navdata_t* navdata)
{
	return mavlink_msg_navdata_pack(system_id, component_id, msg, navdata->seq_number, navdata->drone_number, navdata->drone_state, navdata->vx, navdata->vy, navdata->altitude, navdata->heading, navdata->battery);
}

/**
 * @brief Encode a navdata struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param navdata C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_navdata_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_navdata_t* navdata)
{
	return mavlink_msg_navdata_pack_chan(system_id, component_id, chan, msg, navdata->seq_number, navdata->drone_number, navdata->drone_state, navdata->vx, navdata->vy, navdata->altitude, navdata->heading, navdata->battery);
}

/**
 * @brief Send a navdata message
 * @param chan MAVLink channel to send the message
 *
 * @param seq_number The sequence number of the message to be acknowledged.
 * @param drone_number The number of the drone that the message that is about to be acknowledged was sent to.
 * @param drone_state The state of the drone.
 * @param vx X velocity.
 * @param vy Y velocity.
 * @param altitude Altitude.
 * @param heading Heading.
 * @param battery Battery Life.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_navdata_send(mavlink_channel_t chan, uint8_t seq_number, uint8_t drone_number, uint32_t drone_state, float vx, float vy, float altitude, float heading, uint32_t battery)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
	char buf[MAVLINK_MSG_ID_NAVDATA_LEN];
	_mav_put_uint32_t(buf, 0, drone_state);
	_mav_put_float(buf, 4, vx);
	_mav_put_float(buf, 8, vy);
	_mav_put_float(buf, 12, altitude);
	_mav_put_float(buf, 16, heading);
	_mav_put_uint32_t(buf, 20, battery);
	_mav_put_uint8_t(buf, 24, seq_number);
	_mav_put_uint8_t(buf, 25, drone_number);

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NAVDATA, buf, MAVLINK_MSG_ID_NAVDATA_LEN, MAVLINK_MSG_ID_NAVDATA_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NAVDATA, buf, MAVLINK_MSG_ID_NAVDATA_LEN);
#endif
#else
	mavlink_navdata_t packet;
	packet.drone_state = drone_state;
	packet.vx = vx;
	packet.vy = vy;
	packet.altitude = altitude;
	packet.heading = heading;
	packet.battery = battery;
	packet.seq_number = seq_number;
	packet.drone_number = drone_number;

#if MAVLINK_CRC_EXTRA
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NAVDATA, (const char *)&packet, MAVLINK_MSG_ID_NAVDATA_LEN, MAVLINK_MSG_ID_NAVDATA_CRC);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_NAVDATA, (const char *)&packet, MAVLINK_MSG_ID_NAVDATA_LEN);
#endif
#endif
}

#endif

// MESSAGE NAVDATA UNPACKING


/**
 * @brief Get field seq_number from navdata message
 *
 * @return The sequence number of the message to be acknowledged.
 */
static inline uint8_t mavlink_msg_navdata_get_seq_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  24);
}

/**
 * @brief Get field drone_number from navdata message
 *
 * @return The number of the drone that the message that is about to be acknowledged was sent to.
 */
static inline uint8_t mavlink_msg_navdata_get_drone_number(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint8_t(msg,  25);
}

/**
 * @brief Get field drone_state from navdata message
 *
 * @return The state of the drone.
 */
static inline uint32_t mavlink_msg_navdata_get_drone_state(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field vx from navdata message
 *
 * @return X velocity.
 */
static inline float mavlink_msg_navdata_get_vx(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  4);
}

/**
 * @brief Get field vy from navdata message
 *
 * @return Y velocity.
 */
static inline float mavlink_msg_navdata_get_vy(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  8);
}

/**
 * @brief Get field altitude from navdata message
 *
 * @return Altitude.
 */
static inline float mavlink_msg_navdata_get_altitude(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  12);
}

/**
 * @brief Get field heading from navdata message
 *
 * @return Heading.
 */
static inline float mavlink_msg_navdata_get_heading(const mavlink_message_t* msg)
{
	return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field battery from navdata message
 *
 * @return Battery Life.
 */
static inline uint32_t mavlink_msg_navdata_get_battery(const mavlink_message_t* msg)
{
	return _MAV_RETURN_uint32_t(msg,  20);
}

/**
 * @brief Decode a navdata message into a struct
 *
 * @param msg The message to decode
 * @param navdata C-struct to decode the message contents into
 */
static inline void mavlink_msg_navdata_decode(const mavlink_message_t* msg, mavlink_navdata_t* navdata)
{
#if MAVLINK_NEED_BYTE_SWAP
	navdata->drone_state = mavlink_msg_navdata_get_drone_state(msg);
	navdata->vx = mavlink_msg_navdata_get_vx(msg);
	navdata->vy = mavlink_msg_navdata_get_vy(msg);
	navdata->altitude = mavlink_msg_navdata_get_altitude(msg);
	navdata->heading = mavlink_msg_navdata_get_heading(msg);
	navdata->battery = mavlink_msg_navdata_get_battery(msg);
	navdata->seq_number = mavlink_msg_navdata_get_seq_number(msg);
	navdata->drone_number = mavlink_msg_navdata_get_drone_number(msg);
#else
	memcpy(navdata, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_NAVDATA_LEN);
#endif
}
