#ifndef MODE7_DESTINATION_HH_
# define MODE7_DESTINATION_HH_         202506L

# include <cstdio>

# include <MODE7/Object.hh>
# include <MODE7/Packet.hh>

namespace MODE7
{
	class Destination: public Object
	{
		/* Class methods */
		public:
			static Destination *destinationForStream(FILE *stream); /* will not close on destroy */
			static Destination *destinationForFile(FILE *stream); /* will close on destroy */
			static Destination *destinationForFileNamed(const char *pathname);
			static Destination *destinationForCommand(const char *shellcmd);
			static Destination *destinationForPipe(FILE *pipestream); /* calls pclose on destroy */
		
		/* Constructors and destructor */
		protected:
			Destination();
			virtual ~Destination();

		/* Instance methods */
		public:
			virtual bool write(Packet *packet) = 0;
	};
}

#endif /*!MODE7_DESTINATION_HH_*/
