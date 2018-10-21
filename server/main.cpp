#include "Main.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	InitCommonControls(); // enable visual styles
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAINDIALOG), NULL, (DLGPROC)MainWndProc);
	return 0;
}

BOOL CALLBACK MainWndProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg) {
	case WM_INITDIALOG:
		hWindow = hWnd;
		onCreate();
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			DestroyWindow(hWnd);
			return TRUE;
		case IDCANCEL:
			stopServer();
			DestroyWindow(hWnd);
			return TRUE;
		case IDC_STARTSERVER:
			startServer();
			return TRUE;
		case IDC_STOPSERVER:
			stopServer();
			return TRUE;
		}
		return FALSE;
	case WM_DESTROY:
		onDestroy();
		PostQuitMessage(0);
		return TRUE;
	case MSG_TELDATACHANGED:
		onMsgTelDataChanged();
		return TRUE;
	case MSG_SHUTDOWN:
		onMsgShutdown((int)wParam);
		return TRUE;
	case MSG_CLIENTCONNECT:
		onMsgClientConnect((SOCKET)wParam);
		return TRUE;
	case MSG_CLIENTDISCONNECT:
		onMsgClientDisconnect((SOCKET)wParam);
		return TRUE;
	case MSG_CLIENTNAMESET:
		onMsgClientNameSet((SOCKET)wParam);
		return TRUE;
	}
	return FALSE;
}

void onCreate()
{
	setStatus("initializing...");

	// UI
	toggleConnectControls(false);
	toggleControl(IDC_STOPSERVER, false);
	SendMessage(GetDlgItem(hWindow, IDC_UPDATEPOLICY2), BM_SETCHECK, BST_CHECKED, 0);
	SetDlgItemInt(hWindow, IDC_UPDATEINTERVAL, DEFAULT_SLEEP_INTERVAL, FALSE);
	SetDlgItemInt(hWindow, IDC_PORT, DEFAULT_PORT, FALSE);
	displayTelData(nullptr);

	// list box initialization
	SendDlgItemMessage(hWindow, IDC_CLIENTS, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)LVS_EX_FULLROWSELECT);
	// columns
	LVCOLUMN col = { 0 };
	col.mask = LVCF_TEXT | LVCF_WIDTH;
	// name column
	col.pszText = (LPSTR)"Handle";
	col.cx = 80;
	SendDlgItemMessage(hWindow, IDC_CLIENTS, LVM_INSERTCOLUMN, 0, (LPARAM)&col);
	// PID
	col.pszText = (LPSTR)"IP";
	col.cx = 100;
	SendDlgItemMessage(hWindow, IDC_CLIENTS, LVM_INSERTCOLUMN, 1, (LPARAM)&col);
	// state
	col.pszText = (LPSTR)"Name";
	col.cx = 130;
	SendDlgItemMessage(hWindow, IDC_CLIENTS, LVM_INSERTCOLUMN, 2, (LPARAM)&col);

	// variables
	resetThreadHandles();
	shuttingDown = false;
	running = false;
	InitializeCriticalSection(&telDataCS);
	telData = malloc(TEL_DATA_BLOCK_SIZE);
	memset(telData, 0, TEL_DATA_BLOCK_SIZE);
	serverSocket = INVALID_SOCKET;
	clients = new TClients(MAX_CONNECTIONS, hWindow, MSG_CLIENTCONNECT, MSG_CLIENTDISCONNECT, MSG_CLIENTNAMESET);

	// Winsock initialization
	if (initializeWinsock())
	{
		toggleConnectControls(true);
		setStatus("idle");
	}
	else
		setStatus("Winsock fail");
}

void onDestroy()
{
	DeleteCriticalSection(&telDataCS);
	free(telData);
	delete clients;
	terminateWinsock();
}

