#include "../p_MODE7.h"

#include <MODE7/Internal/FileDestination.hh>

using namespace MODE7;
using namespace MODE7::Internal;

FileDestination::FileDestination(FILE *file): StreamDestination(file)
{
}

FileDestination::FileDestination(const char *pathname): StreamDestination(nullptr)
{
	stream_ = fopen(pathname, "wb+");
	if(!stream_)
	{
		fprintf(stderr, "FileDestination: failed to create pipe: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
}

FileDestination::~FileDestination()
{
	if(stream_)
	{
		fclose(stream_);
		stream_ = nullptr;
	}
}
