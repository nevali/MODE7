#include "../p_MODE7.h"

#include <MODE7/Internal/SubProcessSource.hh>

using namespace MODE7;

Source *
Source::subProcessSource(Processor *processor, int argc, char **argv)
{
	return new MODE7::Internal::SubProcessSource(processor, argc, argv);
}

Source::Source(Processor *processor):
	processor_(processor), carousel_(processor->carousel())
{
	Clock::sharedClock()->addSource(this);
	DPRINTF(">>> Source %p created", this);
}

Source::~Source()
{
	Clock::sharedClock()->removeSource(this);
	DPRINTF(">>> Source %p destroyed", this);
}

bool
Source::processPendingEvents(void)
{
	DPRINTF("*** placeholder Source::processPendingEvents() invoked");
	return false;
}
