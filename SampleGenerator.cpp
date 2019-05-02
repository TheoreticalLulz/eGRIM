#include "SampleGenerator.h"

//-----------------------------------------------------------------------------
//! @brief Constructs a SampleGenerator instance.
//! @param addr The receiver address to associate with the socket.
//! @param port The communication port to complete packet transmissions.
//! @return Nothing.
//-----------------------------------------------------------------------------
SampleGenerator::SampleGenerator(char * addr, int port) : 
	active_process(false), packet_queue(), fpga_socket(), fpga_address(), 
	genthread(NULL), trxthread(NULL), queue_mutex() {

	// Declare all relevant variables.
	WSADATA wsadata;
	unsigned char multicastTTL;

	// Establish the windows socket startup sequence, cleaning up the socket 
	// data in the event of a failure.
	if (WSAStartup(MAKEWORD(2, 2), &wsadata) != NO_ERROR) {
		system("pause");
		WSACleanup();
		exit(10);
	}

	// Establish the communication socket under the UDP protocol.
	fpga_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (fpga_socket == INVALID_SOCKET) {
		system("pause");
		WSACleanup();
		exit(11);
	}

	// Establish multicasting within the client code.
	multicastTTL = 1;
	if (setsockopt(fpga_socket, IPPROTO_IP, IP_MULTICAST_TTL, (char *) 
		&multicastTTL, sizeof(multicastTTL)) < 0) {
		system("pause");
		WSACleanup();
		exit(11);
	}

	// Establish the FPGA address to send the data.
	memset((char*)&fpga_address, 0, sizeof(fpga_address));
	fpga_address.sin_family = AF_INET;
	fpga_address.sin_port = htons(port);
	inet_pton(AF_INET, addr, &fpga_address.sin_addr.s_addr);
}

//-----------------------------------------------------------------------------
//! @brief Destroys a SampleGenerator instance.
//! @return Nothing.
//-----------------------------------------------------------------------------
SampleGenerator::~SampleGenerator() {

	// Pause the system and clean up the socket structure.
	system("pause");
	WSACleanup();
	exit(0);
}

//-----------------------------------------------------------------------------
//! @brief Creates, enables, and commences the packet transmission process.
//! @param queue_len The maximum length of the packet queue.
//! @param packet_rate The delay period between packet transmisssions.
//! @param rotate_rate The angle of antenna rotation for a duration of one 
//! second.
//! @return Nothing.
//-----------------------------------------------------------------------------
void SampleGenerator::init(uint32_t queue_len, double packet_rate, double 
	rotate_start, double rotate_rate) {

	// Toggle the active process flag to enable the generator and transmitter 
	// threads.
	active_process = true;

	// Initialize and start the generator and transmitter threads.
	genthread = new std::thread(&SampleGenerator::generate, this, queue_len, 
		packet_rate, rotate_start, rotate_rate);
	trxthread = new std::thread(&SampleGenerator::transmit, this, packet_rate);
}

//-----------------------------------------------------------------------------
//! @brief Finishes the generator and transmitter threads, clears the queue
//! object, and performs cleanup operations on the socket.
//! @return Nothing.
//-----------------------------------------------------------------------------
void SampleGenerator::uninit() {

	// Lock the queue mutex to prevent pushing/popping samples.
	queue_mutex.lock();

	// Toggle the active process flag to false, allowing the threads to finish.
	active_process = false;

	// Unlock the queue to allow pushing/popping samples.
	queue_mutex.unlock();

	// Join the generator and transmitter threads.
	genthread->join();
	trxthread->join();

	// Free the memory taken be the created thread objects.
	free(genthread);
	free(trxthread);

	// Empty the packet queue of sample objects.
	while (!packet_queue.empty()) {
		packet_queue.pop();
	}

	// Close the socket connection and perform cleanup operations.
	closesocket(fpga_socket);
	WSACleanup();
}

//-----------------------------------------------------------------------------
//! @brief Generates an updated packet and pushes it onto the queue to be 
//! transmitted over the socket.
//! @param queue_len The maximum length of the packet queue.
//! @param packet_rate The delay period between packet transmisssions.
//! @param rotate_rate The angle of antenna rotation for a duration of one 
//! second.
//! @return Nothing.
//-----------------------------------------------------------------------------
void SampleGenerator::generate(uint32_t queue_len, double packet_rate, double 
	rotate_start, double rotate_rate) {

	// Initialize the variables with default values.
	Packet smpl;

	// Set the rotation rate of the antenna object.
	smpl.setAntPos(rotate_rate);

	// Continuously push updated data onto the queue.
	while (active_process) {

		// If the queue is larger than the maximum length, wait for a brief 
		// period before trying again.
		if (packet_queue.size() > queue_len - 1) {
			std::chrono::microseconds delay(50);
			std::this_thread::sleep_for(delay);
			continue;
		}

		// Update the antenna position at the specified rotation rate, and 
		// transmission period.
		smpl.updateAntPos(rotate_rate, packet_rate);

		// Lock the queue mutex to prevent popping samples from the object.
		queue_mutex.lock();

		// Push a copy of the Packet object onto the queue.
		packet_queue.push(smpl);

		// Unlock the queue to allow popping samples from the object.
		queue_mutex.unlock();
	}
}

//-----------------------------------------------------------------------------
//! @brief Generates an updated packet and pushes it onto the queue to be 
//! transmitted over the socket.
//! @param packet_rate The delay period between packet transmisssions.
//! @return Nothing.
//-----------------------------------------------------------------------------
void SampleGenerator::transmit(double packet_rate) {

	// Declare all relevant variables.
	Packet* smpl;
	uint32_t* trns;

	// Continuously push updated data onto the queue.
	while (active_process) {

		// If the queue is empty, wait for a brief period before trying again.
		if (packet_queue.empty()) {
			std::chrono::microseconds delay(50);
			std::this_thread::sleep_for(delay);
			continue;
		}

		// Initialize the variables with default values.
		smpl = new Packet();
		trns = new uint32_t[6];

		// Lock the queue mutex to prevent pushing samples onto the object.
		queue_mutex.lock();

		// Pop a sample from the queue and convert it to a format recognized by 
		// the FPGA device.
		*smpl = packet_queue.front();
		packet_queue.pop();
		smpl->convert(trns);

		// Unlock the queue to allow pushing samples onto the object.
		queue_mutex.unlock();

		// Send the retrieved packet over the communication socket.
		if (sendto(fpga_socket, (const char*)trns, 20, 0, (SOCKADDR*)
			&fpga_address, sizeof(struct sockaddr_in)) < 0) {
			int err = WSAGetLastError();
		}

		// Delay the next packet transmission by whatever period the user has
		// has specified.
		std::chrono::duration<double> delay(packet_rate);
		std::this_thread::sleep_for(delay);

		// Clear the generated sample and associated conversion from memory.
		free(smpl);
		free(trns);
	}
}