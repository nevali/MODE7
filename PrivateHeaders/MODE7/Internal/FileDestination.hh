#ifndef MODE7_INTERNAL_FILEDESTINATION_HH_
# define MODE7_INTERNAL_FILEDESTINATION_HH_ 202505L

# include "StreamDestination.hh"

namespace MODE7
{
	namespace Internal
	{
		class FileDestination: public StreamDestination
		{
			/* Constructors and destructor */
			public:
				FileDestination(FILE *file);
				FileDestination(const char *pathname);
				virtual ~FileDestination();
		};
	}
}

#endif /*!MODE7_INTERNAL_FILEDESTINATION_HH_*/
