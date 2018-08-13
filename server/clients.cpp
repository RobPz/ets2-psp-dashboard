#include "clients.h"

TClient::TClient()
{
	isTaken = false;
}

TClient::TClient(const TClient &obj)
{
	isTaken = obj.isTaken;
	socket = obj.socket;
	thread = obj.thread;
	for (size_t i = 0; i < INET_ADDRSTRLEN; i++)
		IP[i] = obj.IP[i];
	for (size_t i = 0; i < CLIENT_NAME_LEN; i++)
		name[i] = obj.name[i];
}

int TClients::mFind(SOCKET socket)
{
	for (size_t i = 0; i < mSize; i++)
		if (mClients[i].socket == socket)
			return (int)i;
	return -1;
}

void TClients::mEnterCS()
{
	EnterCriticalSection(&mCS);
}

void TClients::mLeaveCS()
{
	LeaveCriticalSection(&mCS);
}

TClients::TClients(size_t maxClientCount, HWND window, UINT clientConnectMsg, UINT clientDisconnectMsg, UINT clientNameSetMsg)
{
	mSize = maxClientCount;
	mWindow = window;
	mClientConnectMsg = clientConnectMsg;
	mClientDisconnectMsg = clientDisconnectMsg;
	mClientNameSetMsg = clientNameSetMsg;
	mClients = new TClient[mSize];
	/*for (size_t i = 0; i < mSize; i++)
		mClients[i].isTaken = false;*/
	InitializeCriticalSection(&mCS);
}

TClients::~TClients()
{
	DeleteCriticalSection(&mCS);
	delete[] mClients;
}

size_t TClients::count()
{
	size_t result = 0;
	mEnterCS();
		for (size_t i = 0; i < mSize; i++)
			if (mClients[i].isTaken)
				result++;
	mLeaveCS();
	return result;
}

bool TClients::add(TClient client)
{
	bool result = false;
	mEnterCS();
		for (size_t i = 0; i < mSize; i++)
			if (!mClients[i].isTaken)
			{
				mClients[i] = TClient(client);
				mClients[i].isTaken = true;
				result = true;
				PostMessage(mWindow, mClientConnectMsg, (WPARAM)mClients[i].socket, NULL);
				break;
			}
	mLeaveCS();
	return result;
}

void TClients::remove(SOCKET socket)
{
	mEnterCS();
		int index = mFind(socket);
		if (index >= 0)
		{
			mClients[index].isTaken = false;
			PostMessage(mWindow, mClientDisconnectMsg, (WPARAM)mClients[index].socket, NULL);
		}		
	mLeaveCS();
}

TClient* TClients::get(SOCKET socket)
{
	TClient* result = nullptr;
	mEnterCS();
		int index = mFind(socket);
		if (index >= 0)
			result = &mClients[index];
	mLeaveCS();
	return result;
}

bool TClients::setName(SOCKET socket, string newName)
{
	bool result = false;
	mEnterCS();
		int index = mFind(socket);
		if (index >= 0)
		{
			strcpy_s(mClients[index].name, newName.length() + 1, newName.c_str());
			PostMessage(mWindow, mClientNameSetMsg, (WPARAM)mClients[index].socket, NULL);
			result = true;
		}
	mLeaveCS();
	return result;
}

void TClients::disconnect()
{
	mEnterCS();
	for (size_t i = 0; i < mSize; i++)
		if (mClients[i].isTaken)
		{
			::shutdown(mClients[i].socket, SD_BOTH);
			closesocket(mClients[i].socket);
		}
	mLeaveCS();
}

size_t TClients::getSockets(SOCKET sockets[])
{
	size_t result = 0;
	mEnterCS();
	for (size_t i = 0; i < mSize; i++)
		if (mClients[i].isTaken)
		{
			sockets[result] = mClients[i].socket;
			result++;
		}
	mLeaveCS();
	return result;
}

size_t TClients::getThreads(HANDLE threads[])
{
	size_t result = 0;
	mEnterCS();
	for (size_t i = 0; i < mSize; i++)
		if (mClients[i].isTaken)
		{
			threads[result] = mClients[i].thread;
			result++;
		}
	mLeaveCS();
	return result;
}