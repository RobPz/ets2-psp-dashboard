#include "main.h"

/* module information */
const char* moduleName = "ETS2 Dashboard";
const int majorVersion = 1;
const int minorVersion = 0;
PSP_MODULE_INFO(moduleName, PSP_MODULE_USER, majorVersion, minorVersion);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(12 * 1024);

/* constants */
// general
const char* CLIENT_NAME = "PSP";
// file constants (general)
const char* FILE_SERVER_ADDRESS = "resources/server address.txt";
const char* FILE_GEAR_MAP = "resources/gear map.txt";
// file constants (textures)
const char* FILE_BACKGROUND = "resources/background.png";
const char* FILE_PAUSE = "resources/pause.png";
const char* FILE_HOR_NUMB_BAR = "resources/horizontal number bar.png";
const char* FILE_VER_NUMB_BAR = "resources/vertical number bar.png";
const char* FILE_SPEED_CIRCLE = "resources/speed circle.png";
const char* FILE_CRUISE_CONTROL = "resources/cruise control.png";
const char* FILE_TACHOMETER = "resources/tachometer.png";
const char* FILE_TACHOMETER_NEEDLE = "resources/tachometer needle.png";
const char* FILE_LEFT_BLINKER_LIGHT = "resources/left blinker.png";
const char* FILE_RIGHT_BLINKER_LIGHT = "resources/right blinker.png";
const char* FILE_LOW_BEAM_LIGHT = "resources/low beam.png";
const char* FILE_HIGH_BEAM_LIGHT = "resources/high beam.png";
const char* FILE_LOW_FUEL_LIGHT = "resources/low fuel.png";
const char* FILE_RETARDER_LIGHT = "resources/retarder.png";
const char* FILE_PARKING_BRAKE_LIGHT = "resources/parking brake.png";
const char* FILE_BATTERY_LOW_LIGHT = "resources/battery low.png";
const char* FILE_AIR_WARNING_LIGHT = "resources/air pressure warning.png";
const char* FILE_AIR_EMERGENCY_LIGHT = "resources/air pressure emergency.png";
const char* FILE_BEACON_LIGHT = "resources/beacon.png";
const char* FILE_SERVICE_LIGHT = "resources/service required.png";
const char* FILE_WATER_TEMPERATURE_LIGHT = "resources/water temperature warning.png";
const char* FILE_OIL_PRESSURE_LIGHT = "resources/oil pressure.png";
const char* FILE_FUEL_BAR = "resources/fuel bar.png";
const char* FILE_TRAILER = "resources/semi trailer.png";
const char* FILE_NAVIGATION = "resources/navigation.png";
const char* FILE_WATER_ICON = "resources/water temperature.png";
const char* FILE_OIL_ICON = "resources/oil temperature.png";
const char* FILE_FUEL_ICON = "resources/fuel.png";
const char* FILE_AIR_ICON = "resources/air pressure.png";
// file constants (fonts)
const char* FILE_FONT_DEFAULT = "flash0:/font/ltn0.pgf"; 
const char* FILE_FONT_LIGHT = "flash0:/font/ltn8.pgf";
const char* FILE_FONT_BOLD = "flash0:/font/ltn12.pgf";
// other
#define MINUTES_IN_HOUR 60
#define MINUTES_IN_DAY (24 * MINUTES_IN_HOUR)

void getTestTelemetryData(TETS2_Telemetry &data)
{
	// game
	data.time = 1;
	data.paused = true;
	data.gameVersionMajor = 1;
	data.gameVersionMinor = 31;
	// time
	data.absoluteTime = 882204;
	data.week = 87;
	data.day = 4;
	data.hour = 15;
	data.minute = 25;
	// truck
	strcpy(data.truckManufacturer, "Scania");
	strcpy(data.truckModel, "R 2012");
	data.odometer = 80652; // km
	data.engineWear = 0.07f;
	data.transmissionWear = 0.11f;
	data.cabinWear = 0.06f;
	data.chassisWear = 0.29f;
	data.wheelWear = 0.25f;
	data.truckWear = (data.engineWear + data.transmissionWear + data.cabinWear + data.chassisWear + data.wheelWear) / 5.0f;
	// engine
	data.engineRunning = true;
	data.engineRPM = 1150;
	data.engineRPMMax = 2500;
	// brakes
	data.retarderBrake = true;
	data.parkingBrake = true;
	data.engineBrake = true;
	// fuel
	data.fuel = 756; // litres
	data.fuelCapacity = 1400; // litres
	data.fuelRange = 2354; // km
	// transmission
	data.gear = 13;
	data.gearDashboard = 13;
	// miscellaneous
	data.electricEnabled = true;
	data.wipers = true;
	data.airPressure = 116; // psi
	// lights
	data.blinkerLeft = true;
	data.blinkerRight = true;
	data.lightsParking = true;
	data.lightsBeamLow = true;
	data.lightsBeamHigh = true;
	data.lightsBeacon = true;
	// warnings
	data.fuelWarning = true;
	data.batteryVoltageWarning = true;
	data.airPressureWarning = true;
	data.airPressureEmergency = true;
	data.adBlueWarning = true;
	data.oilPressureWarning = true;
	data.waterTemperatureWarning = true;
	// temperature
	data.brakeTemperature = 70.0f;
	data.oilTemperature = 90.0f;
	data.waterTemperature = 85.0f;
	// trailer
	data.trailerAttached = true;
	strcpy(data.cargo, "Test Cargo");
	data.trailerMass = 17562; // kg
	data.trailerWear = 0.15f;
	// job info
	data.onJob = true;
	data.jobFinished = true;
	data.jobIncome = 900;
	data.jobDeadline = data.absoluteTime + 279;
	strcpy(data.jobCitySource, "Hamburg");
	strcpy(data.jobCityDestination, "Dortmund");
	strcpy(data.jobCompanySource, "Tradeaux");
	strcpy(data.jobCompanyDestination, "Posped");
	// navigation
	data.speedKmh = 75;
	data.speedMph = (int)round(data.speedKmh / 1.6f);
	data.speedLimitKmh = 90;
	data.speedLimitMph = (int)round(data.speedLimitKmh / 1.6f);
	data.routeDistance = 224648; // meters
	data.routeTime = 168; // minutes
	// cruise control
	data.cruiseControl = true;
	data.cruiseControlSpeedKmh = 85;
	data.cruiseControlSpeedMph = (int)round(data.cruiseControlSpeedKmh / 1.6f);
}

