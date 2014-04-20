
#include <list>
#include <map>
#include <cstdio>
#include <string>

#include <ConstantsAndHeaders.h>
#include <xbeepacket.h>

GCSController::GCSController()
{
}

void GCSController::setup(Ui::MainWindow *_ui) {
    currentDrone = -1;
    overviewDroneId = -1;
    xb = new XBeePacket;
    navdataStream = new NavdataStream;
    for (int i = 0; i < MAX_DRONES; i++) {
        freeDrones.push_back(i);
    }
       try {
        xb->setup();
    } catch (int i) {
        if (xb->handleError(i) == FATAL_ERROR) {
            cout << "crap";
            this->quit();
        }
    }

void GCSController::receiveCommand(struct COMMAND c) {
    switch(c.cmdid) {
    case CONNECT_COMMAND: //the only time we get an ack command is when a drone is connecting
        if (!freeDrones.empty()) {
            c.cmdid = ACK_COMMAND;
            c.droneid = freeDrones.front();
           // qDebug() << "New drone id: " << c.droneid;
            freeDrones.pop_front();
            //c.param1 already has seqNum stored in it
            c.param2 = c.droneid; //hard coding map number for right now
            //create necessary threads here
            //new vid thread
           // connect(this, SIGNAL(gotVidStream(VIDSTREAM)), &vidThreads[c.droneid], SLOT(receiveVidStream(VIDSTREAM)), Qt::QueuedConnection);
           // vidThreads[c.droneid].start();
           // qDebug() << "Setting drone id to: " << c.droneid;
            emit sendCommand(c);
        } else {
            //we have max amount of drones --do nothing for right now
        }
        break;
    default:
        break;
    }
}
