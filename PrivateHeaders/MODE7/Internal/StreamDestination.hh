#ifndef MODE7_INTERNAL_STREAMDESTINATION_HH_
# define MODE7_INTERNAL_STREAMDESTINATION_HH_ 202505L

# include <MODE7/Destination.hh>

namespace MODE7
{
	namespace Internal
	{
		class StreamDestination: public MODE7::Destination
		{
			/* Constructors and destructor */
			public:
				StreamDestination(FILE *stream);
				virtual ~StreamDestination();
			
			/* Instance methods */
			public:
				virtual bool write(Packet *packet);
			
			/* Instance data */
			protected:
				FILE *stream_;
		};
	}
}

#endif /*!MODE7_INTERNAL_STREAMDESTINATION_HH_*/