int main()
{
	// OSL initialization
	initOSLib();
	oslIntraFontInit(INTRAFONT_CACHE_MED | INTRAFONT_STRING_UTF8);

	// enable printf (can't do it earlier)
	pspDebugScreenInit();

	// welcome screen
	printf("Euro Truck Simulator 2 Virtual Dashboard\n");
	printf("\n");

	// our own initialization
	resetHandles();
	getGearMap();
	getServerAddress();
	loadTextures();
	loadFonts();
	loadDashboard();
	initNetworking();

	// connect
#ifndef NO_SERVER
	connectToAccessPoint();
	connect();
#endif 

	// main loop
	int frameSkip = 0;
	unsigned int framesSinceLastUpdate = 0;
	TETS2_Telemetry telData;
	while (!osl_quit)
	{
		// check if the telemetry thread is running
		bool connected = false;
		SceKernelThreadRunStatus status;
		status.size = sizeof(SceKernelThreadRunStatus);
		int result = sceKernelReferThreadRunStatus(telemetryThreadID, &status);
		if (result != 0 || status.status & PSP_THREAD_STOPPED || status.status & PSP_THREAD_KILLED)
		{
			disconnect();
			disconnectFromAccessPoint();
			connected = false;
		}
		else
			connected = true;

		// telemetry data
		bool telemetryPresent = false;
		bool telemetryChanged = false;
		if (connected)
		{
			SceUInt timeout = 10 * 1000;
			result = sceKernelWaitSema(telDataSemaphore, 1, &timeout);
			if (result != 0)
				terminate(false); // temp
			telemetryPresent = telData.load(telDataBlock, telemetryChanged);
			result = sceKernelSignalSema(telDataSemaphore, 1);
			if (result != 0)
				terminate(false); // temp
		}
		else // bypass the semaphore if the telemetry thread is not running
			telemetryPresent = telData.load(telDataBlock, telemetryChanged);
		if ((telemetryPresent && telData.time == TEL_DATA_EMPTY_TIMESTAMP) || !connected)
			telemetryPresent = false;
#ifdef NO_SERVER
		getTestTelemetryData(telData);
		// pretend everything's OK
		connected = true;
		telemetryPresent = true;
		framesSinceLastUpdate = 0;
#endif
		if (telemetryChanged)
			framesSinceLastUpdate = 0;
		else
			framesSinceLastUpdate++;
		if (framesSinceLastUpdate > 500)
			telemetryPresent = false;

		// rendering
		if (!frameSkip)
		{
			oslStartDrawing();

			oslClearScreen(RGBA(0, 0, 0, 255));
			oslDrawImage(textures[TEXTURE_BACKGROUND]);

			renderDashboard(telData, connected, telemetryPresent, telemetryChanged);

			oslEndDrawing();
		}

		// v-sync
		oslEndFrame();
		frameSkip = oslSyncFrame();

		// user input
		oslReadKeys();
		if (osl_keys->released.start) // exit
			break;
		if (osl_keys->released.L || osl_keys->released.left || osl_keys->released.square)
			selectedTab--;
		if (osl_keys->released.R || osl_keys->released.right || osl_keys->released.circle)
			selectedTab++;
		if (selectedTab < 0)
			selectedTab = 2;
		if (selectedTab > 2)
			selectedTab = 0;
	}

	// bye bye
	terminate(false);
	return 0;
}

void sleepS(unsigned int seconds)
{
	sceKernelDelayThread(seconds * 1000 * 1000);
}

void sleepMS(unsigned int milliseconds)
{
	sceKernelDelayThread(milliseconds * 1000);
}

void connect()
{
	/* create a semaphore to protect telemetry data */
	SceUID semaphore = sceKernelCreateSema("telDataSemaphore", 0, 1, 1, 0);
	if (semaphore < 0)
	{
		printf("Error, could not create a semaphore for telemetry data\n");
		terminate(true);
	}
	else
		telDataSemaphore = semaphore;

	/* establish a connection */
	bool disconnected = true;
	while (disconnected)
	{
		printf("\n");
		printf("Connecting to server on %s:%d...\n", serverIP, serverPort);

		sockaddr_in remote;
		remote.sin_family = AF_INET;
		inet_pton(AF_INET, serverIP, &remote.sin_addr);
		remote.sin_port = htons((u_short)serverPort);

		connection = socket(remote.sin_family, SOCK_STREAM, IPPROTO_TCP);
		if (connection < 0)
		{
			int error = errno;
			printf("Unable to create a socket endpoint\n");
			printf(" socket() returned: 0x%08X\n", error);
			terminate(true);
		}

		int result = connect(connection, (sockaddr*)&remote, sizeof(remote));
		if (result < 0)
		{
			int error = errno;
			close(connection);
			connection = -1;

			if (error == ECONNREFUSED || error == ETIMEDOUT) // this is fine, we can try again
			{
				char reason[32];
				switch (error)
				{
				case ECONNREFUSED: strcpy(reason, "host unavailable"); break;
				case ETIMEDOUT: strcpy(reason, "connection timeout"); break;
				default: strcpy(reason, "unknown reason"); break;
				}
				printf("Unable to connect: %s\n", reason);
				printf("Press CROSS to try again or press CIRCLE to quit\n");
				SceCtrlLatch latch;
				bool tryAgain = false;
				while (true)
				{
					sceDisplayWaitVblankStart();
					sceCtrlReadLatch(&latch);
					if (latch.uiBreak & PSP_CTRL_CROSS)
					{
						tryAgain = true;
						break;
					}
					if (latch.uiBreak & PSP_CTRL_CIRCLE)
					{
						terminate(false);
					}
				}
				if (tryAgain)
					continue;
			}
			else // bad state, can't recover
			{
				printf("Unable to connect\n");
				printf(" connect() returned: 0x%08X\n", error);
				terminate(true);
			}
		}

		printf("Connected to server\n");
		disconnected = false;
	}

	/* zero out the telemetry data block */
	memset(telDataBlock, 0, TEL_DATA_BLOCK_SIZE);

	/* create and launch a thread for receiving data from the server */
	SceUID threadID = sceKernelCreateThread("telemetryThread", telemetryThread, 0x18, 0x10000, PSP_THREAD_ATTR_USER, NULL);
	if (threadID < 0)
	{
		printf("Error, could not create a network thread\n");
		terminate(true);
	}
	else
		telemetryThreadID = threadID;
	if (sceKernelStartThread(telemetryThreadID, 0, NULL) != 0)
	{
		printf("Error, could not start the network thread\n");
		terminate(true);
	}
}

void disconnect()
{
	if (connection >= 0)
	{
		shutdown(connection, SHUT_RDWR);
		close(connection);
		connection = -1;
	}
	if (telemetryThreadID >= 0)
	{
		waitForTelemetryThread();
		sceKernelDeleteThread(telemetryThreadID);
		telemetryThreadID = -1;
	}
	if (telDataSemaphore >= 0)
	{
		sceKernelDeleteSema(telDataSemaphore);
		telDataSemaphore = -1;
	}
}

void waitForTelemetryThread()
{
	if (telemetryThreadID < 0)
		return;
	SceUInt timeout = 1000 * 1000; // 1 second
	sceKernelWaitThreadEnd(telemetryThreadID, &timeout);
}

void setName()
{
	uint16_t nameLength = (uint16_t)strlen(CLIENT_NAME) + 1;
	TPacket packet;
	packet.packetType = PACKET_NAME;
	packet.payloadSize = nameLength;
	send(connection, (char*)&packet, PACKET_SIZE, 0);
	send(connection, CLIENT_NAME, (int)nameLength, 0);
}

bool isConnectedToAP()
{
	int state, error;
	error = sceNetApctlGetState(&state);
	return error == 0 && state == PSP_NET_APCTL_STATE_GOT_IP;
}

