#include "ETS2_Telemetry.h"

const char* sharedMemFileName = "Local\\SimTelemetryETS2";

#define MINUTES_IN_HOUR 60
#define HOURS_IN_DAY 24
#define MINUTES_IN_DAY (HOURS_IN_DAY * MINUTES_IN_HOUR)
#define DAYS_IN_WEEK 7
#define MINUTES_IN_WEEK (DAYS_IN_WEEK * MINUTES_IN_DAY)

/* helper methods */
byte getByte(void* source, size_t offset)
{
	return *(byte*)(void*)((char*)source + offset);
}

int getInt(void* source, size_t offset)
{
	return *(int*)(void*)((char*)source + offset);
}

unsigned int getUInt(void* source, size_t offset)
{
	return *(unsigned int*)(void*)((char*)source + offset);
}

float getFloat(void* source, size_t offset)
{
	return *(float*)(void*)((char*)source + offset);
}

void getString(void* source, char* destination, size_t offset)
{
#ifdef _WIN32
	strcpy_s(destination, TEL_DATA_STRING_SIZE, (char*)(source)+offset);
#else
	strcpy(destination, (char*)(source)+offset);
#endif
}

bool getBool(void* source, size_t offset)
{
	return getByte(source, offset);
}

enum Aux
{
	CruiseControl,
	Wipers,
	ParkingBrake,
	EngineBrake,
	ElectricEnabled,
	EngineEnabled,

	BlinkerLeftActive,
	BlinkerRightActive,
	BlinkerLeftOn,
	BlinkerRightOn,

	LightsParking,
	LightsBeamLow,
	LightsBeamHigh,
	LightsAuxFront,
	LightsAuxRoof,
	LightsBeacon,
	LightsBrake,
	LightsReverse,

	BatteryVoltageWarning,
	AirPressureWarning,
	AirPressureEmergency,
	AdBlueWarning,
	OilPressureWarning,
	WaterTemperatureWarning
};

bool getAux(void* source, size_t offset, Aux aux)
{
	return getByte(source, offset + aux) > 0;
}

/* other methods */
unsigned int getTimestamp(void* source)
{
	return getUInt(source, 0);
}

/* telemetry data struct methods */
TETS2_Telemetry::TETS2_Telemetry()
{
	time = !(TEL_DATA_EMPTY_TIMESTAMP);
}

bool TETS2_Telemetry::load(void* source)
{
	bool changed;
	return load(source, changed);
}

