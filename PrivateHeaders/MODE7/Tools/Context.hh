#ifndef MODE7_TOOLS_CONTEXT_HH_
# define MODE7_TOOLS_CONTEXT_HH_

# include <MODE7/Carousel.hh>
# include <MODE7/Source.hh>
# include <MODE7/Destination.hh>
# include <MODE7/Packet.hh>
# include <MODE7/PacketBuffer.hh>
# include <MODE7/Generator.hh>
# include <MODE7/Processor.hh>

namespace MODE7
{
	namespace Tools
	{
		struct Context
		{
			Context(int argc, char **argv, const char *description, const char *options = "[OPTIONS]");
			~Context();

			/* Common option processing */
			void usage(void);
			void processOptions(int *argc, char ***argv);
			void optOutputPath(const char *arg);
			void optOutputCommandPipe(const char *arg);

			/* Default initialisers */
			void defaultTeletextCarousel(void);
			void defaultDestination(void);
			void defaultPacketBuffer(void);
			void defaultProcessor(void);
			void defaultTeletextGenerator(void);

			/* Context data */
			const char *progname;
			Packet::Format packetFormat;
			Processor::Kind processorKind;
			Carousel *carousel;
			Source *source;
			Destination *destination;
			PacketBuffer *packetBuffer;
			Processor *processor;
			Generator *generator;
			int argc;
			char **argv;
			const char *description;
			const char *options;
		};
	}
}

#endif /*!TOOLS_CONTEXT_HH_*/
