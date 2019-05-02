#include <stdint.h>

// Establish the globally accessible macros.
#define ROTATION_STEP 0.001373291015625
#define ROTATION_FULL 262144

//=============================================================================
//! @class Packet
//!
//! @addToGroup eGRIM
//=============================================================================
class Packet {
public:

	//-------------------------------------------------------------------------
	//! @fn Packet
	//!
	//! @brief Constructs a Packet instance.
	//-------------------------------------------------------------------------
	Packet(uint16_t sysid = 0, uint16_t mode = 0, uint8_t length = 0,
		uint32_t number = 0, uint32_t IRIG = 0, uint32_t position = 0, uint8_t
		transmitter = 0, uint8_t phasing = 0, uint8_t integration = 0, uint8_t
		range = 0, uint8_t channel = 0, uint8_t antmode = 0, uint8_t mswitch = 0,
		uint8_t PRI = 0, uint8_t mode_m = 0, uint8_t AFC = 0, uint8_t AGC = 0,
		uint8_t MGC = 0, uint8_t adj_range = 0);

	//-------------------------------------------------------------------------
	//! @fn updateAntPos
	//!
	//! @brief Sets the antenna position to the requested value.
	//-------------------------------------------------------------------------
	void setAntPos(double rotstart);

	//-------------------------------------------------------------------------
	//! @fn updateAntPos
	//!
	//! @brief Updates the antenna position for the requested rotation rate, at 
	//! the specified transmission period.
	//-------------------------------------------------------------------------
	void updateAntPos(double rotrate, double clkrate);

	//-------------------------------------------------------------------------
	//! @fn convert
	//!
	//! @brief Converts the object instance into a contiguous array of words.
	//-------------------------------------------------------------------------
	void convert(uint32_t* arr);
private:

	//! (WORD: 0, 16 bits) Binary System ID (See SYSTEM_ID.IN)
	uint16_t system_id;

	//! (WORD: 0, 16 bits) Mode Setting
	uint16_t mode_setting;

	//! (WORD: 1,  8 bits) Packet Length (=6)
	uint8_t packet_length;

	//! (WORD: 1, 24 bits) Packet Number
	uint32_t packet_number;

	//! (WORD: 2, 32 bits) IRIG Time (LSB=1 millisecond)
	uint32_t IRIG_time;

	//! (WORD: 3, 32 bits) Antenna Position (LSB=360/2^18  Degree)
	uint32_t antenna_position;

	//! (WORD: 4,  1 bits) Transmitter On/Off (0=Off, 1=On) 
	uint8_t transmitter_onoff;

	//! (WORD: 4,  1 bits) Antenna Phasing (0=Phase I, 1=Phase II)
	uint8_t antenna_phasing;

	//! (WORD: 4,  1 bits) Integration FP (0=Off, 1=On)
	uint8_t integration_FP;

	//! (WORD: 4,  2 bits) Range Scale (0=100KM, 1=150KM, 2=250KM, 3=0.5-50KM)
	uint8_t range_scale;

	//! (WORD: 4,  3 bits) Channel Select (0=CH1, 1=CH2, 2=CH3, 3=CH4, 4=CH5,
	//! 5=..., 6=..., 7=...)
	uint8_t channel_select;

	//! (WORD: 4,  3 bits) Antenna Mode (0=Fixed 6 RPM, 1=Fixed 12RPM, 2=Manual
	//! CW, 3=Manual CCW, 4=Stopped, 5=..., 6=..., 7=...)
	uint8_t antenna_mode;

	//! (WORD: 4,  2 bits) Mode Switch (0=Mode A, 1=Mode N, 2=Mode K, 3=Mode S)
	uint8_t mode_switch;

	//! (WORD: 4,  1 bits) PRI Select (0=PRI 1, 1=PRI 2)
	uint8_t PRI_select;

	//! (WORD: 4,  2 bits) Mode M Select (0=Sector, 1=Flicker 1, 2=Flicker 2, 
	//! 3=Not Selected)
	uint8_t mode_m_select;

	//! (WORD: 4,  1 bits) AFC On/Off (0=Off, 1=On)
	uint8_t AFC_onoff;

	//! (WORD: 4,  1 bits) AGC On/Off (0=Off, 1=On)
	uint8_t AGC_onoff;

	//! (WORD: 5,  8 bits) MGC Voltage (LSB=10/2^7 Volts)
	uint8_t MGC_voltage;

	//! (WORD: 5,  8 bits) Adjustable Range Scale (0.5-50KM)(LSB=10/2^7 Volts)
	//! (Unsigned)
	uint8_t adj_range_scale;
};