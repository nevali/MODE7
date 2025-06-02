#include "../p_MODE7.h"

/* Generators */
#include <MODE7/Internal/SerialTeletextGenerator.hh>
#include <MODE7/Internal/ParallelTeletextGenerator.hh>

using namespace MODE7;

/* Class methods */

Generator *
Generator::serialTeletextGeneratorFor(Carousel *carousel, PacketBuffer *packetBuffer)
{
	return new MODE7::Internal::SerialTeletextGenerator(carousel, packetBuffer);
}

Generator *
Generator::parallelTeletextGeneratorFor(Carousel *carousel, PacketBuffer *packetBuffer)
{
	return new MODE7::Internal::ParallelTeletextGenerator(carousel, packetBuffer);
}

/* Constructors and destructor */

Generator::Generator(Carousel *carousel, PacketBuffer *packetBuffer):
	carousel_(carousel), packetBuffer_(packetBuffer), running_(false), stop_(false)
{
	clock_ = Clock::sharedClock();
}

Generator::~Generator()
{
}

/* Instance methods */

void
Generator::runUntilStopped(void)
{
	stop_ = false;

	DPRINTF("+++ Generator is running");

	while(!shouldStop())
	{
		if(!processPendingEvents())
		{
			break;
		}
	}
	DPRINTF("+++ Generator stopped");
}

void
Generator::stop(void)
{
	stop_ = true;
}

bool
Generator::processPendingEvents(void)
{
	bool wasRunning = running_;
	
	running_ = true;

	if(!clock_->tick())
	{
		running_ = wasRunning;
		return false;
	}
	carousel_->refresh();
	generate();

	running_ = wasRunning;

	return true;
}
