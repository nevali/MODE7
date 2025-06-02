#include "../p_MODE7.h"

#include <MODE7/Internal/ParallelTeletextGenerator.hh>

using namespace MODE7;
using namespace MODE7::Internal;

ParallelTeletextGenerator::ParallelTeletextGenerator(Carousel *carousel, PacketBuffer *packetBuffer): TeletextGenerator(carousel, packetBuffer)
{
}

ParallelTeletextGenerator::~ParallelTeletextGenerator()
{
}
