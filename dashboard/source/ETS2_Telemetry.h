#pragma once

#include <string.h>
#include <math.h>

typedef unsigned char byte;

extern const char* sharedMemFileName;

#define TEL_DATA_BLOCK_SIZE 1024
#define TEL_DATA_EMPTY_TIMESTAMP 0 // if a timestamp is equal to this, 
                                   // means there is no data available
#define TEL_DATA_STRING_SIZE 64

unsigned int getTimestamp(void* source);

struct TETS2_Telemetry
{
	/* constructors */
	TETS2_Telemetry();

	/* fields */
	// game
	unsigned int time;
	bool paused;
	unsigned int gameVersionMajor; // incorrect
	unsigned int gameVersionMinor; // incorrect
	// time
	unsigned int absoluteTime; // timestamp in minutes
	unsigned short week;
	byte day;
	byte hour;
	byte minute;
	// truck
	char truckManufacturer[TEL_DATA_STRING_SIZE];
	char truckModel[TEL_DATA_STRING_SIZE];
	unsigned int odometer; // km
	float engineWear;
	float transmissionWear;
	float cabinWear;
	float chassisWear;
	float wheelWear;
	float truckWear;
	// engine
	bool engineRunning;
	unsigned int engineRPM;
	unsigned int engineRPMMax;
	// brakes
	unsigned int retarderBrake;
	bool parkingBrake;
	bool engineBrake;
	// fuel
	unsigned int fuel; // litres
	unsigned int fuelCapacity; // litres
	unsigned int fuelRange; // km
	// transmission
	int gear;
	int gearDashboard;
	// miscellaneous
	bool electricEnabled;
	bool wipers;
	unsigned int airPressure; // psi
	// lights
	bool blinkerLeft;
	bool blinkerRight;
	bool lightsParking;
	bool lightsBeamLow;
	bool lightsBeamHigh;
	bool lightsBeacon;
	// warnings
	bool fuelWarning;
	bool batteryVoltageWarning;
	bool airPressureWarning;
	bool airPressureEmergency;
	bool adBlueWarning;
	bool oilPressureWarning;
	bool waterTemperatureWarning;
	// temperature
	float brakeTemperature;
	float oilTemperature;
	float waterTemperature;
	// trailer
	bool trailerAttached;
	char cargo[TEL_DATA_STRING_SIZE];
	unsigned int trailerMass; // kg
	float trailerWear;
	// job info
	bool onJob;
	bool jobFinished;
	unsigned int jobIncome;
	int jobDeadline; // same as absoluteTime, -1 if WoT job
	char jobCitySource[TEL_DATA_STRING_SIZE];
	char jobCityDestination[TEL_DATA_STRING_SIZE];
	char jobCompanySource[TEL_DATA_STRING_SIZE];
	char jobCompanyDestination[TEL_DATA_STRING_SIZE];
	// navigation
	int speedKmh;
	int speedMph;
	unsigned int speedLimitKmh;
	unsigned int speedLimitMph;
	unsigned int routeDistance; // meters
	unsigned int routeTime; // minutes
	// cruise control
	bool cruiseControl;
	int cruiseControlSpeedKmh;
	int cruiseControlSpeedMph;

	/* methods */
	bool load(void* source);
	bool load(void* source, bool &changed);
};