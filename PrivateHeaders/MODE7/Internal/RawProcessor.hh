#ifndef MODE7_INTERNAL_RAWPROCESSOR_HH_
# define MODE7_INTERNAL_RAWPROCESSOR_HH_ 202506L

# include <MODE7/Processor.hh>

namespace MODE7
{
	namespace Internal
	{
		/* this "processor" simply dumps octets into the frame buffer and
		 * advances the cursor, which wraps at both the end of the line
		 * and the page (no scrolling)
		 *
		 * bit 7 is stripped (in raw Teletext it doesn't make a difference)
		 *
		 * characters such as tab, newline, and carriage return will NOT
		 * be processed as per ASCII/ANSI -- 0x0A is "End Box" for example
		 *
		 * for these reasons this is mainly useful as a base class, or when
		 * reading from binary Teletext files
		 */
		class RawProcessor: public Processor
		{
			/* Class methods */
			public:
				static Processor *newProcessorOfKind(Kind kind, Carousel *carousel);

			/* Constructors and destructor */
			protected:
				RawProcessor(Kind kind, Carousel *carousel);
				~RawProcessor();
			public:
				virtual /*override*/ void putOctet(uint8_t octet);
		};
	}
}

#endif /*!MODE7_INTERNAL_RAWPROCESSOR_HH_*/
