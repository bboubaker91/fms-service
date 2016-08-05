/** Copyright (C) 2014 ACTIA NORDIC AB. All rights reserved. **/

#ifndef CAN_BROADCASTMANAGER_H
#define CAN_BROADCASTMANAGER_H

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/bcm.h>

#include <string>
#include <cstring>
#include <iostream>
#include <cerrno>
#include <unistd.h>
#include <string>

#include "CanTypeDefs.h"

/**
 *  @brief Example class for showing one way of interacting with the
 *         BroadcastManager functionality in the kernel.
 **/
class BroadcastManager {

public:
	//  RcvFunc = std::function<void(uint32_t id, uint8_t data[8], uint8_t dlc)>;
	BroadcastManager();
	~BroadcastManager();



	/**
	 *  @brief Open a connection to a specific CAN network interface
	 *
	 *  @param ifc - Name of the CAN network interface to open
	 */
	bool open(const CanInterface canInterface) {
		std::string ifc;
		if (canInterface == CanInterface::can0) {
			ifc = "can0";
		}
		if (canInterface == CanInterface::can1) {
			ifc = "can1";
		}

		std::cout << "Opening can interface " << ifc << std::endl;
		interface_ = ifc;

	// The socket for communicating with the BroadcastManager needs to
	// be a SOCK_DGRAM socket of type CAN_BCM
	#ifdef __arm__
	// TODO: The CAN_SBCM should be included in SDK, or else we should
	// replace normal CAN_BCM
		sockfd_ = socket(PF_CAN, SOCK_DGRAM, 8);
	#else
		sockfd_ = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
	#endif

		if (-1 == sockfd_) {
			std::cerr << "Failed to open CAN_BCM socket: " << strerror(errno) << "("
					<< errno << ")" << std::endl;
			return false;
		}

		struct ifreq ifr;
	// Bind the socket to a specific CAN interface
		snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", ifc.c_str());
		if (-1 == ioctl(sockfd_, SIOCGIFINDEX, &ifr)) {
			std::cerr << "Failed to get interface index for " << ifc << ": "
					<< strerror(errno) << std::endl;
			return false;
		}

		struct sockaddr_can addr;
		addr.can_family = PF_CAN;
		addr.can_ifindex = ifr.ifr_ifindex;

	// Instead of using bind as in the CAN_RAW solution we need to use
	// a connect call
		if (-1 == connect(sockfd_, (struct sockaddr*) &addr, sizeof(addr))) {
			std::cerr << "Failed  to connect to interface " << ifc << ": "
					<< strerror(errno) << std::endl;
			return false;
		}

		return true;
	};

	bool send(uint16_t id, uint8_t *data, uint8_t dlc);

	/**
	 *  @brief Setup a cyclic transmission of a specific can frame
	 */
	bool send_periodic(uint16_t id, uint8_t *data, uint8_t dlc,
			uint32_t interval_ms);

	/**
	 *  @brief Cancel period send of a specific id
	 *
	 *  @param id - The id number to cancel period transmissions for
	 **/
	bool cancel_send_periodic(uint16_t id);

	/**
	 *  @brief Do a blocking read on the bus, output data is written
	 *          to argument pointers
	 **/
	bool receive(uint16_t *id, uint8_t *data, uint8_t *dlc);

	/**
	 *  @brief Setup a filter for receiving of data.
	 *          Can be called more than one time for setting up
	 *          reception of multiple frames
	 *
	 *  @param filter - Type of filter to be set, for description see
	 *                   \ref FilterType
	 *
	 *  @param id - CAN Id to use in filter
	 **/
	bool rx_filter_setup(enum FilterType filter, uint16_t id);

private:
	size_t setup_rx_filter(struct msg *bcm, enum FilterType filter,
			uint16_t id);
	std::string filter_to_string(enum FilterType filter);
	uint32_t filter_to_opcode(enum FilterType filter);

	std::string interface_;
	int sockfd_;

};

BroadcastManager::BroadcastManager() :
		sockfd_(-1)