void renderDashboard(TETS2_Telemetry &data, bool connected, bool telemetryPresent, bool telemetryChanged)
{
	// variables
	char text[128]; // text buffer
	float tSize; // font size
	unsigned int tWidth, tHeight; // text dimensions
	int x, y; // temporary variables for positioning
	unsigned int width, height; // element and combined element dimensions
	// constants
	const unsigned int topSectionSize = 28;
	const unsigned int topSectionCenter = topSectionSize / 2;
	const unsigned int topSectionIconOffset = 14;
	const unsigned int topSectionIconSpacing = 27;
	const unsigned int bottomSectionSize = 100;

	// speedometer
	speedometer->setTop(topSectionSize);
	speedometer->setLeft(SCREEN_WIDTH / 2 - speedometer->getWidth() / 2
		- speedometer->getSingleDigitWidth() / 2 - speedometer->getDigitPadding() / 2);
	speedometer->setValue(telemetryPresent && connected ? abs(data.speedKmh) : -11);
	speedometer->render();

	// speedometer units
	strcpy(text, "km/h");
	tSize = 0.6f;
	oslSetFont(fonts[FONT_DEFAULT]);
	oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(180, 180, 180, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
	x = SCREEN_WIDTH / 2;
	y = speedometer->getTop() + speedometer->getHeight() + 10;
	tHeight = (int)round(osl_curFont->charHeight * tSize);
	oslDrawString(x, y, text);

	// odometer
	sprintf(text, "%d km", data.odometer);
	tSize = 0.7f;
	oslSetFont(fonts[FONT_DEFAULT]);
	oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(240, 240, 240, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
	tWidth = oslGetStringWidth(text);
	x = SCREEN_WIDTH / 2;
	y += tHeight + 12; // below the speedometer
	tHeight = (int)round(osl_curFont->charHeight * tSize);
	oslDrawString(x, y, text);

	// tachometer
	oslSetImageRotCenter(textures[TEXTURE_TACHOMETER]);
	textures[TEXTURE_TACHOMETER]->x = 385;
	textures[TEXTURE_TACHOMETER]->x = SCREEN_WIDTH - textures[TEXTURE_TACHOMETER]->sizeX / 2 - 25;
	textures[TEXTURE_TACHOMETER]->y = topSectionSize + textures[TEXTURE_TACHOMETER]->sizeY / 2;
	oslDrawImage(textures[TEXTURE_TACHOMETER]);

	// tachometer needle
	oslSetImageRotCenter(textures[TEXTURE_TACHOMETER_NEEDLE]);
	textures[TEXTURE_TACHOMETER_NEEDLE]->x = textures[TEXTURE_TACHOMETER]->x;
	textures[TEXTURE_TACHOMETER_NEEDLE]->y = textures[TEXTURE_TACHOMETER]->y
		+ (textures[TEXTURE_TACHOMETER_NEEDLE]->sizeY - textures[TEXTURE_TACHOMETER]->sizeY) / 2;
	textures[TEXTURE_TACHOMETER_NEEDLE]->angle = (int)round((data.engineRPM > 3000 ? 3000.0f : data.engineRPM) / 3000.0f * 270.0f) - 135;
	oslDrawImage(textures[TEXTURE_TACHOMETER_NEEDLE]);

	// speed limit
	if (data.speedLimitKmh > 0)
	{
		x = 158;
		y = topSectionSize + textures[TEXTURE_SPEED_CIRCLE]->sizeY / 2 + 13;

		// speed circle
		oslSetImageRotCenter(textures[TEXTURE_SPEED_CIRCLE]);
		textures[TEXTURE_SPEED_CIRCLE]->x = x;
		textures[TEXTURE_SPEED_CIRCLE]->y = y;
		oslDrawImage(textures[TEXTURE_SPEED_CIRCLE]);

		// speed limit value
		sprintf(text, "%u", data.speedLimitKmh);
		tSize = data.speedLimitKmh >= 100 ? 0.9f : 1.0f; // just to make it fit better inside the circle
		oslSetFont(fonts[FONT_DEFAULT]);
		oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
		tHeight = (int)round(osl_curFont->charHeight * tSize);
		y -= tHeight / 2;
		oslDrawString(x, y + 5, text); // y fix (I honestly don't know what's wrong, IntraFont is acting up)
	}

	// cruise control
	if (data.cruiseControl)
	{
		const unsigned int spacing = 5; // spacing between the text and the icon
		int y2;

		sprintf(text, "%u", data.cruiseControlSpeedKmh);
		tSize = 1.0f;
		oslSetFont(fonts[FONT_DEFAULT]);
		oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_LEFT);
		tWidth = oslGetStringWidth(text);
		tHeight = (int)round(osl_curFont->charHeight * tSize);

		width = tWidth + spacing + textures[TEXTURE_CRUISE_CONTROL]->sizeX;
		height = tHeight;
		x = 158 - width / 2;
		y = topSectionSize + textures[TEXTURE_SPEED_CIRCLE]->sizeY + 30 + height;

		// cruise speed value
		y2 = y - tHeight;
		oslDrawString(x, y2 + 5, text); // again, +5 pixels to compensate for the random text offset

		// cruise control icon
		x += tWidth + spacing;
		y2 = y - textures[TEXTURE_CRUISE_CONTROL]->sizeY;
		y2 -= 4; // I still don't like how it lines up so I shift it up a bit
		textures[TEXTURE_CRUISE_CONTROL]->x = x;
		textures[TEXTURE_CRUISE_CONTROL]->y = y2;
		oslDrawImage(textures[TEXTURE_CRUISE_CONTROL]);
	}

	// clock
	tSize = 1.5f;
	oslSetFont(fonts[FONT_DEFAULT]);
	oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
	sprintf(text, "%02d:%02d", data.hour, data.minute);
	tWidth = oslGetStringWidth(text);
	tHeight = (int)round(osl_curFont->charHeight * tSize);
	x = 20 + tWidth / 2;
	y = topSectionSize + 11;
	oslDrawString(x, y + tHeight / 2, text); // this time the offset is half the size of the height for some reason

	// week day
	switch (data.day)
	{
	case 1: strcpy(text, "Monday"); break;
	case 2: strcpy(text, "Tuesday"); break;
	case 3: strcpy(text, "Wednesday"); break;
	case 4: strcpy(text, "Thursday"); break;
	case 5: strcpy(text, "Friday"); break;
	case 6: strcpy(text, "Saturday"); break;
	case 7: strcpy(text, "Sunday"); break;
	default: strcpy(text, "-------");
	}
	oslSetFont(fonts[FONT_DEFAULT]);
	tSize = 0.7f;
	oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(180, 180, 180, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
	y += tHeight + 5; // below the clock
	oslDrawString(x, y, text);

	// fuel bar
	width = textures[TEXTURE_FUEL_BAR]->sizeX;
	height = textures[TEXTURE_FUEL_BAR]->sizeY;
	y += 22 + height / 2;
	oslSetImageRotCenter(textures[TEXTURE_FUEL_BAR]);
	textures[TEXTURE_FUEL_BAR]->x = x;
	textures[TEXTURE_FUEL_BAR]->y = y;
	oslDrawImage(textures[TEXTURE_FUEL_BAR]);

	// fuel amount
    if (data.fuel > 0 && data.fuelCapacity > 0)
    {
        x = x - width / 2 + 3;
	    	oslDrawFillRect(x, y + 1,
		x + (int)round(((float)data.fuel / (float)data.fuelCapacity) * (width - 6)), y + height / 2 - 3, RGBA(255, 255, 255, 255));
    }

	////////////////////////////////////////////////////////////////////////////
	// indicator lights
	////////////////////////////////////////////////////////////////////////////

	// blinkers
	if (data.blinkerLeft)
	{
		oslSetImageRotCenter(textures[TEXTURE_LEFT_BLINKER_LIGHT]);
		textures[TEXTURE_LEFT_BLINKER_LIGHT]->x = (topSectionIconOffset + 0 * topSectionIconSpacing);
		textures[TEXTURE_LEFT_BLINKER_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_LEFT_BLINKER_LIGHT]);
	}
	if (data.blinkerRight)
	{
		oslSetImageRotCenter(textures[TEXTURE_RIGHT_BLINKER_LIGHT]);
		textures[TEXTURE_RIGHT_BLINKER_LIGHT]->x = SCREEN_WIDTH - (topSectionIconOffset + 0 * topSectionIconSpacing);
		textures[TEXTURE_RIGHT_BLINKER_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_RIGHT_BLINKER_LIGHT]);
	}
	
	// gear
	tSize = 1.0f;
	oslSetFont(fonts[FONT_DEFAULT]);
	oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
	if (data.gearDashboard > 0)
	{
		sprintf(text, "%s", gears[data.gearDashboard - 1]);
	}
	else
	{
		if (data.gearDashboard == 0)
			sprintf(text, "N");
		else
			sprintf(text, "R%d", abs(data.gearDashboard));
	}
	if (!telemetryPresent)
		sprintf(text, "NO DATA");
	if (!connected)
		sprintf(text, "OFFLINE");
	tHeight = (int)round(osl_curFont->charHeight * tSize);
	x = SCREEN_WIDTH / 2;
	y = topSectionCenter - tHeight / 2;
	oslDrawString(x, y + 5, text); // y fix

	// low beam lights
	if (data.lightsBeamLow)
	{
		oslSetImageRotCenter(textures[TEXTURE_LOW_BEAM_LIGHT]);
		textures[TEXTURE_LOW_BEAM_LIGHT]->x = topSectionIconOffset + 1 * topSectionIconSpacing;
		textures[TEXTURE_LOW_BEAM_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_LOW_BEAM_LIGHT]);
	}

	// high beam lights
	if (data.lightsBeamHigh)
	{
		oslSetImageRotCenter(textures[TEXTURE_HIGH_BEAM_LIGHT]);
		textures[TEXTURE_HIGH_BEAM_LIGHT]->x = topSectionIconOffset + 2 * topSectionIconSpacing;
		textures[TEXTURE_HIGH_BEAM_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_HIGH_BEAM_LIGHT]);
	}

	// beacon light
	if (data.lightsBeacon)
	{
		oslSetImageRotCenter(textures[TEXTURE_BEACON_LIGHT]);
		textures[TEXTURE_BEACON_LIGHT]->x = topSectionIconOffset + 3 * topSectionIconSpacing;
		textures[TEXTURE_BEACON_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_BEACON_LIGHT]);
	}

	// parking brake
	if (data.parkingBrake)
	{
		oslSetImageRotCenter(textures[TEXTURE_PARKING_BRAKE_LIGHT]);
		textures[TEXTURE_PARKING_BRAKE_LIGHT]->x = topSectionIconOffset + 4 * topSectionIconSpacing;
		textures[TEXTURE_PARKING_BRAKE_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_PARKING_BRAKE_LIGHT]);
	}

	// retarder
	if (data.retarderBrake)
	{
		oslSetImageRotCenter(textures[TEXTURE_RETARDER_LIGHT]);
		textures[TEXTURE_RETARDER_LIGHT]->x = topSectionIconOffset + 5 * topSectionIconSpacing;
		textures[TEXTURE_RETARDER_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_RETARDER_LIGHT]);
	}

	// pause indicator
	if (data.paused)
	{
		oslSetImageRotCenter(textures[TEXTURE_PAUSE]);
		textures[TEXTURE_PAUSE]->x = topSectionIconOffset + 6 * topSectionIconSpacing;
		textures[TEXTURE_PAUSE]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_PAUSE]);
	}

	// low fuel warning
	if (data.fuelWarning && data.fuelCapacity > 0)
	{
		oslSetImageRotCenter(textures[TEXTURE_LOW_FUEL_LIGHT]);
		textures[TEXTURE_LOW_FUEL_LIGHT]->x = SCREEN_WIDTH - (topSectionIconOffset + 1 * topSectionIconSpacing);
		textures[TEXTURE_LOW_FUEL_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_LOW_FUEL_LIGHT]);
	}

	// battery warning
	if (data.batteryVoltageWarning)
	{
		oslSetImageRotCenter(textures[TEXTURE_BATTERY_LOW_LIGHT]);
		textures[TEXTURE_BATTERY_LOW_LIGHT]->x = SCREEN_WIDTH - (topSectionIconOffset + 2 * topSectionIconSpacing);
		textures[TEXTURE_BATTERY_LOW_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_BATTERY_LOW_LIGHT]);
	}

	// air pressure warning
	if (data.airPressureWarning && !data.airPressureEmergency)
	{
		oslSetImageRotCenter(textures[TEXTURE_AIR_WARNING_LIGHT]);
		textures[TEXTURE_AIR_WARNING_LIGHT]->x = SCREEN_WIDTH - (topSectionIconOffset + 3 * topSectionIconSpacing);
		textures[TEXTURE_AIR_WARNING_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_AIR_WARNING_LIGHT]);
	}

	// air pressure critical
	if (data.airPressureEmergency)
	{
		oslSetImageRotCenter(textures[TEXTURE_AIR_EMERGENCY_LIGHT]);
		textures[TEXTURE_AIR_EMERGENCY_LIGHT]->x = SCREEN_WIDTH - (topSectionIconOffset + 3 * topSectionIconSpacing);
		textures[TEXTURE_AIR_EMERGENCY_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_AIR_EMERGENCY_LIGHT]);
	}

	// service needed
	if (data.engineWear >= 0.1f || data.transmissionWear >= 0.1f || data.chassisWear >= 0.1f || data.truckWear >= 0.1f)
	{
		oslSetImageRotCenter(textures[TEXTURE_SERVICE_LIGHT]);
		textures[TEXTURE_SERVICE_LIGHT]->x = SCREEN_WIDTH - (topSectionIconOffset + 4 * topSectionIconSpacing);
		textures[TEXTURE_SERVICE_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_SERVICE_LIGHT]);
	}

	// water temperature
	if (data.waterTemperatureWarning)
	{
		oslSetImageRotCenter(textures[TEXTURE_WATER_TEMPERATURE_LIGHT]);
		textures[TEXTURE_WATER_TEMPERATURE_LIGHT]->x = SCREEN_WIDTH - (topSectionIconOffset + 5 * topSectionIconSpacing);
		textures[TEXTURE_WATER_TEMPERATURE_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_WATER_TEMPERATURE_LIGHT]);
	}

	// oil pressure
	if (data.oilPressureWarning)
	{
		oslSetImageRotCenter(textures[TEXTURE_OIL_PRESSURE_LIGHT]);
		textures[TEXTURE_OIL_PRESSURE_LIGHT]->x = SCREEN_WIDTH - (topSectionIconOffset + 6 * topSectionIconSpacing);
		textures[TEXTURE_OIL_PRESSURE_LIGHT]->y = topSectionCenter;
		oslDrawImage(textures[TEXTURE_OIL_PRESSURE_LIGHT]);
	}

	////////////////////////////////////////////////////////////////////////////
	// bottom section
	////////////////////////////////////////////////////////////////////////////

	// divider
	//oslDrawLine(0, SCREEN_HEIGHT - bottomSectionSize, SCREEN_WIDTH, SCREEN_HEIGHT - bottomSectionSize, RGBA(255, 255, 255, 255));

	// tab headers
	{
		unsigned int screenSection = SCREEN_WIDTH / 3;
		unsigned int selectedShadowColor = RGBA(255, 255, 255, 100);

		tSize = 1.0f;
		oslSetFont(fonts[FONT_DEFAULT]);
		tHeight = (int)round(osl_curFont->charHeight * tSize);
		y = SCREEN_HEIGHT - bottomSectionSize - tHeight - 2;

		// general tab header
		oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(255, 255, 255, 255),
			selectedTab == 0 ? selectedShadowColor : RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
		sprintf(text, "General");
		tWidth = oslGetStringWidth(text);	
		x = screenSection / 2;
		oslDrawString(x, y + 5, text); // y fix
		if (selectedTab == 0)
			oslDrawRect(x - screenSection / 2, y - 2, x + screenSection / 2, y + tHeight + 3, RGBA(255, 255, 255, 255));
		else
			oslDrawLine(x - screenSection / 2, y + tHeight + 2, x + screenSection / 2, y + tHeight + 2, RGBA(255, 255, 255, 255));

		// truck tab header
		oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(255, 255, 255, 255),
			selectedTab == 1 ? selectedShadowColor : RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
		sprintf(text, "Truck");
		tWidth = oslGetStringWidth(text);
		x = SCREEN_WIDTH / 2;
		oslDrawString(x, y + 5, text); // y fix
		if (selectedTab == 1)
			oslDrawRect(x - screenSection / 2, y - 2, x + screenSection / 2, y + tHeight + 3, RGBA(255, 255, 255, 255));
		else
			oslDrawLine(x - screenSection / 2, y + tHeight + 2, x + screenSection / 2, y + tHeight + 2, RGBA(255, 255, 255, 255));

		// trailer tab header
		oslIntraFontSetStyle(fonts[FONT_DEFAULT], tSize, RGBA(255, 255, 255, 255),
			selectedTab == 2 ? selectedShadowColor : RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
		sprintf(text, "Trailer");
		tWidth = oslGetStringWidth(text);
		x = SCREEN_WIDTH - screenSection / 2;
		oslDrawString(x, y + 5, text); // y fix
		if (selectedTab == 2)
			oslDrawRect(x - screenSection / 2, y - 2, x + screenSection / 2, y + tHeight + 3, RGBA(255, 255, 255, 255));
		else
			oslDrawLine(x - screenSection / 2, y + tHeight + 2, x + screenSection / 2, y + tHeight + 2, RGBA(255, 255, 255, 255));
	}
	
	y = SCREEN_HEIGHT - bottomSectionSize;

	// general tab
	if (selectedTab == 0)
	{
		bool displayRouteInfo = data.routeDistance > 0;
		unsigned int sideOffset = SCREEN_WIDTH / 13;

		tSize = 1.0f;
		oslSetFont(fonts[FONT_LIGHT]);
		tHeight = (int)round(osl_curFont->charHeight * tSize);
		if (displayRouteInfo)
			x = SCREEN_WIDTH / 2 + SCREEN_WIDTH / 4 - sideOffset;
		else
			x = SCREEN_WIDTH / 2;
		y = SCREEN_HEIGHT - bottomSectionSize;

		// truck manufacturer
		oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
		sprintf(text, data.truckManufacturer);
		y = SCREEN_HEIGHT - bottomSectionSize + bottomSectionSize / 2 - bottomSectionSize / 5 - tHeight / 2;
		oslDrawString(x, y, text);

		// truck model
		oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
		sprintf(text, data.truckModel);
		y = SCREEN_HEIGHT - bottomSectionSize + bottomSectionSize / 2 + bottomSectionSize / 5 - tHeight / 2;
		oslDrawString(x, y, text);

		if (displayRouteInfo)
		{
			tSize = 1.0f;
			oslSetFont(fonts[FONT_LIGHT]);
			tHeight = (int)round(osl_curFont->charHeight * tSize);
			x = SCREEN_WIDTH / 4 + sideOffset;
			y = SCREEN_HEIGHT - bottomSectionSize;

			// icon
			oslSetImageRotCenter(textures[TEXTURE_NAVIGATION]);
			textures[TEXTURE_NAVIGATION]->x = SCREEN_WIDTH / 6;
			textures[TEXTURE_NAVIGATION]->y = SCREEN_HEIGHT - bottomSectionSize + bottomSectionSize / 2;
			oslDrawImage(textures[TEXTURE_NAVIGATION]);

			// caption
			oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
			sprintf(text, "Route:");
			y += 18;
			oslDrawString(x, y, text);

			// distance left
			oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
			if (data.routeDistance >= 1000)
				sprintf(text, "%u km", data.routeDistance / 1000);
			else
				if (data.routeDistance >= 100)
					sprintf(text, "%.1f km", data.routeDistance * 0.001f);
				else
					sprintf(text, "<0.1 km");
			y += tHeight + 10;
			oslDrawString(x, y, text);

			// route time
			oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
			unsigned int minutes = data.routeTime;
			unsigned char hours = minutes / MINUTES_IN_HOUR;
			minutes -= hours * MINUTES_IN_HOUR;
			char hoursText[8], minutesText[8];
			if (hours == 0)
				hoursText[0] = '\0';
			else
				sprintf(hoursText, "%dh ", hours);
			if (minutes == 0)
				minutesText[0] = '\0';
			else
				sprintf(minutesText, "%d min", minutes);
			sprintf(text, "%s%s", hoursText, minutesText);
			y += tHeight + 10;
			oslDrawString(x, y, text);
		}
	}

	// truck tab
	if (selectedTab == 1)
	{
		tSize = 1.0f;
		oslSetFont(fonts[FONT_LIGHT]);
		tHeight = (int)round(osl_curFont->charHeight * tSize);
		unsigned int x1 = SCREEN_WIDTH / 8;
		unsigned int y1 = SCREEN_HEIGHT - bottomSectionSize + bottomSectionSize / 2 - bottomSectionSize / 5;
		unsigned int x2 = SCREEN_WIDTH / 2 + SCREEN_WIDTH / 10;
		unsigned int y2 = SCREEN_HEIGHT - bottomSectionSize + bottomSectionSize / 2 + bottomSectionSize / 5;
		unsigned int offset = 10; // between the icon and text

		// water temperature
		oslSetImageRotCenter(textures[TEXTURE_WATER_ICON]);
		textures[TEXTURE_WATER_ICON]->x = x1;
		textures[TEXTURE_WATER_ICON]->y = y1;
		oslDrawImage(textures[TEXTURE_WATER_ICON]);
		// value
		oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_LEFT);
		sprintf(text, "%.0f C", data.waterTemperature);
		x = textures[TEXTURE_WATER_ICON]->x + textures[TEXTURE_WATER_ICON]->sizeX / 2 + offset;
		y = textures[TEXTURE_WATER_ICON]->y - tHeight / 2;
		oslDrawString(x, y + 5, text); // y fix

		// oil temperature
		oslSetImageRotCenter(textures[TEXTURE_OIL_ICON]);
		textures[TEXTURE_OIL_ICON]->x = x1;
		textures[TEXTURE_OIL_ICON]->y = y2;
		oslDrawImage(textures[TEXTURE_OIL_ICON]);
		// value
		oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_LEFT);
		sprintf(text, "%.0f C", data.oilTemperature);
		x = textures[TEXTURE_OIL_ICON]->x + textures[TEXTURE_OIL_ICON]->sizeX / 2 + offset;
		y = textures[TEXTURE_OIL_ICON]->y - tHeight / 2;
		oslDrawString(x, y + 5, text); // y fix

		// fuel
		oslSetImageRotCenter(textures[TEXTURE_FUEL_ICON]);
		textures[TEXTURE_FUEL_ICON]->x = x2;
		textures[TEXTURE_FUEL_ICON]->y = y1;
		oslDrawImage(textures[TEXTURE_FUEL_ICON]);
		// value
		oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_LEFT);
		sprintf(text, "%u l | %u km", data.fuel, data.fuelRange);
		x = textures[TEXTURE_FUEL_ICON]->x + textures[TEXTURE_FUEL_ICON]->sizeX / 2 + offset;
		y = textures[TEXTURE_FUEL_ICON]->y - tHeight / 2;
		oslDrawString(x, y + 5, text); // y fix

		// air pressure
		oslSetImageRotCenter(textures[TEXTURE_AIR_ICON]);
		textures[TEXTURE_AIR_ICON]->x = x2;
		textures[TEXTURE_AIR_ICON]->y = y2;
		oslDrawImage(textures[TEXTURE_AIR_ICON]);
		// value
		oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_LEFT);
		sprintf(text, "%u psi / 180 psi", data.airPressure);
		x = textures[TEXTURE_AIR_ICON]->x + textures[TEXTURE_AIR_ICON]->sizeX / 2 + offset;
		y = textures[TEXTURE_AIR_ICON]->y - tHeight / 2;
		oslDrawString(x, y + 5, text); // y fix
	}

	// trailer tab
	if (selectedTab == 2)
	{
		if (!data.onJob && !data.trailerAttached)
		{
			tSize = 1.0f;
			oslSetFont(fonts[FONT_LIGHT]);
			oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
			sprintf(text, "no trailer attached nor jobs active");
			tHeight = (int)round(osl_curFont->charHeight * tSize);
			x = SCREEN_WIDTH / 2;
			y = SCREEN_HEIGHT - bottomSectionSize + bottomSectionSize / 2 - tHeight / 2;
			oslDrawString(x, y, text);
		}
		else
		{
			if (data.onJob)
			{
				tSize = 1.0f;
				oslSetFont(fonts[FONT_LIGHT]);
				tHeight = (int)round(osl_curFont->charHeight * tSize);
				x = 75;
				int x2 = x + 8;
				y = SCREEN_HEIGHT - bottomSectionSize;

				// start
				oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_RIGHT);
				sprintf(text, "From:");
				y += 18;
				oslDrawString(x, y, text);
				// value
				oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_LEFT);
				sprintf(text, "%s (%s)", data.jobCitySource, data.jobCompanySource);
				tWidth = oslGetStringWidth(text);
				oslDrawString(x2, y, text);

				// destination
				oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_RIGHT);
				sprintf(text, "To:");
				y += tHeight + 10;
				oslDrawString(x, y, text);
				// value
				oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_LEFT);
				sprintf(text, "%s (%s)", data.jobCityDestination, data.jobCompanyDestination);
				tWidth = oslGetStringWidth(text);
				oslDrawString(x2, y, text);

				// deadline
				oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_RIGHT);
				sprintf(text, "Deadline:");
				y += tHeight + 10;
				oslDrawString(x, y, text);
				// value
				unsigned int color;
				if (data.jobDeadline > -1)
				{
					int minutes = data.jobDeadline - data.absoluteTime;
					if (minutes > 0)
					{
						if (data.absoluteTime > data.jobDeadline - data.routeTime)
							color = RGBA(230, 126, 34, 255);
						else
							color = RGBA(255, 255, 255, 255);						
						unsigned char days = minutes / MINUTES_IN_DAY;
						minutes -= days * MINUTES_IN_DAY;
						unsigned char hours = minutes / MINUTES_IN_HOUR;
						minutes -= hours * MINUTES_IN_HOUR;
						char daysText[16], hoursText[16], minutesText[16];
						if (days == 0)
							daysText[0] = '\0';
						else
						{
							if (days == 1)
								sprintf(daysText, "1 day ");
							else
								sprintf(daysText, "%d days ", days);
						}
						if (hours == 0)
							hoursText[0] = '\0';
						else
						{
							if (hours == 1)
								sprintf(hoursText, "1 hour ");
							else
								sprintf(hoursText, "%d hours ", hours);
						}
						if (minutes == 0)
							minutesText[0] = '\0';
						else
						{
							if (minutes == 1)
								sprintf(minutesText, "1 minute");
							else
								sprintf(minutesText, "%d minutes", minutes);
						}
						sprintf(text, "%s%s%s", daysText, hoursText, minutesText);
					}
					else
					{
						color = RGBA(234, 32, 39, 255);
						sprintf(text, "overdue");
					}
					oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, color, RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_LEFT);
					tWidth = oslGetStringWidth(text);
					oslDrawString(x2, y, text);
				}
			}

			if (data.trailerAttached)
			{
				// trailer sprite
				x = SCREEN_WIDTH / 9 * 7;// 8 * 6;
				y = SCREEN_HEIGHT - bottomSectionSize + 17 + textures[TEXTURE_TRAILER]->sizeY / 2;
				oslSetImageRotCenter(textures[TEXTURE_TRAILER]);
				textures[TEXTURE_TRAILER]->x = x;
				textures[TEXTURE_TRAILER]->y = y;
				oslDrawImage(textures[TEXTURE_TRAILER]);

				// trailer mass
				if (data.trailerMass > 0)
				{
					tSize = 1.2f;
					oslSetFont(fonts[FONT_LIGHT]);
					oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(0, 0, 0, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
					tHeight = (int)round(osl_curFont->charHeight * tSize);
					y = y - tHeight / 2 - tHeight / 5;
					if (data.trailerMass >= 1000)
						sprintf(text, "%dt", data.trailerMass / 1000);
					else
						sprintf(text, "%dkg", data.trailerMass);
					oslDrawString(x, y, text);
				}

				// trailer contents
				tSize = 1.0f;
				oslSetFont(fonts[FONT_LIGHT]);
				oslIntraFontSetStyle(fonts[FONT_LIGHT], tSize, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), INTRAFONT_ALIGN_CENTER);
				sprintf(text, data.cargo);
				y += textures[TEXTURE_TRAILER]->sizeY / 2 + 15;
				oslDrawString(x, y, text);
			}
		}
	}
}