void startServer()
{
	if (running || shuttingDown)
		return;

	while (true)
	{
		setStatus("starting...");

		// UI
		toggleConnectControls(false);

		// reset global variables
		running = false;
		shuttingDown = false;
		hMapFile = NULL;
		pMapFile = NULL;
		resetThreadHandles();
		serverSocket = INVALID_SOCKET;

#ifndef NO_GAME
		char message[TEXT_BUF_SIZE];
		int errorCode;

		// hook to the game
		hMapFile = OpenFileMapping(
			FILE_MAP_READ,     // read-only access
			FALSE,             // do not inherit the name
			sharedMemFileName  // name of the mapping object
		);
		if (hMapFile == NULL)
		{
			errorCode = GetLastError();
			setStatus("telemetry access fail");
			sprintf_s(message, TEXT_BUF_SIZE, "Failed to open access to telemetry data. Error code: 0x%08X", errorCode);
			errorBox("Failed to hook to the game", message);
			break;
		}

		pMapFile = (LPTSTR)MapViewOfFile(
			hMapFile,       // handle to the map object
			FILE_MAP_READ,  // read-only permission
			0,			    // none high offset
			0,			    // none low offset
			TEL_DATA_BLOCK_SIZE        // mapping size
		);
		if (pMapFile == NULL)
		{
			errorCode = GetLastError();
			setStatus("telemetry access fail");
			sprintf_s(message, TEXT_BUF_SIZE, "Failed to map telemetry data. Error code: 0x%08X", errorCode);
			errorBox("Failed to hook to the game", message);
			break;
		}
#endif

		// create a socket server
		unsigned int port = GetDlgItemInt(hWindow, IDC_PORT, NULL, FALSE);

		sockaddr_in local;
		local.sin_family = AF_INET;
		local.sin_addr.s_addr = INADDR_ANY;
		local.sin_port = htons((u_short)port);

		serverSocket = socket(local.sin_family, SOCK_STREAM, IPPROTO_TCP);
		if (serverSocket == INVALID_SOCKET)
		{
			errorCode = WSAGetLastError();
			setStatus("server fail");
			sprintf_s(message, TEXT_BUF_SIZE, "Failed to create a listening server. Error code: 0x%08X", errorCode);
			errorBox("Failed to create a server. socket() fail", message);
			break;
		}

		if (bind(serverSocket, (sockaddr*)&local, sizeof(local)) == SOCKET_ERROR)
		{
			errorCode = WSAGetLastError();
			setStatus("server fail");
			sprintf_s(message, TEXT_BUF_SIZE, "Failed to create a listening server. Error code: 0x%08X", errorCode);
			errorBox("Failed to create a server. bind() fail", message);
			break;
		}

		// disable Nagle's algorithm
		int iOptVal = TRUE; // NO DELAY = TRUE
		int iOptLen = sizeof(int);
		setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, (char*)&iOptVal, iOptLen);

		if (listen(serverSocket, MAX_CONNECTIONS) == SOCKET_ERROR)
		{
			errorCode = WSAGetLastError();
			setStatus("server fail");
			sprintf_s(message, TEXT_BUF_SIZE, "Failed to create a listening server. Error code: 0x%08X", errorCode);
			errorBox("Failed to create a server. listen() fail", message);
			break;
		}

		// telemetry thread sleep interval
		telThreadSleepInterval = MIN_SLEEP_INTERVAL;
		if (IsDlgButtonChecked(hWindow, IDC_UPDATEPOLICY2))
		{
			telThreadSleepInterval = GetDlgItemInt(hWindow, IDC_UPDATEINTERVAL, NULL, FALSE);
			if (telThreadSleepInterval < MIN_SLEEP_INTERVAL)
				telThreadSleepInterval = MIN_SLEEP_INTERVAL;
		}
		else
			telThreadSleepInterval = DEFAULT_SLEEP_INTERVAL;

		// start the threads
		threads[0] = CreateThread(NULL, 0, telemetryDataThread, NULL, 0, NULL);
		threads[1] = CreateThread(NULL, 0, serverAcceptThread, NULL, 0, NULL);

		running = true;
		toggleConnectControls(false);
		toggleControl(IDC_STOPSERVER, true);
		setStatus("running");

		return;
	}

	if (pMapFile != NULL)
	{
		UnmapViewOfFile(pMapFile);
		pMapFile = NULL;
	}
	if (hMapFile != NULL)
	{
		CloseHandle(hMapFile);
		hMapFile = NULL;
	}
	if (serverSocket != INVALID_SOCKET)
	{
		closesocket(serverSocket);
		serverSocket = INVALID_SOCKET;
	}
	toggleConnectControls(true);
}

void stopServer()
{
	if (!running || shuttingDown)
		return;

	setStatus("stopping...");
	toggleControl(IDC_STOPSERVER, false);

	shuttingDown = true;
	if (serverSocket != INVALID_SOCKET)
	{
		shutdown(serverSocket, SD_BOTH);
		closesocket(serverSocket);
		serverSocket = INVALID_SOCKET;
	}
	clients->disconnect();
	waitForThreads();
	resetThreadHandles(true);

	if (pMapFile != NULL)
	{
		UnmapViewOfFile(pMapFile);
		pMapFile = NULL;
	}

	if (hMapFile != NULL)
	{
		CloseHandle(hMapFile);
		hMapFile = NULL;
	}

	running = false;
	shuttingDown = false;
	displayTelData(nullptr);
	toggleConnectControls(true);
	setStatus("idle");
}

