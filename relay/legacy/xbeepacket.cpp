#include "xbeepacket.h"

XBeePacket::XBeePacket(QThread *parent) :
    QThread(parent)
{
}

/*
 * This function handles all XBeePacket errors;
 * Currently XBeePacket throws fatal error becuase no errors originate from XBeePacket
 *
 */
bool XBeePacket::handleError(int i) {
    switch (i) {
    default:
        return FATAL_ERROR;
        break;
    }
}

/*
 * This is the execution loop for XBeePacket.
 * All unhandled events are processed then XBeePacket checks to see if anything has been sent to the GCS.
 *
 */
void XBeePacket::run() {
    while (1) {
        QCoreApplication::processEvents();
        ackMux.lock();
        read();
        ackMux.unlock();
        usleep(XBEEPACKET_SLEEP_TIME);
    }
}

/*
 * This is the slot that XBeePacket uses to send commands.
 * Signals sent are queued and handled in run by QCoreApplication::processEvents()
 *
 */

void XBeePacket::xBSendSlot(struct COMMAND c) {
    qDebug() << "Command signal received from gcs";
    ackMux.lock();
    sendCommand(c);
    ackMux.unlock();
}

/*
 * This function sends an array to XBee to read raw data that XBeePacket then formats
 *
 */

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
                    qDebug() << "Got ACK Message from drone: " << ack.drone_number << " seqNum: " << ack.seq_number;
                    break;
                case MAVLINK_MSG_ID_NAVDATA:
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
                    break;
                default:
                    break;
                }
            }
        }
    }

    //we still have to update the ack list even if we dont read anything
    checkEntry(-1, -1);
}

/*
 * This is an internal function that sends acks for received data.
 * This function does not get used by connection commands because the droneid is not known
 *
 */

void XBeePacket::sendACK(int droneId, int seqNumIn, int mapNumIn) {
    mavlink_message_t message;
    mavlink_msg_ack_pack(0, 0, &message, droneId, seqNumIn, mapNumIn); //seqNum of command sent
    xb.sendMsg(message);
}

/*
 * Internal function that packs commands using the mavlink header and sends the command to XBee
 *
 */

void XBeePacket::sendCommand(struct COMMAND c) {
    if (c.droneid >= MAX_DRONES || c.droneid < 0) {
        return;
    }
    qDebug() << "Got a command to send";
    if (std::find(disconnectedDrones.begin(), disconnectedDrones.end(), c.droneid) != disconnectedDrones.end()) {
        //we dont send messages to a drone that is disconnected
        //prompt user that drone is dc
        qDebug() << "Can't send command to disconnected drone";
        //return;
    }
    mavlink_message_t message;
    switch (c.cmdid) {
    case ROTATE_COMMAND:
    case LAND_COMMAND:
    case TAKEOFF_COMMAND:
    case FORWARD_COMMAND:
    case HOVER_COMMAND:
        qDebug() << "MESSAGE SENT!!!";
        mavlink_msg_cmdmsg_pack(0, 0, &message, seqNum, c.cmdid, c.droneid, c.param1, c.param2, c.param3);
        qDebug() << "packet: command packed";
        break;
    //for ack commands sequence number is stored in param1 and map number in param2
    case ACK_COMMAND:
        mavlink_msg_ack_pack(0, 0, &message, c.droneid, (int)c.param1, (int)c.param2);
        qDebug() << c.droneid << " droneId";
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
    qDebug() << "Command sent";
    //add the sent command to the ack db
    if (c.cmdid != ACK_COMMAND) {
        addEntry(c);
    }
}

/*
 * This function stores sent commands and increments the seqNum counter.
 *
 */
void XBeePacket::addEntry(const struct COMMAND c) {
    struct ACK_ENTRY a;
    time(&a.start);
    a.timeouts = 0;
    a.command = c;
    a.seqNum = seqNum;
    pendingAcks[c.droneid].insert(std::pair<int, struct ACK_ENTRY>(seqNum, a));
    seqNum++;
}

/*
 * This function updates the ack db by either removing ack'd commands or by checking to see if a timeout has occured.
 * If enough timeouts have occured the drone is considered disconnected and added to the disconnected list
 *
 */

void XBeePacket::checkEntry(int droneId, int seqNumIn) {
    if (droneId >= 0 && droneId < MAX_DRONES) {
        //search for the entry
        std::map<int, struct ACK_ENTRY>::iterator it = pendingAcks[droneId].find(seqNumIn);

        //if we found it
        if (it != pendingAcks[droneId].end()) {
            std::list<int>::iterator droneIt = std::find(disconnectedDrones.begin(), disconnectedDrones.end(), it->first);
            //and if the drone is disconnected
            if (droneIt != disconnectedDrones.end()) {
                //remove it from the disconnected list
                disconnectedDrones.erase(droneIt);
            }
            //remove the ack entry
            pendingAcks[droneId].erase(it);
        }
    }
    //Now we check for timeouts and disconnections on all drones
    time_t end;
    time(&end);
    for (int i = 0; i < MAX_DRONES; i++) {
        //if the drone isnt disconnected --we don't update timeout counters for drones that have dc'd--
        if (std::find(disconnectedDrones.begin(), disconnectedDrones.end(), i) == disconnectedDrones.end()) {
            //if the drone is waiting for acks
            if (!pendingAcks[i].empty()) {
                double seconds = difftime(end, pendingAcks[i].begin()->second.start);
                if (seconds > XBEETIMEOUT_TIME) {
                    pendingAcks[i].begin()->second.timeouts++;
                    qDebug() << "Timeout has occured on drone " << i;
                    //timeout has occured so resend the command
                    resendCommand(pendingAcks[i].begin()->second);
                    if (pendingAcks[i].begin()->second.timeouts > XBEETIMEOUT_THRESHOLD) {
                        //the drone has officially been disconnected
                        disconnectedDrones.push_back(pendingAcks[i].begin()->second.command.droneid);
                        qDebug() << "Drone " << i << " has disconnected\n";
                    }
                }
            }
        }
    }

}

/*
 * This helper function resends commands using the same sequence number it was initially sent with.
 * We don't resend ACKs because we don't ACK ACKs
 *
 */
void XBeePacket::resendCommand(struct ACK_ENTRY &a) {
    mavlink_message_t message;
    switch (a.command.cmdid) {
    case TAKEOFF_COMMAND:
    case LAND_COMMAND:
        mavlink_msg_cmdmsg_pack(0, 0, &message, a.seqNum, a.command.cmdid, a.command.droneid, a.command.param1, a.command.param2, a.command.param3);
        break;
    default:
        message.len = 0;
        break;
    }

    if (message.len != 0) {
        xb.sendMsg(message);
    }
    //update the timer for timeouts
    time(&a.start);
}

/*
 * This function initializes XBeePacket
 *
 */
void XBeePacket::setup() {
    try {
        xb.setup();
    } catch (int i){
        if (xb.handleError(i) == FATAL_ERROR) {
            throw FATAL_ERROR;
        }
    }

    seqNum = INIT_SEQNUM;
}