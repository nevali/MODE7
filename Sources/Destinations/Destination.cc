#include "../p_MODE7.h"

#include <MODE7/Internal/StreamDestination.hh>
#include <MODE7/Internal/FileDestination.hh>
#include <MODE7/Internal/PipeDestination.hh>

using namespace MODE7;

/* does not close on destroy */
Destination *
Destination::destinationForStream(FILE *stream)
{
	return new MODE7::Internal::StreamDestination(stream);
}

/* calls fclose() on destroy */
Destination *
Destination::destinationForFile(FILE *file)
{
	return new MODE7::Internal::FileDestination(file);
}
Destination *
Destination::destinationForFileNamed(const char *filename)
{
	return new MODE7::Internal::FileDestination(filename);
}

Destination *
Destination::destinationForCommand(const char *command)
{
	return new MODE7::Internal::PipeDestination(command);
}

/* calls pclose() on destroy */
Destination *
Destination::destinationForPipe(FILE *stream)
{
	return new MODE7::Internal::PipeDestination(stream);
}


/* Constructors and destructor */

Destination::Destination()
{
}

Destination::~Destination()
{
}
