#include "../p_MODE7.h"

#include <MODE7/Internal/T45Packets.hh>
#include <MODE7/Internal/T42Packets.hh>
#include <MODE7/Internal/DebugPackets.hh>

using namespace MODE7;

/* Class methods */

PacketBuffer *
PacketBuffer::bufferForFormat(Packet::Format format, Destination *destination)
{
	switch(format)
	{
		case Packet::T45:
			return new MODE7::Internal::T45PacketBuffer(format, destination);
		case Packet::T42:
			return new MODE7::Internal::T42PacketBuffer(format, destination);
		case Packet::Debug:
			return new MODE7::Internal::DebugPacketBuffer(format, destination);
	}
	abort();
	return nullptr;
}

/* Constructors and destructor */

PacketBuffer::PacketBuffer(Packet::Format format, Destination *destination):
	format_(format), destination_(destination)
{
}

PacketBuffer::~PacketBuffer()
{
}
