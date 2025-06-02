#include <cstdlib>
#include <csignal>

#include <unistd.h>

#include <MODE7/MODE7.h>

#include <MODE7/Tools/Context.hh>
#include <MODE7/Tools/DemoSource.hh>

static void sigINT(int signo)
{
	(void) signo;

	MODE7::Clock::sharedClock()->stop();
}

int
main(int argc, char **argv)
{
	MODE7::Tools::Context context(argc, argv, "Teletext output generator");
	context.processOptions(&argc, &argv);
	if(argc > 0)
	{
		context.usage();
		exit(EXIT_FAILURE);
	}
	/* Create a carousel to hold the magazines, pages, and frames (sub-pages) */
	context.defaultTeletextCarousel();
	
	/* Create a processor to write the source's output into the frame */
	context.defaultProcessor();
	
	/* Create a source to generate the frame data */
	MODE7::Tools::DemoSource *source = new MODE7::Tools::DemoSource(context.processor);
	context.source = source;
	source->generateDemoFrame();
	source->generateInfoFrame();

	/* Create a destination to encapsulate the stream that will receive
	 * the packets
	 */
	context.defaultDestination();
	
	/* Create a packet buffer attached to the destination for our desired
	 * output format
	 */
	context.defaultPacketBuffer();
	
	/* Create the generator to turn the carousel into packets using the buffer */
	context.defaultTeletextGenerator();

	/* Run the generator */
	signal(SIGINT, sigINT);
	context.generator->runUntilStopped();
	signal(SIGINT, SIG_DFL);
	
	return 0;
}
