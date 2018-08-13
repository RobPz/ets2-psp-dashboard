#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>

#define CLIENT_NAME_LEN 30

using std::string;

struct TClient
{
	bool isTaken;
	SOCKET socket;
	HANDLE thread;
	char IP[INET_ADDRSTRLEN];
	char name[CLIENT_NAME_LEN];
	TClient();
	TClient(const TClient &obj);
};

class TClients
{
private:
	CRITICAL_SECTION mCS;
	TClient* mClients;
	size_t mSize;
	HWND mWindow;
	UINT mClientConnectMsg; 
	UINT mClientDisconnectMsg;
	UINT mClientNameSetMsg;
	int mFind(SOCKET socket);
	void mEnterCS();
	void mLeaveCS();
public:
	TClients(size_t maxClientCount, HWND window, UINT clientConnectMsg, UINT clientDisconnectMsg, UINT clientNameSetMsg);
	~TClients();

	size_t count();
	bool add(TClient client);
	void remove(SOCKET socket);
	TClient* get(SOCKET socket);
	bool setName(SOCKET socket, string newName);
	void disconnect();
	size_t getSockets(SOCKET sockets[]);
	size_t getThreads(HANDLE threads[]);
};