bool TETS2_Telemetry::load(void* source, bool &changed)
{
	if (source == NULL)
		return false;

	bool result = false;
	changed = false;
	try
	{
		unsigned int newTime = getTimestamp(source);
		if (time != newTime)
		{
			const size_t flagCount = 4;
			byte flags[flagCount];
			for (size_t i = 0; i < flagCount; i++)
				flags[i] = getByte(source, 20 + i);

			// game
			time = newTime;
			paused = getUInt(source, 4) > 0;
			gameVersionMajor = getUInt(source, 12);
			gameVersionMinor = getUInt(source, 16);

			// time
			int absoluteTime_ = getInt(source, 160);
			if (absoluteTime_ < 0)
				absoluteTime = 0;
			else
				absoluteTime = absoluteTime_;
			absoluteTime_ = absoluteTime;
			week = absoluteTime_ / MINUTES_IN_WEEK;
			absoluteTime_ -= week * MINUTES_IN_WEEK;
			day = absoluteTime_ / MINUTES_IN_DAY;
			absoluteTime_ -= day * MINUTES_IN_DAY;
			hour = absoluteTime_ / MINUTES_IN_HOUR;
			absoluteTime_ -= hour * MINUTES_IN_HOUR;
			minute = absoluteTime_;
			week++;
			day++;

			// truck
			getString(source, truckManufacturer, 676);
			getString(source, truckModel, 804);
			odometer = (unsigned int)round(getFloat(source, 668));
			engineWear = getFloat(source, 644);
			transmissionWear = getFloat(source, 648);
			cabinWear = getFloat(source, 652);
			chassisWear = getFloat(source, 656);
			wheelWear = getFloat(source, 660);
			truckWear = (engineWear + transmissionWear + cabinWear + chassisWear + wheelWear) / 5.0f;

			// engine
			engineRunning = getAux(source, 580, EngineEnabled);
			engineRPM = (int)round(getFloat(source, 80));
			engineRPMMax = (int)round(getFloat(source, 84));

			// brakes
			retarderBrake = (unsigned int)getInt(source, 564);
			parkingBrake = getAux(source, 580, ParkingBrake);
			engineBrake = getAux(source, 580, EngineBrake);

			// fuel
			fuel = (unsigned int)getFloat(source, 88);
			fuelCapacity = (unsigned int)getFloat(source, 92);
			fuelRange = (unsigned int)getFloat(source, 880);

			// transmission
			gear = getInt(source, 64);
			gearDashboard = getInt(source, 1016);

			// miscellaneous
			electricEnabled = getAux(source, 580, ElectricEnabled);
			wipers = getAux(source, 580, Wipers);
			airPressure = (unsigned int)getFloat(source, 604);

			// lights
			blinkerLeft = getAux(source, 580, BlinkerLeftOn);
			blinkerRight = getAux(source, 580, BlinkerRightOn);
			lightsParking = getAux(source, 580, LightsParking);
			lightsBeamLow = getAux(source, 580, LightsBeamLow);
			lightsBeamHigh = lightsBeamLow ? getAux(source, 580, LightsBeamHigh) : false;
			lightsBeacon = getAux(source, 580, LightsBeacon) && flags[1] > 0;

			// warnings
			fuelWarning = (fuel <= (fuelCapacity * 0.2f)) && engineRunning;
			batteryVoltageWarning = getAux(source, 580, BatteryVoltageWarning) && engineRunning;
			airPressureWarning = getAux(source, 580, AirPressureWarning) && engineRunning;
			airPressureEmergency = getAux(source, 580, AirPressureEmergency) && engineRunning;
			adBlueWarning = getAux(source, 580, AdBlueWarning) && engineRunning;
			oilPressureWarning = getAux(source, 580, OilPressureWarning) && engineRunning;
			waterTemperatureWarning = getAux(source, 580, WaterTemperatureWarning) && engineRunning;

			// temperature
			brakeTemperature = getFloat(source, 608);
			oilTemperature = getFloat(source, 628);
			waterTemperature = getFloat(source, 632);

			// trailer
			getString(source, cargo, 236);
			trailerMass = (unsigned int)getFloat(source, 168);
			trailerAttached = flags[1] > 0;
			trailerWear = getFloat(source, 664);

			// job
			onJob = getByte(source, 1020) != 0;
			jobFinished = getByte(source, 1021) != 0;
			jobIncome = getInt(source, 300);
			jobDeadline = getInt(source, 304);
			getString(source, jobCitySource, 308);
			getString(source, jobCityDestination, 372);
			getString(source, jobCompanySource, 436);
			getString(source, jobCompanyDestination, 500);

			// navigation
			float speed = getFloat(source, 24); // m/s
			speedKmh = (int)round(speed * 3.6f);
			speedMph = (int)round(speed * 3.6f / 1.6f);
			float speedLimit = getFloat(source, 868); // m/s
			speedLimitKmh = (unsigned int)round(speedLimit * 3.6f);
			speedLimitMph = (unsigned int)round(speedLimit * 3.6f / 1.6f);
			routeDistance = (unsigned int)round(getFloat(source, 872));
			routeTime = (unsigned int)round(getFloat(source, 876) / 60.0f);

			// cruise control
			cruiseControl = getAux(source, 580, CruiseControl);
			float cruiseControlSpeed = getFloat(source, 672); // m/s
			cruiseControlSpeedKmh = (int)round(cruiseControlSpeed * 3.6f);
			cruiseControlSpeedMph = (int)round(cruiseControlSpeed * 3.6f / 1.6f);

			changed = true;
		}

		result = true;
	}
	catch (...)
	{

	}
	return result;
}