int initOSLib() 
{
	oslInit(0);
	oslInitGfx(OSL_PF_8888, 1);
	//oslInitAudio();
	oslSetQuitOnLoadFailure(1);
	oslSetKeyAutorepeatInit(40);
	oslSetKeyAutorepeatInterval(10);
	oslSetFrameskip(1);
	oslSetMaxFrameskip(3);
	oslSetVSync(1);
	return 0;
}

void resetHandles()
{
	for (size_t i = 0; i < TEXTURE_COUNT; i++)
		textures[i] = NULL;
	for (size_t i = 0; i < FONT_COUNT; i++)
		fonts[i] = NULL;
	speedometer = NULL;
	connection = -1;
	telemetryThreadID = -1;
	telDataSemaphore = -1;
}

void getServerAddress()
{
	try
	{
		// oepn
		FILE* file;
		if ((file = fopen(FILE_SERVER_ADDRESS, "r")) == NULL)
			throw 1;

		// read one line
		char line[256];
		fgets(line, sizeof(line), file);

		// close
		fclose(file);

		size_t i = 0;
		// read IP
		while (true)
		{
			char c = line[i];
			if (c == ':')
			{
				serverIP[i] = '\0';
				break;
			}
			if (c == '\0')
				throw 1;
			serverIP[i] = c;
			i++;
		}

		// read port
		char port[6];
		i++; // skip the delimiter
		size_t j = 0;
		while (true)
		{
			char c = line[i];
			if (c == '\0')
			{
				if (j == 0)
					throw 1;
				port[i] = '\0';
				break;
			}
			port[j] = c;
			i++;
			j++;
		}
		serverPort = atoi(port);
	}
	catch (...)
	{
		printf("Unable to open/parse \"%s\"\n", FILE_SERVER_ADDRESS);
		terminate(true);
	}
}

