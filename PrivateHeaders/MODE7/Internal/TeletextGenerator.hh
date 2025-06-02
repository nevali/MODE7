#ifndef MODE7_INTERNAL_TELETEXTGENERATOR_HH_
# define MODE7_INTERNAL_TELETEXTGENERATOR_HH_ 202505L

# include <MODE7/Generator.hh>

namespace MODE7
{
	namespace Internal
	{
		/* base class for both serial and parallel Teletext generators */
		class TeletextGenerator: public Generator
		{
			/* Constructors and destructor */
			protected:
				TeletextGenerator(Carousel *carousel, PacketBuffer *packetBuffer);
			public:
				virtual ~TeletextGenerator();
			/* Instance methods */
			protected:
				virtual void generate(void);
				virtual void generateFiller(Magazine *magazine);
				virtual void populateHeader(Packet *packet, Page *page);
				virtual void populateTextRow(Packet *packet, Frame *frame, uint8_t row);
				virtual Packet *generateNextRow(Cursor *cursor, uint8_t row);
				virtual void performHeaderSubstitutions(uint8_t *buffer, size_t bufsize, Page *page);
		};
	}
}

#endif /*!MODE7_INTERNAL_TELETEXTGENERATOR_HH_*/
