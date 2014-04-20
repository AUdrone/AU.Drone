CC=g++
UDEV=-ludev

CommRelay: comm_relay.cpp legacy/xbee.cpp
	$(CC) comm_relay.cpp legacy/xbee.cpp -o CommRelay $(UDEV) -g

clean:
	rm -rf CommRelay