void getGearMap()
{
	try
	{
		// oepn
		FILE* file;
		if ((file = fopen(FILE_GEAR_MAP, "r")) == NULL)
			throw 1;

		// read
		size_t i = 0;
		while (fgets(gears[i], sizeof(gears[i]), file))
		{
			size_t len = strlen(gears[i]);
			if (gears[i][len - 1] == '\n' && len > 0)
				gears[i][len - 1] = '\0';
			i++;
		}

		// close
		fclose(file);
	}
	catch (...)
	{
		printf("Unable to open/parse \"%s\"\n", FILE_GEAR_MAP);
		terminate(true);
	}
}

bool loadTexture(unsigned int textureId, const char* filename, bool swizzle)
{
	bool result = false;
	try
	{
		textures[textureId] = loadPNG(filename, swizzle);
		if (textures[textureId] != NULL)
			result = true;
	}
	catch (...)
	{
		printf("Unable to load:\n \"%s\"\n", filename);
		result = false;
	}
	if (result == false)
		throw textureId;
	return result;
}

void loadTextures()
{
	printf("Loading textures...\n");
	try
	{
		loadTexture(TEXTURE_BACKGROUND, FILE_BACKGROUND);
		textures[TEXTURE_BACKGROUND]->x = 0;
		textures[TEXTURE_BACKGROUND]->y = 0;
		loadTexture(TEXTURE_PAUSE, FILE_PAUSE);
		loadTexture(TEXTURE_HOR_NUMB_BAR, FILE_HOR_NUMB_BAR);
		loadTexture(TEXTURE_VER_NUMB_BAR, FILE_VER_NUMB_BAR);
		loadTexture(TEXTURE_SPEED_CIRCLE, FILE_SPEED_CIRCLE);
		loadTexture(TEXTURE_CRUISE_CONTROL, FILE_CRUISE_CONTROL);
		loadTexture(TEXTURE_TACHOMETER, FILE_TACHOMETER);
		loadTexture(TEXTURE_TACHOMETER_NEEDLE, FILE_TACHOMETER_NEEDLE);
		loadTexture(TEXTURE_LEFT_BLINKER_LIGHT, FILE_LEFT_BLINKER_LIGHT);
		loadTexture(TEXTURE_RIGHT_BLINKER_LIGHT, FILE_RIGHT_BLINKER_LIGHT);
		loadTexture(TEXTURE_LOW_BEAM_LIGHT, FILE_LOW_BEAM_LIGHT);
		loadTexture(TEXTURE_HIGH_BEAM_LIGHT, FILE_HIGH_BEAM_LIGHT);
		loadTexture(TEXTURE_LOW_FUEL_LIGHT, FILE_LOW_FUEL_LIGHT);
		loadTexture(TEXTURE_RETARDER_LIGHT, FILE_RETARDER_LIGHT);
		loadTexture(TEXTURE_PARKING_BRAKE_LIGHT, FILE_PARKING_BRAKE_LIGHT);
		loadTexture(TEXTURE_BATTERY_LOW_LIGHT, FILE_BATTERY_LOW_LIGHT);
		loadTexture(TEXTURE_AIR_WARNING_LIGHT, FILE_AIR_WARNING_LIGHT);
		loadTexture(TEXTURE_AIR_EMERGENCY_LIGHT, FILE_AIR_EMERGENCY_LIGHT);
		loadTexture(TEXTURE_BEACON_LIGHT, FILE_BEACON_LIGHT);
		loadTexture(TEXTURE_SERVICE_LIGHT, FILE_SERVICE_LIGHT);
		loadTexture(TEXTURE_WATER_TEMPERATURE_LIGHT, FILE_WATER_TEMPERATURE_LIGHT);
		loadTexture(TEXTURE_OIL_PRESSURE_LIGHT, FILE_OIL_PRESSURE_LIGHT);
		loadTexture(TEXTURE_FUEL_BAR, FILE_FUEL_BAR);
		loadTexture(TEXTURE_TRAILER, FILE_TRAILER);
		loadTexture(TEXTURE_NAVIGATION, FILE_NAVIGATION);
		loadTexture(TEXTURE_WATER_ICON, FILE_WATER_ICON);
		loadTexture(TEXTURE_OIL_ICON, FILE_OIL_ICON);
		loadTexture(TEXTURE_FUEL_ICON, FILE_FUEL_ICON);
		loadTexture(TEXTURE_AIR_ICON, FILE_AIR_ICON);
	}
	catch (unsigned int e)
	{
		terminate(true);
	}
	catch (...)
	{
		printf("Unable to load or configure textures\n");
		terminate(true);
	}
}

