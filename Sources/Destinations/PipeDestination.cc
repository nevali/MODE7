#include "../p_MODE7.h"

#include <MODE7/Internal/PipeDestination.hh>

using namespace MODE7;
using namespace MODE7::Internal;

PipeDestination::PipeDestination(FILE *stream): StreamDestination(stream)
{
}

PipeDestination::PipeDestination(const char *command): StreamDestination(nullptr)
{
	stream_ = popen(command, "w");
	if(!stream_)
	{
		fprintf(stderr, "PipeDestination: failed to create pipe: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
}

PipeDestination::~PipeDestination()
{
	if(stream_)
	{
		DPRINTF("<<< PipeDestination: closing stream");
		pclose(stream_);
		stream_ = nullptr;
	}
}