void displayTelData(TETS2_Telemetry* data)
{
	if (data == nullptr || data->time == TEL_DATA_EMPTY_TIMESTAMP)
	{
		SetDlgItemText(hWindow, IDC_TELDATAFIELD1, "N/A");
		SetDlgItemText(hWindow, IDC_TELDATAFIELD2, "N/A");
		SetDlgItemText(hWindow, IDC_TELDATAFIELD3, "N/A");
		SetDlgItemText(hWindow, IDC_TELDATAFIELD4, "N/A");
		SetDlgItemText(hWindow, IDC_TELDATAFIELD5, "N/A");
		SetDlgItemText(hWindow, IDC_TELDATAFIELD6, "N/A");
		SetDlgItemText(hWindow, IDC_TELDATAFIELD7, "N/A");
		SetDlgItemText(hWindow, IDC_TELDATAFIELD8, "N/A");
	}
	else
	{
		char charBuffer[TEXT_BUF_SIZE];

		// time
		char day[3+1];
		switch (data->day)
		{
		case 1: strcpy_s(day, 4, "Mon"); break;
		case 2: strcpy_s(day, 4, "Tue"); break;
		case 3: strcpy_s(day, 4, "Wed"); break;
		case 4: strcpy_s(day, 4, "Thu"); break;
		case 5: strcpy_s(day, 4, "Fri"); break;
		case 6: strcpy_s(day, 4, "Sat"); break;
		case 7: strcpy_s(day, 4, "Sun"); break;
		default: strcpy_s(day, 4, "???");
		}
		sprintf_s(charBuffer, TEXT_BUF_SIZE, "%s %02d:%02d", day, data->hour, data->minute);
		SetDlgItemText(hWindow, IDC_TELDATAFIELD1, charBuffer);

		// truck
		sprintf_s(charBuffer, TEXT_BUF_SIZE, "%s %s", data->truckManufacturer, data->truckModel);
		SetDlgItemText(hWindow, IDC_TELDATAFIELD2, charBuffer);

		// odometer
		sprintf_s(charBuffer, TEXT_BUF_SIZE, "%u km", data->odometerKm);
		SetDlgItemText(hWindow, IDC_TELDATAFIELD3, charBuffer);

		// lights
		sprintf_s(charBuffer, TEXT_BUF_SIZE, "%s %s %s %s %s %s", 
			data->blinkerLeft ? "<" : " ",
			data->lightsParking ? "P" : " ",
			data->lightsBeamLow ? "L" : " ",
			data->lightsBeamHigh ? "H" : " ",
			data->lightsBeacon ? "B" : " ",
			data->blinkerRight ? ">" : " "
		);
		SetDlgItemText(hWindow, IDC_TELDATAFIELD4, charBuffer);

		// speed
		sprintf_s(charBuffer, TEXT_BUF_SIZE, "%d km/h", data->speedKmh);
		SetDlgItemText(hWindow, IDC_TELDATAFIELD5, charBuffer);

		// RPM
		sprintf_s(charBuffer, TEXT_BUF_SIZE, "%u / %u", data->engineRPM, data->engineRPMMax);
		SetDlgItemText(hWindow, IDC_TELDATAFIELD6, charBuffer);

		// fuel
		sprintf_s(charBuffer, TEXT_BUF_SIZE, "%u / %u", data->fuel, data->fuelCapacity);
		SetDlgItemText(hWindow, IDC_TELDATAFIELD7, charBuffer);

		// cargo
		SetDlgItemText(hWindow, IDC_TELDATAFIELD8, data->cargo);
	}
}

void onMsgTelDataChanged()
{
	enterTelDataCS();
	TETS2_Telemetry tel;
	if (tel.load(telData))
		displayTelData(&tel);
		SOCKET sockets[MAX_CONNECTIONS];
		size_t count = clients->getSockets(sockets);
		for (size_t i = 0; i < count && count > 0; i++)
			sendTelemetryData(sockets[i]);
	leaveTelDataCS();
}

void onMsgShutdown(int reason)
{
	stopServer();
	switch (reason)
	{
	case SHR_READFAIL:
		setStatus("tel. data read fail");
		errorBox("Read fail", "Failed to access/parse telemetry data");
		break;
	case SHR_ACCEPTFAIL:
		setStatus("server fail");
		errorBox("Server fault", "Server has failed");
		break;
	default:
		setStatus("unexpected shutdown");
		errorBox("Unexpected shutdown", "An unknown fault has occurred");
	}
}

