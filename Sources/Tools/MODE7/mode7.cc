#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <MODE7/MODE7.h>
#include <MODE7/Tools/Context.hh>

static MODE7::Tools::Context *context;

static void
sigINT(int signo)
{
	(void) signo;

	context->generator->stop();
	MODE7::Clock::sharedClock()->stop();
}

int
main(int argc, char **argv)
{
	context = new MODE7::Tools::Context(argc, argv, "Teletext virtual terminal", "[OPTIONS] -- PROGNAME [ARGUMENTS...]");
	context->processOptions(&argc, &argv);
	if(argc < 1)
	{
		context->usage();
		delete context;
		exit(EXIT_FAILURE);
	}
	/* Create a carousel to hold the magazines, pages, and frames (sub-pages) */
	context->defaultTeletextCarousel();
	context->carousel->setTeletextHeader(">MODE7  %%#  %%a %e %%b %H:%M/%S");
	
	/* Create a processor to write the output of the subprocess into the frame */
	context->defaultProcessor();

	/* Create a source to generate the input to the processor */
	context->source = MODE7::Source::subProcessSource(context->processor, argc, argv);
	
	/* Create a destination to encapsulate the stream that will receive
	 * the packets
	 */
	context->defaultDestination();
	
	/* Create a packet buffer attached to the destination for our desired
	 * output format
	 */
	context->defaultPacketBuffer();
	
	/* Create the generator to turn the carousel into packets using the buffer */
	context->defaultTeletextGenerator();

	/* Run the generator until interrupted by Ctrl+C */
	signal(SIGINT, sigINT);
	context->generator->runUntilStopped();
	signal(SIGINT, SIG_DFL);
	
	delete context;
	return 0;
}
