#include "NetPackets.h"

TPacket::TPacket()
{
	packetType = 0;
	payloadSize = 0;
}

TPacket::TPacket(const TPacket &obj)
{
	packetType = obj.packetType;
	payloadSize = obj.payloadSize;
}

TPacketBuffer::TPacketBuffer(size_t bufferSize)
{
	mBufferSize = bufferSize;
	mBuffer = malloc(mBufferSize);
	mDataOffset = 0;
	mDataSize = 0;
}

TPacketBuffer::~TPacketBuffer()
{
	free(mBuffer);
}

bool TPacketBuffer::write(void* data, size_t dataSize)
{
	if (mDataSize + dataSize > mBufferSize) // buffer overflow	
		return false;

	if (mDataOffset + mDataSize + dataSize > mBufferSize) // need to shift the data in the buffer to the beginning
	{
		if (mDataOffset >= mDataSize)
		{
			memcpy(mBuffer, (void*)((char*)mBuffer + mDataOffset), mDataSize);
			mDataOffset = 0;
		}
		else
		{
			size_t blockSize = mDataOffset;
			size_t copyCount = mDataSize / blockSize;
			size_t tail = mDataSize % blockSize;
			size_t offset1 = 0, offset2 = mDataOffset;
			while (copyCount > 0)
			{
				memcpy((void*)((char*)mBuffer + offset1), (void*)((char*)mBuffer + offset2), blockSize);
				offset1 += blockSize;
				offset2 += blockSize;
			}
			if (tail > 0)
				memcpy((void*)((char*)mBuffer + offset1), (void*)((char*)mBuffer + offset2), tail);
			mDataOffset = 0;
		}
	}

	// write to the buffer
	memcpy((void*)((char*)mBuffer + mDataOffset + mDataSize), data, dataSize);
	mDataSize += dataSize;

	return true;
}

bool TPacketBuffer::getPacket(TPacket &packet, void* payload, size_t bufferSize)
{
	if (mDataSize == 0) // nothing to read
		return false;

	if (mDataOffset + PACKET_SIZE > mDataSize) // incomplete packet
		return false;

	// packet
	memcpy(&packet, (void*)((char*)mBuffer + mDataOffset), PACKET_SIZE);

	// payload
	if (mDataOffset + PACKET_SIZE + packet.payloadSize > mDataSize) // incomplete payload
		return false;

	if (payload == NULL)
	{
		payload = malloc(packet.payloadSize);
	}
	else
	{
		if (bufferSize < packet.payloadSize) // buffer is too small
			return false;
	}

	memcpy(payload, (void*)((char*)mBuffer + mDataOffset + PACKET_SIZE), packet.payloadSize);

	mDataSize -= PACKET_SIZE + packet.payloadSize;
	if (mDataSize == 0)
		mDataOffset = 0;
	else
		mDataOffset += PACKET_SIZE + packet.payloadSize;

	return true;
}