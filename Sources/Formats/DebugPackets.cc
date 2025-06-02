#include "../p_MODE7.h"

#include <MODE7/Internal/DebugPackets.hh>

using namespace MODE7;
using namespace MODE7::Internal;

DebugPacketBuffer::DebugPacketBuffer(Packet::Format format, Destination *destination): T45PacketBuffer(format, destination)
{
	DPRINTF(">>> new DebugPacketBuffer created for destination");
}

DebugPacketBuffer::~DebugPacketBuffer()
{
	DPRINTF("<<< DebugPacketBuffer destroyed");
}
