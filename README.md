## Virtual dashboard for Euro Truck Simulator 2 running on the PSP

This is a free dashboard made for [Euro Truck Simulator 2](https://eurotrucksimulator2.com) to display various kinds of information from the game. It is written in C++ for the [PlayStation Portable](https://en.wikipedia.org/wiki/PlayStation_Portable).

The project was made for people like me who have a spare PSP laying around, and people who don't want to have in-game HUD enabled or for some other reason cannot see the truck's dashboard, for example, due to low screen resolution.

There are two parts - the server and the client. The server is a Windows application that hooks to the game and sends the telemetry data to all connected clients over TCP/IP. The dashboard itself is the said server client.

### Requirements

Your device has to have CFW (custom firmware) installed as stock PSP firmware does not allow running custom binaries.

### Dashboard overview

The dashboard displays following information:

- Speed, selected gear, engine RPM
- Speed limit, cruise control speed
- Time, weekday
- Indicator lights (low beams, blinkers, parking brake, low air pressure, etc)
- Truck information (model, kilometers traveled, oil temperature, etc)
- Trailer information (cargo, weight)
- Job information (start & destination, deadline)
- Route status (distance & time left)

![dashboard trailer tab](https://i.imgur.com/CpS6fbH.png)

### Setting up

1. Download and install [ETS2 SDK plug-in](https://github.com/nlhans/ets2-sdk-plugin/releases).
2. Connect your PSP to your computer.
3. Inside **dashboard/tasks/** edit the **target directory.cmd** file so that it points to the PSP that you've connected (you only have to change the drive letter).
4. Inside **dashboard/resources/** open **server address.txt** and replace the given IP with your computer's IP address. Using your local address over public IP address is recommended.
4. Run **dashboard/tasks/upload all.cmd**.

Due to user inactivity the PSP's screen will eventually turn off, until I find a way to disable that within the application, you will have to manually disable this feature by going to Settings > Power Save Settings and toggling Backlight Auto-Off to Off.

### Usage

1. Run Euro Truck Simulator 2 and enter the world.
2. Start the server by running **server/bin/ETS2_Telemetry_Server.exe** and clicking **Start server**.
3. Run **ETS2 Dashboard** on your PSP and wait for it to connect to the server.
4. Done. If everything went well, the dashboard will be displayed.

To switch between the information panels use L/R triggers, Left/Right or Square/Circle buttons. To toggle between kilometers and miles use Select. Press Start or Home button to quit.

### Transmission configurations

Most transmissions in game have 14 forward gears: 2 crawler gears and 12 drive gears; but if you are using a different configuration of forward gears, you have to edit the **dashboard/resources/gear map.txt** accordingly.

### Developer information

##### Notes

- The PSP dashboard is compiled using [PSPSDK](https://sourceforge.net/projects/minpspw), it's pretty outdated so don't expect much.
- The server part is a Visual Studio 2017 project, the PSP dashboard is a Makefile project so you can use your preferred editor or IDE to work on it.
- All shared units are located in **dashboard/source/** folder.

##### Win32 Client

Win32 Client is mainly used for testing connectivity between the server and the client, but it can also be used as a base for creating custom dashboards on Windows. May be removed in future.