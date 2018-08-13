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

/* networking modules */
#include <winsock2.h>
#include <ws2tcpip.h>

/* other includes */
#include <windows.h>
#include "resource.h"
#include <CommCtrl.h>

#include <stdio.h>
#include <stdlib.h>

/* my includes */
#include "NetPackets.h"
#include "ETS2_Telemetry.h"

/* constants */
#define TEXT_BUF_SIZE 255
#define DEFAULT_PORT 30055
#define RECV_BUFFER_SIZE (4 * 1024)
#define PACKET_BUFFER_SIZE (32 * 1024)
#define PACKET_PAYLOAD_BUFFER_SIZE (32 * 1024)

/* custom messages */
#define MSG_TELDATACHANGED    (WM_APP + 1)
#define MSG_SHUTDOWN          (WM_APP + 2)
#define MSG_CONNECTED         (WM_APP + 3)
#define MSG_DISCONNECTED      (WM_APP + 4)
#define MSG_NAMESET           (WM_APP + 5)

/* shutdown reasons */
#define SHR_CONNECTFAIL 1

/* packets */
#define PACKET_NAME 1
#define PACKET_TELDATA 2

/* Win32 stuff */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int);
BOOL CALLBACK MainWndProc(HWND, UINT, WPARAM, LPARAM);

/* variables */
HWND hWindow;
bool shuttingDown;
HANDLE thread;
SOCKET clientSocket;
void* telData; // local copy of telemetry data
CRITICAL_SECTION telDataCS;

/* main methods */
void onCreate();
void onDestroy();
void connect();
void disconnect();
void displayTelData(TETS2_Telemetry* data);
void setName();

/* events */
void onMsgTelDataChanged();
void onMsgShutdown(int reason);
void onMsgConnect();
void onMsgDisconnect();

/* helper methods */
void enterTelDataCS();
void leaveTelDataCS();
void postMessage(UINT message, int arg = 0);

/* network related methods */
bool initializeWinsock();
void terminateWinsock();

/* UI methods */
void setStatus(const char* status);
void errorBox(const char* caption, const char* message);
void toggleControl(int control, bool enable);

/* threads */
DWORD WINAPI clientThread(LPVOID lpParameter);