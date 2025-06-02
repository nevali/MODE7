#ifndef MODE7_INTERNAL_SERIALTELETEXTGENERATOR_HH_
# define MODE7_INTERNAL_SERIALTELETEXTGENERATOR_HH_ 202505L

# include "TeletextGenerator.hh"

namespace MODE7
{
	namespace Internal
	{
		class SerialTeletextGenerator: public TeletextGenerator
		{
			/* Constructors and destructor */
			public:
				SerialTeletextGenerator(Carousel *carousel, PacketBuffer *packetBuffer);
				virtual ~SerialTeletextGenerator();
			
			/* Instance methods */
			public:
				virtual void generate(void); /* send a single packet */
			protected:
				void nextRow(void);
				void resetRow(void);
			private:
				/* the serial generator uses a single cursor that steps
				 * sequentially through the magazine, sending the active frame
				 * from each page, row by row
				 */
				Cursor *cursor_;
				uint8_t row_;
		};
	}
}

#endif /*!MODE7_INTERNAL_SERIALTELETEXTGENERATOR_HH_*/