void loadFonts()
{
	printf("Loading fonts...\n");
	try
	{
		fonts[FONT_DEFAULT] = oslLoadFontFile(FILE_FONT_DEFAULT);
		if (fonts[FONT_DEFAULT] == NULL)
			throw FONT_DEFAULT;

		fonts[FONT_LIGHT] = oslLoadFontFile(FILE_FONT_LIGHT);
		if (fonts[FONT_LIGHT] == NULL)
			throw FONT_LIGHT;

		fonts[FONT_BOLD] = oslLoadFontFile(FILE_FONT_BOLD);
		if (fonts[FONT_BOLD] == NULL)
			throw FONT_BOLD;
	}
	catch (int e)
	{
		printf("Unable to load ");
		switch (e)
		{
		case FONT_DEFAULT:
			printf("\"%s\"", FILE_FONT_DEFAULT);
			break;
		case FONT_LIGHT:
			printf("\"%s\"", FILE_FONT_LIGHT);
			break;
		case FONT_BOLD:
			printf("\"%s\"", FILE_FONT_BOLD);
			break;
		}
		printf("\n");
		terminate(true);
	}
	catch (...)
	{
		printf("Unable to load fonts\n");
		terminate(true);
	}
}

void loadDashboard()
{
	printf("Loading the dashboard...\n");
	try
	{
		speedometer = new SegmentDisplay(3, textures[TEXTURE_HOR_NUMB_BAR], textures[TEXTURE_VER_NUMB_BAR], 6, 1);

		selectedTab = 0;
	}
	catch (int e)
	{
		printf("Unable to load the dashboard\n");
		terminate(true);
	}
	catch (...)
	{
		printf("Unable to load the dashboard\n");
		terminate(true);
	}
}

