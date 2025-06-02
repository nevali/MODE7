#ifndef MODE7_INTERNAL_TELETEXTPROCESSOR_HH_
# define MODE7_INTERNAL_TELETEXTPROCESSOR_HH_ 202506L

# include <MODE7/Internal/RawProcessor.hh>

namespace MODE7
{
	namespace Internal
	{
		/* this is a MODE 7-like Processor class which assumes that local
		 * control codes are in the range 0 >= x >= 31, and Teletext control
		 * codes are in the range 128 >= x >= 159
		 */
		class TeletextProcessor: public RawProcessor
		{
			/* Class methods */
			public:
				static Processor *newProcessorOfKind(Kind kind, Carousel *carousel);

			/* Constructors and destructor */
			protected:
				TeletextProcessor(Kind kind, Carousel *carousel);
				~TeletextProcessor();
			public:
				virtual /*override*/ void putOctet(uint8_t octet);
		};
	}
}

#endif /*!MODE7_INTERNAL_TELETEXTPROCESSOR_HH_*/
