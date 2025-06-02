#include "../p_MODE7.h"

#include <MODE7/Internal/TeletextProcessor.hh>

using namespace MODE7;
using namespace MODE7::Internal;

Processor *
TeletextProcessor::newProcessorOfKind(Processor::Kind kind, Carousel *carousel)
{
	assert(Processor::Teletext == kind);
	assert(nullptr != carousel);
	return new TeletextProcessor(kind, carousel);
}

TeletextProcessor::TeletextProcessor(Processor::Kind kind, Carousel *carousel): RawProcessor(kind, carousel)
{
}

TeletextProcessor::~TeletextProcessor()
{
}

void
TeletextProcessor::putOctet(uint8_t octet)
{
	Frame *frame = this->currentFrame();
	int x = frame->curx(), y = frame->cury();

	assert(nullptr != frame);

	if(octet < 32 || octet == 127)
	{
		/* (local) control codes */
		switch(octet)
		{
			case 0x00:
				frame->nputch(' '); break;
			case 0x08:
				frame->left(true); return;
			case 0x09:
				frame->right(true); return;
			case 0x0a:
				frame->down(true); return;
			case 0x0b:
				frame->up(true); return;
			case 0x0c:
				frame->clear(); return;
			case 0x0d:
				frame->gotoxy(1, y); return;
			case 0x1e:
				frame->gotoxy(1, 1); return;
			case 0x7f:
				frame->left(true); frame->nputch(' '); return;
			default:
				return;
		}
	}
	else
	{
		frame->nputch(octet & 0x7f);
	}
	/* advance the cursor */
	if(x == MODE7_WIDTH)
	{
		frame->newline();
	}
	else
	{
		frame->right();
	}
//	DPRINTF("--- TeletextProcessor::putOctet(%02x), (%d, %d) -> (%d, %d)", ((int) octet), x, y, frame->curx(), frame->cury());
}
