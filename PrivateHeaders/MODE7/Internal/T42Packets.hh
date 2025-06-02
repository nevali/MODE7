#ifndef MODE7_INTERNAL_T42PACKETS_HH_
# define MODE7_INTERNAL_T42PACKETS_HH_ 202505L

# include "T45Packets.hh"

namespace MODE7
{
	namespace Internal
	{
		class T42PacketBuffer;
		
		/* T42 packets are identical to T45 packets but omit the first three
		 * (timing sync) octets
		 *
		 * this is the format used by raspi-teletext
		 */
		class T42Packet: public T45Packet
		{
			friend class T42PacketBuffer;

			/* Constructors and destructor */
			protected:
				T42Packet(T42PacketBuffer *packetBuffer);
			public:
				virtual ~T42Packet();
			public:
				virtual /*override*/ const uint8_t *buffer(void) const;
				virtual /*override*/ size_t size(void) const;
		};

		class T42PacketBuffer: public T45PacketBuffer
		{
			public:
				T42PacketBuffer(Packet::Format format, Destination *destination);
				virtual ~T42PacketBuffer();
			protected:
				virtual /*override*/ T45Packet *createPacket(void);
		};
	}
}

#endif /*!MODE7_INTERNAL_T42PACKETS_HH_*/