void onMsgClientConnect(SOCKET socket)
{
	addListItem(socket);
	sendTelemetryData(socket);
}

void onMsgClientDisconnect(SOCKET socket)
{
	removeListItem(socket);
}

void onMsgClientNameSet(SOCKET socket)
{
	TClient* client = clients->get(socket);
	if (client)
	{
		int row = getListItem(socket);
		if (row >= 0)
		{
			setListItem((size_t)row, 2, client->name);
		}
	}
}

void resetThreadHandles(bool closeHandles)
{
	for (size_t i = 0; i < THREAD_COUNT; i++)
	{
		if (closeHandles)
			CloseHandle(threads[i]);
		threads[i] = INVALID_HANDLE_VALUE;
	}
}

void enterTelDataCS()
{
	EnterCriticalSection(&telDataCS);
}

void leaveTelDataCS()
{
	LeaveCriticalSection(&telDataCS);
}

void postMessage(UINT message, int arg)
{
	PostMessage(hWindow, message, (WPARAM)arg, NULL);
}

void waitForThreads()
{
	HANDLE threads_[THREAD_COUNT + MAX_CONNECTIONS];
	size_t count = 0;

	for (size_t i = 0; i < THREAD_COUNT; i++)
		if (threads[i] != INVALID_HANDLE_VALUE)
		{
			threads_[count] = threads[i];
			count++;
		}

	// wait for main threads
	if (count > 0)
		WaitForMultipleObjects((DWORD)count, threads_, TRUE, INFINITE);

	// wait for client threads
	count = clients->getThreads(&threads_[0]);
	if (count > 0)
		WaitForMultipleObjects((DWORD)count, threads_, TRUE, INFINITE);
}

void clearListBox()
{
	ListView_DeleteAllItems(GetWindow(hWindow, IDC_CLIENTS));
}

void addListItem(SOCKET socket)
{
	TClient* client = clients->get(socket);
	if (client)
	{
		LVITEM item = { 0 };
		item.mask = LVIF_PARAM;
		item.iItem = MAX_CONNECTIONS; // put it at the end
		LRESULT pos = SendDlgItemMessage(hWindow, IDC_CLIENTS,
			LVM_INSERTITEM, 0, (LPARAM)&item);

		char text[TEXT_BUF_SIZE];

		// handle
		sprintf_s(text, TEXT_BUF_SIZE, "%u", (unsigned int)client->socket);
		setListItem((size_t)pos, 0, text);

		// IP
		setListItem((size_t)pos, 1, client->IP);

		// name
		setListItem((size_t)pos, 2, client->name);
	}
}

void removeListItem(SOCKET socket)
{
	int row = getListItem(socket);
	if (row >= 0)
		SendDlgItemMessage(hWindow, IDC_CLIENTS,
			LVM_DELETEITEM, row, 0);
}

int getListItem(SOCKET socket)
{
	int count = ListView_GetItemCount(GetDlgItem(hWindow, IDC_CLIENTS));
	int row = -1;
	for (int i = 0; i < count; i++)
	{
		LVITEM item = { 0 };
		item.mask = LVIF_TEXT;
		item.iItem = i;
		item.iSubItem = 0;

		char text[TEXT_BUF_SIZE];
		item.cchTextMax = TEXT_BUF_SIZE;
		item.pszText = (LPTSTR)(LPCTSTR)text;

		SendDlgItemMessage(hWindow, IDC_CLIENTS,
			LVM_GETITEM, 0, (LPARAM)&item);		if ((SOCKET)atoi(text) == socket)		{			row = i;			break;		}
	}
	return row;
}

void setListItem(size_t row, size_t column, char* value)
{
	LVITEM item = { 0 };
	item.mask = LVIF_PARAM;
	item.mask = LVIF_TEXT;
	item.iItem = (int)row;
	item.iSubItem = (int)column;
	item.pszText = value;
	SendDlgItemMessage(hWindow, IDC_CLIENTS,
		LVM_SETITEM, 0, (LPARAM)&item);
}

bool initializeWinsock()
{
	WSADATA wsaData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	char message[TEXT_BUF_SIZE];
	if (result != 0)
	{
		sprintf_s(message, TEXT_BUF_SIZE, "Winsock initialization failed. Error code: 0x%08X", result);
		return false;
	}
	else
		return true;
}

void terminateWinsock()
{
	WSACleanup();
}

