#include "Packet.h"

//-----------------------------------------------------------------------------
//! @brief Constructs a Packet instance.
//! @return Nothing.
//-----------------------------------------------------------------------------
Packet::Packet(uint16_t sysid, uint16_t mode, uint8_t length, uint32_t number,
	uint32_t IRIG, uint32_t position, uint8_t transmitter, uint8_t phasing,
	uint8_t integration, uint8_t range, uint8_t channel, uint8_t antmode,
	uint8_t mswitch, uint8_t PRI, uint8_t mode_m, uint8_t AFC, uint8_t AGC,
	uint8_t MGC, uint8_t adj_range) {


	// Establish the default class parameter values.
	system_id = sysid;
	mode_setting = mode;
	packet_length = length;
	packet_number = number;
	IRIG_time = IRIG;
	antenna_position = position;
	transmitter_onoff = transmitter;
	antenna_phasing = phasing;
	integration_FP = integration;
	range_scale = range;
	channel_select = channel;
	antenna_mode = antmode;
	mode_switch = mswitch;
	PRI_select = PRI_select;
	mode_m_select = mode_m;
	AFC_onoff = AFC;
	AGC_onoff = AGC;
	MGC_voltage = MGC;
	adj_range_scale = adj_range;
}

//-------------------------------------------------------------------------
//! @brief Sets the antenna position to the requested value.
//! @param rotstart The initial antenna position.
//! @return Nothing.
//-------------------------------------------------------------------------
void Packet::setAntPos(double rotstart) {

	antenna_position %= ROTATION_FULL;
}

//-----------------------------------------------------------------------------
//! @brief Updates the antenna position for the requested rotation rate, at the 
//! specified transmission period.
//! @param rotrate The rotation rate for the antenna, in degrees, at a constant
//! duration of one second.
//! @param clkrate The packet transmission rate, in seconds.
//! @return Nothing
//-----------------------------------------------------------------------------
void Packet::updateAntPos(double rotrate, double clkrate) {

	// Calculate the rotation of the antenna for the specified rate and period, 
	// converting the value to an eighteen bit representation.
	antenna_position += (uint32_t)(clkrate * rotrate / ROTATION_STEP);
	antenna_position %= ROTATION_FULL;
}

//-----------------------------------------------------------------------------
//! @brief Converts the object instance into a contiguous array of words.
//! @param arr A pointer to a memory location which shall be populated.
//! @return Nothing.
//-----------------------------------------------------------------------------
void Packet::convert(uint32_t* arr) {

	// Create an empty array of six words, and set an initial value.
	for (int i = 0; i < 6; ++i) {
		arr[i] = 0;
	}

	// Establish the first word of the array.
	arr[0] += system_id;
	arr[0] += mode_setting << 16;

	// Establish the second word of the array.
	arr[1] += packet_length;
	arr[1] += packet_number << 8;

	// Establish the third word of the array.
	arr[2] += IRIG_time;

	// Establish the fourth word of the array.
	arr[3] += antenna_position;

	// Establish the fifth word of the array.
	arr[4] += transmitter_onoff << 1;
	arr[4] += antenna_phasing << 2;
	arr[4] += integration_FP << 3;
	arr[4] += range_scale << 4;
	arr[4] += channel_select << 6;
	arr[4] += antenna_mode << 9;
	arr[4] += mode_switch << 12;
	arr[4] += PRI_select << 14;
	arr[4] += mode_m_select << 16;
	arr[4] += AFC_onoff << 18;
	arr[4] += AGC_onoff << 19;

	// Establish the sixth word of the array.
	arr[5] += MGC_voltage;
	arr[5] += adj_range_scale << 8;
}