void initNetworking()
{
	printf("Initializing networking...\n");
	try
	{
		if (sceUtilityLoadModule(PSP_MODULE_NET_COMMON) != 0)
			throw 1;
		if (sceUtilityLoadModule(PSP_MODULE_NET_INET) != 0)
			throw 2;
		if (pspSdkInetInit() != 0)
			throw 3;
	}
	catch (int e)
	{
		switch (e)
		{
		case 1:
			printf("Unable to load \"NET_COMMON\" module\n");
			break;
		case 2:
			printf("Unable to load \"NET_INET\" module\n");
			break;
		case 3:
			printf("pspSdkInetInit failed\n");
			break;
		}
		terminate(true);
	}
	catch (...)
	{
		printf("Unable to initialize networking\n");
		terminate(true);
	}
}

void connectToAccessPoint()
{
	printf("\n");
	try
	{
		// scan for Wi-Fi access point configurations
		// by checking which IDs (1 .. n) exist
		printf("Scanning for access point configurations...\n");
		int configurations[4]; // we only save the first four
		size_t count = 0;
		for (int config = 1; config <= MAX_AP_CONFIGS_TO_SCAN && count < 4; config++)
			if (sceUtilityCheckNetParam(config) == 0)
			{
				configurations[count] = config;
				count++;
			}
		int config;
		if (count == 0)
		{
			printf("No access point configurations found. Cannot continue\n");
			throw 0;
		}
		if (count == 1)
		{
			printf("One access point configuration found\n");
			config = configurations[0];
		}
		else
		{
			printf("Multiple access point configurations found\n");
			printf("Use action buttons to select\n");
			netData* param = (netData*)malloc(sizeof(netData));
			for (size_t i = 0; i < count; i++)
			{
				memset(param, 0, sizeof(netData));
				sceUtilityGetNetParam(configurations[i], PSP_NETPARAM_SSID, param);
				char button[16];
				switch (i)
				{
				case 0: strcpy(button, "CROSS"); break;
				case 1: strcpy(button, "CIRCLE"); break;
				case 2: strcpy(button, "SQUARE"); break;
				case 3: strcpy(button, "TRIANGLE"); break;
				}
				printf("  \"%s\"  -  %s\n", param->asString, button);
			}
			free(param);
			SceCtrlLatch latch;
			while (true)
			{
				sceDisplayWaitVblankStart();
				sceCtrlReadLatch(&latch);
				if (latch.uiBreak & PSP_CTRL_CROSS && count >= 1)
				{
					config = configurations[0];
					break;
				}
				if (latch.uiBreak & PSP_CTRL_CIRCLE && count >= 2)
				{
					config = configurations[1];
					break;
				}
				if (latch.uiBreak & PSP_CTRL_SQUARE && count >= 3)
				{
					config = configurations[2];
					break;
				}
				if (latch.uiBreak & PSP_CTRL_TRIANGLE && count >= 4)
				{
					config = configurations[3];
					break;
				}
			}
		}

		// connect
		netData* param = (netData*)malloc(sizeof(netData));
		memset(param, 0, sizeof(netData));
		sceUtilityGetNetParam(config, PSP_NETPARAM_SSID, param);
		printf("\n");
		printf("Connecting to access point \"%s\"...\n", param->asString);
		free(param);

		int error = sceNetApctlConnect(config);
		if (error != 0)
		{
			printf("Failed to connect\n");
			printf(" sceNetApctlConnect() returned: 0x%08X\n", error);
			throw 0;
		}

		int previousState = -1;
		bool connectionInitiated = false;
		while (true)
		{
			int state;
			error = sceNetApctlGetState(&state);
			if (error != 0)
			{
				printf("Failed to connect\n");
				printf(" sceNetApctlGetState() returned: 0x%08X\n", error);
				throw 0;
			}
			if (state != previousState)
			{
				if (!connectionInitiated && state != PSP_NET_APCTL_STATE_DISCONNECTED)
					connectionInitiated = true;
				if (connectionInitiated && state == PSP_NET_APCTL_STATE_DISCONNECTED)
				{
					printf("Connection attempt aborted\n");
					throw 0;
				}
				char stateinfo[25];
				switch (state)
				{
				case PSP_NET_APCTL_STATE_DISCONNECTED:
					if (previousState == -1)
						strcpy(stateinfo, "waiting");
					else
						throw 1;
					break;
				case PSP_NET_APCTL_STATE_SCANNING:
					strcpy(stateinfo, "scanning");
					break;
				case PSP_NET_APCTL_STATE_JOINING:
					strcpy(stateinfo, "joining");
					break;
				case PSP_NET_APCTL_STATE_GETTING_IP:
					strcpy(stateinfo, "obtaining IP address");
					break;
				case PSP_NET_APCTL_STATE_GOT_IP:
					strcpy(stateinfo, "IP address obtained");
					break;
				case PSP_NET_APCTL_STATE_EAP_AUTH:
					strcpy(stateinfo, "authenticating using EAP");
					break;
				case PSP_NET_APCTL_STATE_KEY_EXCHANGE:
					strcpy(stateinfo, "exchanging key information");
					break;
				}
				printf(" %s\n", stateinfo);
				previousState = state;
			}
			if (state == PSP_NET_APCTL_STATE_GOT_IP)
				break; // connected

			// wait a little before polling again
			sleepMS(50);
		}

		printf("Connected to access point\n");
	}
	catch (int e)
	{
		switch (e)
		{
		case 0: break;
		case 1:
			printf("Failed to connect\n");
			break;
		}
		terminate(true);
	}
	catch (...)
	{
		printf("Unable to connect to an access point\n");
		terminate(true);
	}
}

