#pragma once

#include <stdlib.h>
#include <inttypes.h>
#include <cstring>

typedef unsigned int PacketType;

struct TPacket
{
	uint32_t packetType;
	uint16_t payloadSize;
	TPacket();
	TPacket(const TPacket &obj);
};

#define PACKET_SIZE sizeof(TPacket)

class TPacketBuffer
{
private:
	void* mBuffer;
	size_t mBufferSize;
	size_t mDataOffset, mDataSize;
public:
	TPacketBuffer(size_t bufferSize);
	~TPacketBuffer();

	bool write(void* data, size_t dataSize);
	bool getPacket(TPacket &packet, void* payload, size_t bufferSize = 0);
};