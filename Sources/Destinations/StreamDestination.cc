#include "../p_MODE7.h"

#include <MODE7/Internal/StreamDestination.hh>

using namespace MODE7;
using namespace MODE7::Internal;

StreamDestination::StreamDestination(FILE *stream): Destination(),
	stream_(stream)
{
}

StreamDestination::~StreamDestination()
{
}

bool
StreamDestination::write(Packet *packet)
{
	if(fwrite(packet->buffer(), packet->size(), 1, stream_) != 1)
	{
		fprintf(stderr, "Error: failed to write packet to stream(): %s\n", strerror(errno));
		return false;
	}
	fflush(stream_);
	return true;
}