void sendTelemetryData(SOCKET client)
{
	enterTelDataCS();
		TPacket packet;
		packet.packetType = PACKET_TELDATA;
		packet.payloadSize = TEL_DATA_BLOCK_SIZE;
		send(client, (char*)&packet, PACKET_SIZE, 0);
		send(client, (char*)telData, packet.payloadSize, 0);
	leaveTelDataCS();
}

void setStatus(const char* status)
{
	SetDlgItemText(hWindow, IDC_STATUS, status);
}

void errorBox(const char* caption, const char* message)
{
	EnableWindow(hWindow, FALSE);
	MessageBox(NULL, message, caption, MB_OK | MB_ICONERROR);
	EnableWindow(hWindow, TRUE);
}

void toggleControl(int control, bool enable)
{
	EnableWindow(GetDlgItem(hWindow, control), enable ? TRUE : FALSE);
}

void toggleConnectControls(bool enable)
{
	BOOL bEnable = enable ? TRUE : FALSE;
	toggleControl(IDC_STARTSERVER, bEnable);
	toggleControl(IDC_PORT, bEnable);
	toggleControl(IDC_UPDATEPOLICY1, bEnable);
	toggleControl(IDC_UPDATEPOLICY2, bEnable);
	toggleControl(IDC_UPDATEINTERVAL, bEnable);
	toggleControl(IDC_MSLABEL, bEnable);
}

DWORD telemetryDataThread(LPVOID lpParameter)
{
	// variables
	unsigned int previousTime = 0;
	void* memFile = (void*)pMapFile;
	DWORD sleepInterval = telThreadSleepInterval;

	// main loop
	while (!shuttingDown)
	{
		Sleep(sleepInterval);

		// we have slept so we have to
		// check this once again
		if (shuttingDown)
			break;

#ifndef NO_GAME
		try
		{
			if (memFile == nullptr)
				throw 1;

			// check if there are changes
			unsigned int time = getTimestamp(memFile);

			// read telemetry data (if needed)
			if (time != previousTime)
			{
				enterTelDataCS();
				memcpy(telData, memFile, TEL_DATA_BLOCK_SIZE);
				leaveTelDataCS();

				previousTime = time;

				postMessage(MSG_TELDATACHANGED);
			}
		}
		catch (...)
		{
			postMessage(MSG_SHUTDOWN, SHR_READFAIL);
			break;
		}
#else
		postMessage(MSG_TELDATACHANGED); // simulate updates
#endif
	}

	return 0;
}

DWORD serverAcceptThread(LPVOID lpParameter)
{
	SOCKET client;

	sockaddr_in remote;
	int remoteStructSize = sizeof(remote);

	while (!shuttingDown)
	{
		client = accept(serverSocket, (sockaddr*)&remote, &remoteStructSize);
		if (client == INVALID_SOCKET)
		{
			if (shuttingDown)
				break;
			postMessage(MSG_SHUTDOWN, SHR_ACCEPTFAIL);
			break;
		}
		else
		{
			TClient connection;
			connection.socket = client;
			inet_ntop(remote.sin_family, &remote.sin_addr, connection.IP, INET_ADDRSTRLEN);
			connection.name[0] = '\0';

			if (clients->count() < MAX_CONNECTIONS)
			{
				HANDLE thread = CreateThread(NULL, 0, clientThread, (LPVOID)client, CREATE_SUSPENDED, NULL);
				connection.thread = thread;
				clients->add(connection);
				ResumeThread(thread);
			}
			else
			{
				shutdown(client, SD_BOTH);
				closesocket(client);
			}
		}
	}

	return 0;
}

DWORD WINAPI clientThread(LPVOID lpParameter)
{
	SOCKET client = (SOCKET)lpParameter;

	TPacketBuffer packetBuffer(PACKET_BUFFER_SIZE);
	char recvBuffer[RECV_BUFFER_SIZE];
	char payload[PACKET_PAYLOAD_BUFFER_SIZE];
	TPacket packet;

	while (!shuttingDown)
	{	
		int result = recv(client, recvBuffer, RECV_BUFFER_SIZE, 0);
		if (shuttingDown)
			break;
		if (result > 0) // received data
		{
			if (result > RECV_BUFFER_SIZE)
				break;

			if (!packetBuffer.write(recvBuffer, (size_t)result))
				break;

			while (packetBuffer.getPacket(packet, payload, PACKET_PAYLOAD_BUFFER_SIZE))
			{
				if (packet.packetType == PACKET_NAME)
				{
					clients->setName(client, string(payload));
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

	if (!shuttingDown)
		closesocket(client);

	clients->remove(client);

	return 0;
}