#include "../p_MODE7.h"

#include <MODE7/Internal/RawProcessor.hh>

using namespace MODE7;
using namespace MODE7::Internal;

Processor *
RawProcessor::newProcessorOfKind(Processor::Kind kind, Carousel *carousel)
{
	assert(Processor::Raw == kind);
	assert(nullptr != carousel);
	return new RawProcessor(kind, carousel);
}

RawProcessor::RawProcessor(Processor::Kind kind, Carousel *carousel): Processor(kind, carousel)
{
}

RawProcessor::~RawProcessor()
{
}

/* the octet is placed at the current cursor position, and the cursor advanced
 * the only processing performed is stripping the high bit for consistency
 * the cursor wraps around at both the right and bottom edges
 */
void
RawProcessor::putOctet(uint8_t octet)
{
	Frame *frame = this->currentFrame();

	assert(nullptr != frame);
	frame->nputch(octet & 0x7f);
	int x = frame->curx(), y = frame->cury();
	if(x >= MODE7_WIDTH && y >= MODE7_TEXTROWS)
	{
		/* wrap around to top left */
		frame->gotoxy(1, 1);
	}
	else if(x >= MODE7_WIDTH)
	{
		frame->gotoxy(1, y + 1);
	}
	else
	{
		frame->gotoxy(x + 1, y);
	}
//	DPRINTF("--- RawProcessor::putOctet(%02x), (%d, %d) -> (%d, %d)", ((int) octet), x, y, frame->curx(), frame->cury());
}
