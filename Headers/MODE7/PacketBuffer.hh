#ifndef MODE7_PACKETBUFFER_HH_
# define MODE7_PACKETBUFFER_HH_        202506L

# include <MODE7/Object.hh>
# include <MODE7/Packet.hh>

namespace MODE7
{
	class Destination;
	class Frame;

	/* a PacketBuffer manages the entire lifecycle of Packets: it is 
	 * responsible for deciding when Packets should be created, reused, or
	 * destroyed, and does so without explicit interaction from the Generator
	 * using it.
	 *
	 * exactly one Generator can use a PacketBuffer at any one time, there's no
	 * reentrancy support whatsoever: if you need to write two independent sets
	 * of packets, create two independent PacketBuffers.
	 *
	 * PacketBuffers are the only mechanism for obtaining and delivering
	 * packets, they cannot be created independently.
	 *
	 * although in principle the PacketBuffer and Packet classes are protocol-
	 * neutral, they are designed around the structure of the Carousel family
	 * of classes and so intrinsically geared towards delivering Teletext-style
	 * frames row by row.
	 */
	class PacketBuffer: public Object
	{
		/* Class methods */
		public:
			/* Obtain a new PacketBuffer for a given Packet format (T45, T42, etc.) */
			static PacketBuffer *bufferForFormat(Packet::Format format, Destination *destination);
		
		/* Constructor and destructor */
		protected:
			PacketBuffer(Packet::Format format, Destination *destination);
			virtual ~PacketBuffer();
		
		/* Instance methods */
		public:
			/* Obtain an empty header packet for a frame */
			virtual Packet *headerPacketForFrame(Frame *frame) = 0;
			/* Obtain an empty text packet for a row of a frame */
			virtual Packet *packetForTextRow(Frame *frame, uint8_t row) = 0;
			/* Obtain a filler packet */
			virtual Packet *fillerPacket(Magazine *magazine) = 0;	

		/* Instance data */
		private:
			Packet::Format format_;
			Destination *destination_;

		/* Inline accessors */
		public:
			inline Packet::Format format(void) const { return format_; }
			inline Destination *destination(void) const { return destination_; }
	};
}

#endif /*!MODE7_PACKETBUFFER_HH_*/
