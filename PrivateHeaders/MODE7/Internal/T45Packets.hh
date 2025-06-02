#ifndef MODE7_INTERNAL_T45PACKETS_HH_
# define MODE7_INTERNAL_T45PACKETS_HH_ 202505L

# include <MODE7/Packet.hh>
# include <MODE7/PacketBuffer.hh>

namespace MODE7
{
	namespace Internal
	{
		/* the classes implement the full 45-octet Teletext packet and packet
		 * buffer logic
		 *
		 * T42Packet and DebugPacket are subclasses which alter the output
		 * format
		 */
		
		class T45PacketBuffer;

		class T45Packet: public Packet
		{
			friend class T45PacketBuffer;

			/* Constructors and destructor */
			protected:
				T45Packet(T45PacketBuffer *packetBuffer);
			public:
				virtual ~T45Packet();
			/* Instance methods */
			public:
				virtual /*override*/ void setTextOctets(const uint8_t *octets, size_t count);
				virtual /*override*/ void deliver(void);

				virtual /*override*/ const uint8_t *buffer(void) const;
				virtual /*override*/ size_t size(void) const;
			protected:
				virtual /*override*/ void update(void);

			/* Internal instance methods -- callable only by T45PacketBuffer */
			protected:
				/* these methods reset the packet state ready for it to be
				 * reused
				 */
				void reset(void);
				void resetToHeader(Frame *frame);
				void resetToTextRow(Frame *frame, uint8_t row);
				void resetToFiller(uint8_t mag);
				void updateFraming(void);
				void updateParity(size_t offset);
				void updateM0(void);
				void updateText(void);
			/* Instance data */
			protected:
				T45PacketBuffer *packetBuffer_;
				uint8_t row_;
				uint8_t mag_;
				uint8_t pageNumber_;
				uint16_t subcode_;
				Magazine *magazine_;
				Page *page_;
				Frame *frame_;
				uint8_t buffer_[MODE7_PACKETSIZE + 8]; /* safety net */
		};

		class T45PacketBuffer: public PacketBuffer
		{
			public:
				T45PacketBuffer(Packet::Format format, Destination *destination);
				virtual ~T45PacketBuffer();
				
			/* Instance methods */
			public:
				/* Obtain an empty header packet for a page */
				virtual /*override*/ Packet *headerPacketForFrame(Frame *frame);
				/* Obtain an empty text packet for a row of a frame */
				virtual /*override*/ Packet *packetForTextRow(Frame *frame, uint8_t row);
				/* Obtain a filler packet for a magazine */
				virtual /*override*/ Packet *fillerPacket(Magazine *magazine);
				
			/* Internal instance methods */
			public:
				virtual void deliver(T45Packet *packet);

			protected:
				virtual T45Packet *createPacket(void);
			/* Instance data */
			private:
				T45Packet *packet_;
		};
	}
}

#endif /*!MODE7_INTERNAL_T45PACKETS_HH_*/
