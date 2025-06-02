#include "../p_MODE7.h"

#include <MODE7/Internal/T42Packets.hh>

using namespace MODE7;
using namespace MODE7::Internal;

T42PacketBuffer::T42PacketBuffer(Packet::Format format, Destination *destination): T45PacketBuffer(format, destination)
{
	DPRINTF(">>> new T42PacketBuffer created for destination");
}

T42PacketBuffer::~T42PacketBuffer()
{
	DPRINTF("<<< T42PacketBuffer destroyed");
}

T45Packet *
T42PacketBuffer::createPacket(void)
{
	return new T42Packet(this);
}


T42Packet::T42Packet(T42PacketBuffer *packetBuffer): T45Packet(packetBuffer)
{
}

T42Packet::~T42Packet()
{
}

const uint8_t *
T42Packet::buffer(void) const
{
	return &(buffer_[3]);
}

size_t
T42Packet::size(void) const
{
	return 42;
}