void disconnectFromAccessPoint()
{
	if (isConnectedToAP())
	{
		sceNetApctlDisconnect();
		while (isConnectedToAP())
			sleepMS(10);
	}
}

void freeResources()
{
	delete speedometer;
	for (size_t i = 0; i < TEXTURE_COUNT; i++)
		if (textures[i])
			oslDeleteImage(textures[i]);
	for (size_t i = 0; i < FONT_COUNT; i++)
		if (fonts[i])
			oslDeleteFont(fonts[i]);
}

void terminate(bool waitForUser)
{
	// disconnect
	disconnect();
	disconnectFromAccessPoint();

	// clean-up
	freeResources();
	pspSdkInetTerm();
	sceUtilityUnloadModule(PSP_MODULE_NET_INET);
	sceUtilityUnloadModule(PSP_MODULE_NET_COMMON);
	oslEndGfx();

	// wait user acknowledgement
	if (waitForUser)
	{
		printf("\n");
		printf("Press X to quit . . .\n");
		SceCtrlLatch latch;
		while (true)
		{
			sceDisplayWaitVblankStart();
			sceCtrlReadLatch(&latch);
			if (latch.uiBreak & PSP_CTRL_CROSS)
				break;
		}
	}

	// quit
	sceKernelExitGame();
}

int telemetryThread(SceSize args, void* argp)
{
	int clientSocket = connection;

	TPacketBuffer packetBuffer(PACKET_BUFFER_SIZE);
	char recvBuffer[RECV_BUFFER_SIZE];
	char payload[PACKET_PAYLOAD_BUFFER_SIZE];
	TPacket packet;

	setName();

	while (true)
	{
		int result = recv(clientSocket, recvBuffer, RECV_BUFFER_SIZE, 0);
		if (result > 0) // received data
		{
			if (result > RECV_BUFFER_SIZE)
				break;

			if (!packetBuffer.write(recvBuffer, (size_t)result))
				break;

			while (packetBuffer.getPacket(packet, payload, PACKET_PAYLOAD_BUFFER_SIZE))
			{
				if (packet.packetType == PACKET_TELDATA)
				{
					SceUInt timeout = 10 * 1000;
					sceKernelWaitSema(telDataSemaphore, 1, &timeout);
					memcpy(telDataBlock, payload, TEL_DATA_BLOCK_SIZE);
					sceKernelSignalSema(telDataSemaphore, 1);
				}
			}
		}
		if (result == 0) // graceful disconnect
		{
			break;
		}
		if (result < 0) // error
		{
			break;
		}
	}

	sceKernelExitThread(0);
	return 0;
}