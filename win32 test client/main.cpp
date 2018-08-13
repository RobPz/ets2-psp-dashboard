#include "main.h"

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
			disconnect();
			DestroyWindow(hWnd);
			return TRUE;
		case IDC_CONNECT:
			connect();
			return TRUE;
		case IDC_DISCONNECT:
			disconnect();
			return TRUE;
		case IDC_SETNAME:
			setName();
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
	case MSG_CONNECTED:
		onMsgConnect();
		return TRUE;
	case MSG_DISCONNECTED:
		onMsgDisconnect();
		return TRUE;
	}
	return FALSE;
}

void onCreate()
{
	setStatus("initializing...");

	// UI
	toggleControl(IDC_CONNECT, false);
	toggleControl(IDC_DISCONNECT, false);
	toggleControl(IDC_ADDRESS, false);
	toggleControl(IDC_PORT, false);
	toggleControl(IDC_NAME, false);
	toggleControl(IDC_SETNAME, false);
	SetDlgItemText(hWindow, IDC_ADDRESS, "127.0.0.1");
	SetDlgItemInt(hWindow, IDC_PORT, DEFAULT_PORT, FALSE);
	SetDlgItemText(hWindow, IDC_NAME, "WIN32 DEMO");
	displayTelData(nullptr);

	// variables
	shuttingDown = false;
	thread = INVALID_HANDLE_VALUE;
	clientSocket = INVALID_SOCKET;
	InitializeCriticalSection(&telDataCS);
	telData = malloc(TEL_DATA_BLOCK_SIZE);
	memset(telData, 0, TEL_DATA_BLOCK_SIZE);

	// Winsock initialization
	if (initializeWinsock())
	{
		toggleControl(IDC_CONNECT, true);
		toggleControl(IDC_ADDRESS, true);
		toggleControl(IDC_PORT, true);
		setStatus("idle");
	}
	else
		setStatus("Winsock fail");
}

void onDestroy()
{
	DeleteCriticalSection(&telDataCS);
	free(telData);
	terminateWinsock();
}

void connect()
{
	if (shuttingDown)
		return;

	setStatus("connecting...");

	// reset global variables
	shuttingDown = false;
	clientSocket = INVALID_SOCKET;

	// UI
	toggleControl(IDC_CONNECT, false);
	toggleControl(IDC_ADDRESS, false);
	toggleControl(IDC_PORT, false);

	thread = CreateThread(NULL, 0, clientThread, NULL, 0, NULL);
}

void disconnect()
{
	if (shuttingDown)
		return;

	setStatus("disconnecting...");
	toggleControl(IDC_DISCONNECT, false);

	shuttingDown = true;
	if (clientSocket != INVALID_SOCKET)
	{
		shutdown(clientSocket, SD_BOTH);
		closesocket(clientSocket);
		clientSocket = INVALID_SOCKET;
	}
	if (thread != INVALID_HANDLE_VALUE)
	{
		WaitForSingleObject(thread, INFINITE);
		CloseHandle(thread);
		thread = INVALID_HANDLE_VALUE;
	}

	shuttingDown = false;
	displayTelData(nullptr);

	toggleControl(IDC_CONNECT, true);
	toggleControl(IDC_ADDRESS, true);
	toggleControl(IDC_PORT, true);
	toggleControl(IDC_NAME, false);
	toggleControl(IDC_SETNAME, false);
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
		char day[3 + 1];
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
		sprintf_s(charBuffer, TEXT_BUF_SIZE, "%u km", data->odometer);
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

void setName()
{
	char name[TEXT_BUF_SIZE] = "";
	GetDlgItemText(hWindow, IDC_NAME, name, TEXT_BUF_SIZE);

	uint16_t nameLength = (uint16_t)strlen(name) + 1;
	TPacket packet;
	packet.packetType = PACKET_NAME;
	packet.payloadSize = nameLength;
	send(clientSocket, (char*)&packet, PACKET_SIZE, 0);
	send(clientSocket, name, (int)nameLength, 0);
}

void onMsgTelDataChanged()
{
	enterTelDataCS();
	TETS2_Telemetry tel;
	if (tel.load(telData))
		displayTelData(&tel);
	leaveTelDataCS();
}

void onMsgShutdown(int reason)
{
	disconnect();
	switch (reason)
	{
	case SHR_CONNECTFAIL:
		setStatus("failed to connect");
		errorBox("Unable to establish a connection", "Failed to connect");
		break;
	default:
		setStatus("unexpected shutdown");
		errorBox("Unexpected shutdown", "An unknown fault has occurred");
	}
}

void onMsgConnect()
{
	toggleControl(IDC_DISCONNECT, true);
	toggleControl(IDC_NAME, true);
	toggleControl(IDC_SETNAME, true);

	setStatus("connected");
}

void onMsgDisconnect()
{
	disconnect();

	toggleControl(IDC_CONNECT, true);
	toggleControl(IDC_ADDRESS, true);
	toggleControl(IDC_PORT, true);
	toggleControl(IDC_NAME, false);
	toggleControl(IDC_SETNAME, false);

	setStatus("disconnected");
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

DWORD clientThread(LPVOID lpParameter)
{
	char address[TEXT_BUF_SIZE] = "";
	GetDlgItemText(hWindow, IDC_ADDRESS, address, TEXT_BUF_SIZE);
	unsigned int port = GetDlgItemInt(hWindow, IDC_PORT, NULL, FALSE);

	sockaddr_in remote;
	remote.sin_family = AF_INET;
	inet_pton(AF_INET, address, &remote.sin_addr);
	remote.sin_port = htons((u_short)port);

	clientSocket = socket(remote.sin_family, SOCK_STREAM, IPPROTO_TCP);
	if (clientSocket == INVALID_SOCKET)
	{
		postMessage(MSG_SHUTDOWN, SHR_CONNECTFAIL);
		return 1;
	}

	int result = connect(clientSocket, (sockaddr*)&remote, sizeof(remote));
	if (result == SOCKET_ERROR)
	{
		postMessage(MSG_SHUTDOWN, SHR_CONNECTFAIL);
		return 2;
	}

	postMessage(MSG_CONNECTED);

	TPacketBuffer packetBuffer(PACKET_BUFFER_SIZE);
	char recvBuffer[RECV_BUFFER_SIZE];
	char payload[PACKET_PAYLOAD_BUFFER_SIZE];
	TPacket packet;

	while (!shuttingDown)
	{
		int result = recv(clientSocket, recvBuffer, RECV_BUFFER_SIZE, 0);
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
				if (packet.packetType == PACKET_TELDATA)
				{
					enterTelDataCS();
					memcpy(telData, payload, TEL_DATA_BLOCK_SIZE);
					leaveTelDataCS();

					postMessage(MSG_TELDATACHANGED);
				}
			}
		}
		if (result == 0) // graceful disconnect
		{
			postMessage(MSG_DISCONNECTED);
			break;
		}
		if (result < 0) // error
		{
			int error = WSAGetLastError();
			if (error == WSAESHUTDOWN || error == WSAECONNRESET)
			{
				postMessage(MSG_DISCONNECTED);
				break;
			}
			postMessage(MSG_SHUTDOWN, SHR_CONNECTFAIL);
			return 3;
		}
	}

	return 0;
}