#include "xbeepacket.h"

XBeePacket::XBeePacket()
{
}

void XBeePacket::run() {
    while (1) {
      //  ackMux.lock();
        read();
      //  ackMux.unlock();
        usleep(XBEEPACKET_SLEEP_TIME);
    }
}

void XBeePacket::xBSendSlot(struct COMMAND c) {
    cout << "Command signal received from gcs";
  //  ackMux.lock();
    sendCommand(c);
  //  ackMux.unlock();
}

void XBeePacket::read() {
    int bytesRead;

    //send the buffer to XBee
    xb.readMsg(mavlinkBuffer, bytesRead); //might need this in try catch

    //if we have a read any data
    if (bytesRead > 0) {
        u_int8_t messageReceived = false;
        mavlink_message_t message;
        mavlink_status_t status;
        //parse every byte read into mavlink internal buffer
        //data sent to the internal buffer is remembered by mavlink
        for (int i = 0; i < bytesRead; i++) {
            messageReceived = mavlink_parse_char(MAVLINK_COMM_1, mavlinkBuffer[i], &message, &status);
            //if we have a full message
            if (messageReceived) {
                switch (message.msgid) {
                //for connection commands, sequence number is stored in param1
                case MAVLINK_MSG_ID_CONNECT:
                    mavlink_connect_t conn;
                    mavlink_msg_connect_decode(&message, &conn);
                    struct COMMAND c;
                    c.cmdid = CONNECT_COMMAND;
                    c.param1 = (float)conn.seq_number;
                    emit xBCommandReceived(c);
                    break;

                //we received an ack so check for the sequence number in the ack db
                case MAVLINK_MSG_ID_ACK:
                    mavlink_ack_t ack;
                    mavlink_msg_ack_decode(&message, &ack);
                    checkEntry(ack.drone_number, ack.seq_number);
		//changed next line
                    cout << "Got ACK Message from drone: " << ack.drone_number << " seqNum: " << ack.seq_number;
                    break;
          /*      case MAVLINK_MSG_ID_NAVDATA:
                    mavlink_navdata_t navdata;
                    mavlink_msg_navdata_decode(&message, &navdata);
                    struct NAVDATA n;
                    n.droneId = navdata.drone_number;
                    n.altitude = navdata.altitude;
                    n.battery = navdata.battery;
                    n.dronestate = navdata.drone_state;
                    n.heading = navdata.heading;
                    n.seqNum = navdata.seq_number;
                    n.vx = navdata.vx;
                    n.vy = navdata.vy;
                    sendACK(n.droneId, n.seqNum, -1);
                    emit xBNavdataReceived(n);
                    break;
                case MAVLINK_MSG_ID_VIDSTREAM:
                    qDebug() << "Got vid stream";
                    mavlink_vidstream_t vid;
                    mavlink_msg_vidstream_decode(&message, &vid);
                    struct VIDSTREAM v;
                    v.droneId = vid.drone_number;
                    v.seqNum = vid.seq_number;
                    memcpy(v.buff, vid.vidstream, sizeof(v.buff));
                    sendACK(v.droneId, v.seqNum, -1);
                    xBVidStreamReceived(v);
                    break; */
                default:
                    break;
                }
            }
        }
    }

    //we still have to update the ack list even if we dont read anything
    checkEntry(-1, -1);
}

void XBeePacket::sendCommand(struct COMMAND c) {
    if (c.droneid >= MAX_DRONES || c.droneid < 0) {
        return;
    }
    cout << "Got a command to send";
    if (std::find(disconnectedDrones.begin(), disconnectedDrones.end(), c.droneid) != disconnectedDrones.end()) {
        //we dont send messages to a drone that is disconnected
        //prompt user that drone is dc
        qDebug << "Can't send command to disconnected drone";
        //return;
    }
    mavlink_message_t message;
    switch (c.cmdid) {
    case ROTATE_COMMAND:
    case LAND_COMMAND:
    case TAKEOFF_COMMAND:
    case FORWARD_COMMAND:
    case HOVER_COMMAND:
        cout << "MESSAGE SENT!!!";
        mavlink_msg_cmdmsg_pack(0, 0, &message, seqNum, c.cmdid, c.droneid, c.param1, c.param2, c.param3);
        cout << "packet: command packed";
        break;
    //for ack commands sequence number is stored in param1 and map number in param2
    case ACK_COMMAND:
        mavlink_msg_ack_pack(0, 0, &message, c.droneid, (int)c.param1, (int)c.param2);
        cout << c.droneid << " droneId";
        break;
    default:
        message.len = 0;
        break;
    }

    if (message.len != 0) {
        try {
            xb.sendMsg(message);
        } catch (int i) {
            if (xb.handleError(i) == FATAL_ERROR) {
                throw FATAL_ERROR;
            }
        }
    }
    cout << "Command sent";
    //add the sent command to the ack db
    if (c.cmdid != ACK_COMMAND) {
        addEntry(c);
    }
}
