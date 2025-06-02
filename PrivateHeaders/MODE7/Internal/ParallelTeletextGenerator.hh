#ifndef MODE7_INTERNAL_PARALLELTELETEXTGENERATOR_HH_
# define MODE7_INTERNAL_PARALLELTELETEXTGENERATOR_HH_ 202505L

# include "TeletextGenerator.hh"

namespace MODE7
{
	namespace Internal
	{
		class ParallelTeletextGenerator: public TeletextGenerator
		{
			/* Constructors and destructor */
			public:
				ParallelTeletextGenerator(Carousel *carousel, PacketBuffer *packetBuffer);
				virtual ~ParallelTeletextGenerator();
		};
	}
}

#endif /*!MODE7_INTERNAL_PARALLELTELETEXTGENERATOR_HH_*/
