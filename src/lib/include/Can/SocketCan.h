/** Copyright (C) 2015 ACTIA NORDIC AB. All rights reserved. **/

#ifndef SOCKETCAN_H
#define SOCKETCAN_H

#include <linux/can.h>
#include <linux/can/error.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <glib.h>
#include <unistd.h>
#include <functional>
#include <string>

/**
 *  Example class for showing one way of interacting with the
 *  SocketCAN functionality in the kernel using RAW sockets.
 **/
class SocketCAN {
public:
	using RcvFunc = std::function<void(uint32_t id, uint8_t data[8], uint8_t dlc)>;
	explicit SocketCAN(const std::string& interface) :
			interface_(interface), sockfd_(-1), channel_(nullptr) {

	}
	~SocketCAN() {
		if (channel_) {
			g_io_channel_unref(channel_);
		}
		::close(sockfd_);
	}

	bool receive(RcvFunc f) {
		rcv_f_ = f;
		std::cout << "Opening can interface " << interface_ << std::endl;
		// In the ACU-III, protocol CAN_RAW can't be used. Instead, use
		// protocol id 7.
		sockfd_ = socket(PF_CAN, SOCK_RAW, 7);

		if (-1 == sockfd_) {
			std::cerr << "Failed to open CAN_RAW socket: " << strerror(errno)
					<< std::endl;
			return false;
		}

		// Find out the interface index for specified interface
		struct ifreq ifr;
		snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", interface_.c_str());
		if (-1 == ioctl(sockfd_, SIOCGIFINDEX, &ifr)) {
			std::cerr << "Failed to get interface index for " << interface_
					<< ": " << strerror(errno) << std::endl;
			return false;
		}

		// Bind the socket to the interface
		struct sockaddr_can addr;
		addr.can_family = PF_CAN;
		addr.can_ifindex = ifr.ifr_ifindex;
		if (-1 == bind(sockfd_, (struct sockaddr*) &addr, sizeof(addr))) {
			std::cerr << "Failed  to bind to interface " << interface_ << ": "
					<< strerror(errno) << std::endl;
			return false;
		}

		// Stick the socket inside a g_io_channel to get callbacks
		// when data is available
		channel_ = g_io_channel_unix_new(sockfd_);
		g_io_add_watch(channel_, G_IO_IN, SocketCAN::s_Readable, this);

		return true;
	}

	bool send(uint32_t id, uint8_t *data, uint8_t dlc) {
		struct can_frame frame;

		std::cout << "TX: id=0x" << std::hex << id << std::endl;

		frame.can_id = id;
		frame.can_dlc = dlc;
		memcpy(frame.data, data, dlc);

		if (sizeof(frame) != write(sockfd_, &frame, sizeof(frame))) {
			perror("CAN frame write fail:");
			return false;
		}

		return true;
	}

private: static gboolean s_Readable(GIOChannel *source, GIOCondition condition,
			void *data) {
		static_cast<SocketCAN*>(data)->Readable();
		return TRUE;
	}

	void Readable() {
		struct can_frame frame;
		int n_read = read(sockfd_, &frame, sizeof(frame));

		if (-1 == n_read) {
			perror("SocketCAN::receive()");
		}

		if (n_read == sizeof(frame)) {
			rcv_f_(frame.can_id, frame.data, frame.can_dlc);
		} else {
			std::cerr << "Incomplete CAN frame received, read " << n_read
					<< std::endl;
		}
	}
protected:
	std::string interface_;
	int sockfd_;
	GIOChannel *channel_;
	RcvFunc rcv_f_;

};

#endif
