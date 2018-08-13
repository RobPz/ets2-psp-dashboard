#pragma once

/* for enabling visual styles */
#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")
#pragma comment(lib, "ComCtl32.lib")

/* Winsock library */
#pragma comment(lib, "Ws2_32.lib")

/* no need for unicode */
#undef UNICODE

/* for testing without ETS2 open */
//#define NO_GAME

/* networking modules */
#include <winsock2.h>
#include <ws2tcpip.h>

/* other includes */
#include <windows.h>
#include "resource.h"
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <conio.h>
#include <CommCtrl.h>

/* my includes */
#include "clients.h"
#include "NetPackets.h"
#include "ETS2_Telemetry.h"

/* constants */
#define TEXT_BUF_SIZE 255
#define DEFAULT_SLEEP_INTERVAL 33 // 30 times a second
#define MIN_SLEEP_INTERVAL 16 // 60 times a second
#define MAX_CONNECTIONS 4
#define THREAD_COUNT 2
#define DEFAULT_PORT 30055
#define RECV_BUFFER_SIZE (4 * 1024)
#define PACKET_BUFFER_SIZE (32 * 1024)
#define PACKET_PAYLOAD_BUFFER_SIZE (32 * 1024)

/* custom messages */
#define MSG_TELDATACHANGED    (WM_APP + 1)
#define MSG_SHUTDOWN          (WM_APP + 2)
#define MSG_CLIENTCONNECT     (WM_APP + 3)
#define MSG_CLIENTDISCONNECT  (WM_APP + 4)
#define MSG_CLIENTNAMESET     (WM_APP + 5)

/* shutdown reasons */
#define SHR_READFAIL 1
#define SHR_ACCEPTFAIL 2

/* packets */
#define PACKET_NAME 1
#define PACKET_TELDATA 2

/* Win32 stuff */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int);
BOOL CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

/* variables */
HWND hWindow;
HANDLE hMapFile;
LPCTSTR pMapFile;
bool shuttingDown, running;
void* telData; // local copy of telemetry data
CRITICAL_SECTION telDataCS;
HANDLE threads[THREAD_COUNT];
SOCKET serverSocket;
TClients* clients;
DWORD telThreadSleepInterval;

/* main methods */
void onCreate();
void onDestroy();
void startServer();
void stopServer();
void displayTelData(TETS2_Telemetry* data);

/* events */
void onMsgTelDataChanged();
void onMsgShutdown(int reason);
void onMsgClientConnect(SOCKET socket);
void onMsgClientDisconnect(SOCKET socket);
void onMsgClientNameSet(SOCKET socket);

/* helper methods */
void resetThreadHandles(bool closeHandles = false);
void enterTelDataCS();
void leaveTelDataCS();
void postMessage(UINT message, int arg = 0);
void waitForThreads();

/* list control methods */
void clearListBox();
void addListItem(SOCKET socket);
void removeListItem(SOCKET socket); 
int getListItem(SOCKET socket);
void setListItem(size_t row, size_t column, char* value);

/* network related methods */
bool initializeWinsock();
void terminateWinsock();
void sendTelemetryData(SOCKET client);

/* UI methods */
void setStatus(const char* status);
void errorBox(const char* caption, const char* message);
void toggleControl(int control, bool enable);
void toggleConnectControls(bool enable);

/* threads */
DWORD WINAPI telemetryDataThread(LPVOID lpParameter);
DWORD WINAPI serverAcceptThread(LPVOID lpParameter);
DWORD WINAPI clientThread(LPVOID lpParameter);