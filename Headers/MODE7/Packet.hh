#ifndef MODE7_PACKET_HH_
# define MODE7_PACKET_HH_              202506L

# include <MODE7/Object.hh>

namespace MODE7
{
	/* a Packet represents a single row of output in the chosen output format
	 *
	 * a Packet is intended to be reusable
	 */
	class Packet: public Object
	{
		/* Class types */
		public:
			/* Packet formats */
			typedef enum
			{
				T45, /* 45-byte raw packets*/
				Raw = T45,
				T42, /* T42 packets */
				Debug  /* Debug packet generator useful for sending to stdout or a file */
			} Format;

		/* Constructors and destructor */
		protected:
			Packet();
			virtual ~Packet();

		/* Instance methods */
		public:
			virtual void setText(const char *text);        /* set the text portion of the packet to given string */
			virtual void setTextOctets(const uint8_t *text, size_t count) = 0; /* set the text portion of the packet to the given octets */
			virtual void deliver(void) = 0;                /* send the packet to its destination */
			virtual const uint8_t *buffer(void) const = 0; /* return a pointer to the actual packet */
			virtual size_t size(void) const = 0;           /* return the size of the packet */
		protected:
			virtual void update(void) = 0;                 /* update checksums, etc. */
	};
}

#endif /*!MODE7_PACKET_HH_*/