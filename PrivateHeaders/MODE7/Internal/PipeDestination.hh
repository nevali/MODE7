#ifndef MODE7_INTERNAL_PIPEDESTINATION_HH_
# define MODE7_INTERNAL_PIPEDESTINATION_HH_ 202505L

#include "StreamDestination.hh"

namespace MODE7
{
	namespace Internal
	{
		class PipeDestination: public StreamDestination
		{
			/* Constructors and destructor */
			public:
				PipeDestination(const char *command);
				PipeDestination(FILE *pipestream);
				virtual ~PipeDestination();
		};
	}
}

#endif /*!MODE7_INTERNAL_PIPEDESTINATION_HH_*/
