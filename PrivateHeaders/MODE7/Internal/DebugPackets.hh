#ifndef MODE7_INTERNAL_DEBUGPACKETS_HH_
# define MODE7_INTERNAL_DEBUGPACKETS_HH_ 202505L

# include "T45Packets.hh"

namespace MODE7
{
	namespace Internal
	{
		/* debug packets are identical to T45 packets but their output is
		 * a dump of the packet content in human-readable form
		 */
		class DebugPacket: public T45Packet
		{

		};

		class DebugPacketBuffer: public T45PacketBuffer
		{
			public:
				DebugPacketBuffer(Packet::Format format, Destination *destination);
				virtual ~DebugPacketBuffer();
		};
	}
}

#endif /*!MODE7_INTERNAL_DEBUGPACKETS_HH_*/