{

}

BroadcastManager::~BroadcastManager() {
	if (sockfd_) {
		// Error handling should be done for close
		close(sockfd_);
	}
}

/*bool BroadcastManager::open(const CanInterface canInterface) {
	std::string ifc;
	if (canInterface == CanInterface::can0) {
		ifc = "can0";
	}
	if (canInterface == CanInterface::can1) {
		ifc = "can1";
	}

	std::cout << "Opening can interface " << ifc << std::endl;
	interface_ = ifc;

// The socket for communicating with the BroadcastManager needs to
// be a SOCK_DGRAM socket of type CAN_BCM
#ifdef __arm__
// TODO: The CAN_SBCM should be included in SDK, or else we should
// replace normal CAN_BCM
	sockfd_ = socket(PF_CAN, SOCK_DGRAM, 8);
#else
	sockfd_ = socket(PF_CAN, SOCK_DGRAM, CAN_BCM);
#endif

	if (-1 == sockfd_) {
		std::cerr << "Failed to open CAN_BCM socket: " << strerror(errno) << "("
				<< errno << ")" << std::endl;
		return false;
	}

	struct ifreq ifr;
// Bind the socket to a specific CAN interface
	snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", ifc.c_str());
	if (-1 == ioctl(sockfd_, SIOCGIFINDEX, &ifr)) {
		std::cerr << "Failed to get interface index for " << ifc << ": "
				<< strerror(errno) << std::endl;
		return false;
	}

	struct sockaddr_can addr;
	addr.can_family = PF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

// Instead of using bind as in the CAN_RAW solution we need to use
// a connect call
	if (-1 == connect(sockfd_, (struct sockaddr*) &addr, sizeof(addr))) {
		std::cerr << "Failed  to connect to interface " << ifc << ": "
				<< strerror(errno) << std::endl;
		return false;
	}

	return true;
}*/

// All messages sent to configure BroadcastManager expects a structure
// of this form; first a message header and then a number of can
// frames referred to by the header.
struct msg {
	struct bcm_msg_head head;
	struct can_frame frame;
};

bool BroadcastManager::send(uint16_t id, uint8_t *data, uint8_t dlc) {
	struct msg bcm;

	bcm.head.opcode = TX_SEND;

	bcm.frame.can_id = id;
	bcm.frame.can_dlc = dlc;
	memcpy(bcm.frame.data, data, dlc);

	std::cout << "TX_SETUP: id=0x" << std::hex << id << std::endl;

	if (sizeof(bcm) != write(sockfd_, &bcm, sizeof(bcm))) {
		std::cerr << "CAN frame write fail: " << strerror(errno) << std::endl;
		return false;
	}

	return true;
}

bool BroadcastManager::send_periodic(uint16_t id, uint8_t *data, uint8_t dlc,
		uint32_t interval_ms) {
	struct msg bcm;

	bcm.head.opcode = TX_SETUP;
	bcm.head.flags = SETTIMER | STARTTIMER | TX_CP_CAN_ID;
// Only use a single interval for sending, see the next comment
	bcm.head.count = 0;
// ival1 can be used to send with one interval for 'count' times
// before start using ival2 as interval
	bcm.head.ival1.tv_sec = 0;
	bcm.head.ival1.tv_usec = 0;
	bcm.head.ival2.tv_sec = interval_ms / 1000;
	bcm.head.ival2.tv_usec = (interval_ms % 1000) * 1000;
	bcm.head.can_id = id;
// It is possible to add multiple frames to be sent, if nframes is
// set to a larger value the BroadcastManger will cycle between all
// of the frames specified. In this case the 'struct can_frame'
// will need to be sized accordingly in 'struct msg'
	bcm.head.nframes = 1;
	bcm.frame.can_id = id;
	bcm.frame.can_dlc = dlc;
	memcpy(bcm.frame.data, data, dlc);

	std::cout << "TX_SETUP: id=0x" << std::hex << id << " period=" << std::dec
			<< interval_ms << " ms " << std::endl;

	if (sizeof(bcm) != write(sockfd_, &bcm, sizeof(bcm))) {
		std::cerr << "CAN frame write fail: " << strerror(errno) << std::endl;
		return false;
	}

	return true;
}

