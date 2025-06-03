#include <cassert>
#include <cstring>
#include <cstdlib>
#include <unistd.h>

#include <MODE7/Tools/Context.hh>

using namespace MODE7;
using namespace MODE7::Tools;

Context::Context(int nargs, char **args, const char *desc, const char *opts):
	progname(nullptr), packetFormat(Packet::T42), processorKind(Processor::Teletext),
	carousel(nullptr), source(nullptr), destination(nullptr), packetBuffer(nullptr),
	processor(nullptr), generator(nullptr), argc(nargs), argv(args),
	description(desc), options(opts)
{
	const char *t;

	assert(argc > 0);
	assert(nullptr != argv);
	assert(nullptr != argv[0]);
	
	if(nullptr == (t = strrchr(argv[0], '/')))
	{
		progname = argv[0];
	}
	else
	{
		progname = ++t;
	}
}

Context::~Context()
{
	if(nullptr != generator)
	{
		generator->release();
		generator = nullptr;
	}
	if(nullptr != destination)
	{
		destination->release();
		destination = nullptr;
	}
	if(nullptr != packetBuffer)
	{
		packetBuffer->release();
		packetBuffer = nullptr;
	}
	if(nullptr != source)
	{
		source->release();
		source = nullptr;
	}
	if(nullptr != processor)
	{
		processor->release();
		processor = nullptr;
	}
	if(nullptr != carousel)
	{
		carousel->release();
		carousel = nullptr;
	}
	progname = nullptr;
}

void
Context::usage(void)
{
	fprintf(stderr,
		"Usage: %s %s\n\n"
		"%s\n\n"
		"OPTIONS may be:\n"
		"  -h           Display this message and exit\n"
		"  -o FILE      Write packets to FILE instead of standard output\n"
		"  -p COMMAND   Pipe packets through COMMAND\n",
		progname, options, description);
}

void
Context::processOptions(int *rargc, char ***rargv)
{
	int ch;

	while((ch = getopt(argc, argv, "ho:p:")) != -1)
	{
		switch(ch)
		{
			case 'h':
				usage();
				exit(EXIT_SUCCESS);
			case 'o':
				optOutputPath(optarg);
				break;
			case 'p':
				optOutputCommandPipe(optarg);
				break;
			default:
				usage();
				exit(EXIT_FAILURE);
		}
	}
	*rargc = argc - optind;
	*rargv = argv + optind;
}

void
Context::optOutputPath(const char *arg)
{
	if(nullptr != destination)
	{
		fprintf(stderr, "%s: cannot specify more than one destination\n", progname);
		exit(EXIT_FAILURE);
	}
	destination = MODE7::Destination::destinationForFileNamed(arg);
	assert(nullptr != destination);
}

void
Context::optOutputCommandPipe(const char *arg)
{
	if(nullptr != destination)
	{
		fprintf(stderr, "%s: cannot specify more than one destination\n", progname);
		exit(EXIT_FAILURE);
	}
	destination = MODE7::Destination::destinationForCommand(arg);
	assert(nullptr != destination);
}

void
Context::defaultDestination(void)
{
	if(nullptr == destination)
	{
		destination = MODE7::Destination::destinationForStream(stdout);
		assert(nullptr != destination);
	}
}

void
Context::defaultProcessor(void)
{
	if(nullptr == processor)
	{
		assert(nullptr != carousel);
		processor = MODE7::Processor::newProcessorOfKind(processorKind, carousel);
		assert(nullptr != processor);
	}
}

void
Context::defaultPacketBuffer(void)
{
	if(nullptr == packetBuffer)
	{
		assert(nullptr != destination);
		packetBuffer = MODE7::PacketBuffer::bufferForFormat(packetFormat, destination);
		assert(nullptr != packetBuffer);
	}
}

void
Context::defaultTeletextCarousel(void)
{
	if(nullptr == carousel)
	{
		carousel = MODE7::Carousel::newCarousel();
		assert(nullptr != carousel);
	}
}

void
Context::defaultTeletextGenerator(void)
{
	if(nullptr == generator)
	{
		assert(nullptr != carousel);
		assert(nullptr != packetBuffer);
		generator = MODE7::Generator::serialTeletextGeneratorFor(carousel, packetBuffer);
		assert(nullptr != generator);
	}
}
