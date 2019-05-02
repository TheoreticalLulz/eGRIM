#include "stdafx.h"
#include <queue>
#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <tchar.h>
#include <WS2tcpip.h>
#include <winsock2.h>
#include "Packet.h"

//=============================================================================
//! @class SampleGenerator
//!
//! @addToGroup eGRIM
//=============================================================================
class SampleGenerator {
public:

	//-------------------------------------------------------------------------
	//! @fn SampleGenerator
	//!
	//! @brief Constructs a SampleGenerator instance.
	//-------------------------------------------------------------------------
	SampleGenerator(char* addr, int port);

	//-------------------------------------------------------------------------
	//! @fn ~SampleGenerator
	//!
	//! @brief Destroys a SampleGenerator instance.
	//-------------------------------------------------------------------------
	~SampleGenerator();

	//-------------------------------------------------------------------------
	//! @fn init
	//!
	//! @brief Creates, enables, and commences the packet transmission process.
	//-------------------------------------------------------------------------
	void init(uint32_t queue_len, double packet_rate, double rotate_start, 
		double rotate_rate);

	//-------------------------------------------------------------------------
	//! @fn uninit
	//!
	//! @brief Finishes the generator and transmitter threads, clears the queue
	//! object, and performs cleanup operations on the socket.
	//-------------------------------------------------------------------------
	void uninit();

	//-------------------------------------------------------------------------
	//! @fn generate
	//!
	//! @brief Generates an updated packet and pushes it onto the queue to be 
	//! transmitted over the socket.
	//-------------------------------------------------------------------------
	void generate(uint32_t queue_len, double packet_rate, double rotate_start, 
		double rotate_rate);

	//-------------------------------------------------------------------------
	//! @fn transmit
	//!
	//! @brief Transmits a packet over the socket connection at a continuous 
	//! rate of time.
	//-------------------------------------------------------------------------
	void transmit(double packet_rate);
private:

	//! A flag to restrict the operation of the generation and transmission 
	//! threads.
	std::atomic<bool> active_process;

	//! A queue of Packet objects to be transmitted at a constant rate.
	std::queue<Packet> packet_queue;

	//! A communication socket over which Packets are transferred to the FPGA 
	//! device.
	SOCKET fpga_socket;

	//! An address representing the location of the FPGA device.
	sockaddr_in fpga_address;

	//! A pointer to the packet-generating thread.
	std::thread* genthread;

	//! A pointer to the packet-transmitting thread.
	std::thread* trxthread;

	//! A mutex to prevent simultaneous popping and pushing of samples onto the 
	//! queue object.
	std::mutex queue_mutex;
};