bool BroadcastManager::cancel_send_periodic(uint16_t id) {
	struct bcm_msg_head bcm;
	bcm.opcode = TX_DELETE;
	bcm.can_id = id;
	std::cout << "Removing periodic send of id=0x" << std::hex << id
			<< std::endl;
	if (sizeof(bcm) != write(sockfd_, &bcm, sizeof(bcm))) {
		std::cerr << "CAN frame write fail on cancel: " << strerror(errno)
				<< std::endl;
	}
	return true;
}

bool BroadcastManager::rx_filter_setup(enum FilterType filter, uint16_t id) {
	struct msg bcm;
	ssize_t msg_size = setup_rx_filter(&bcm, filter, id);

	std::cout << "Adding " << filter_to_string(filter) << " for id 0x"
			<< std::hex << id << std::endl;

// Set the filter by writing the 'struct msg' containing of a
// bcm_msg_head and (optionally) a CAN frame. The information in
// this structure is used by the BroadcastManager in the kernel to
// decide what incoming messages on the bus will be forwarded to
// our socket
	while (msg_size != write(sockfd_, &bcm, msg_size)) {
		if (errno != EAGAIN) {
			std::cerr << "CAN frame write fail on rx_setup: " << strerror(errno)
					<< std::endl;
			return false;
		}
	}
	return true;
}

size_t BroadcastManager::setup_rx_filter(struct msg *bcm,
		enum FilterType filter, uint16_t id) {
	memset(bcm, 0, sizeof(struct msg)); /* clear timers, nframes, etc. */

	bcm->head.opcode = RX_SETUP;
	bcm->head.can_id = id;
	bcm->head.flags = filter_to_opcode(filter);

	size_t msg_size = sizeof(struct bcm_msg_head);

// All other filter types than id filtering needs a frame in
// addition to the bcm_msg_head
	if (RX_FILTER_ID != filter) {
		bcm->head.nframes = 1;
		msg_size = sizeof(struct msg);
	}

	if (FILTER_DATA == filter) {
		// Then filtering for data change the frame data is used as a
		// mask to choose which bits to detect changes for, in this
		// example we choose to mask on any change
		memset(bcm->frame.data, 0xff, 8);
	}

	return msg_size;
}

uint32_t BroadcastManager::filter_to_opcode(enum FilterType filter) {
	switch (filter) {
	case FILTER_ID:
		return RX_FILTER_ID;

	case FILTER_DLC:
		return RX_CHECK_DLC;

	case FILTER_DATA:
		return 0;

	default:
		return 0;
	}
}

std::string BroadcastManager::filter_to_string(enum FilterType filter) {
	switch (filter) {
	case FILTER_ID:
		return "FILTER_ID";

	case FILTER_DLC:
		return "FILTER_DLC";

	case FILTER_DATA:
		return "FILTER_DATA";

	default:
		return "";
	}
}

bool BroadcastManager::receive(uint16_t *id, uint8_t *data, uint8_t *dlc) {
	struct msg bcm;

// Socket is configured for blocking read as default. This socket
// could be incorporated into a normal 'select' call instead to be
// able to do other things while waiting for CAN data
	int n_read = read(sockfd_, &bcm, sizeof(bcm));

// Right now we only care about full 'struct msg' reception. It is
// also possible that we will get just a bcm_msg_head message with
// another opcode than RX_CHANGED
	if (n_read == sizeof(bcm) && bcm.head.opcode == RX_CHANGED) {
		*id = bcm.frame.can_id;
		*dlc = bcm.frame.can_dlc;
		memcpy(data, bcm.frame.data, bcm.frame.can_dlc);

		return true;
	} else {
		std::cerr << "Received incomplete BCM frame" << std::endl;
		return false;
	}
}

#endif
