#include "../p_MODE7.h"

#include <MODE7/Internal/RawProcessor.hh>
#include <MODE7/Internal/TeletextProcessor.hh>

using namespace MODE7;

Processor *
Processor::newProcessorOfKind(Kind kind, Carousel *carousel)
{
	switch(kind)
	{
		case Raw:
			return MODE7::Internal::RawProcessor::newProcessorOfKind(kind, carousel);
		case Teletext:
			return MODE7::Internal::TeletextProcessor::newProcessorOfKind(kind, carousel);
	}
	assert(false);
	abort();
	return nullptr;
}

Processor::Processor(Processor::Kind kind, Carousel *carousel): Object(),
	kind_(kind), carousel_(carousel), frame_(nullptr)
{
	assert(carousel != nullptr);
	carousel->retain();
}

Processor::~Processor()
{
	if(nullptr != frame_)
	{
		frame_->release();
		frame_ = nullptr;
	}
	if(nullptr != carousel_)
	{
		carousel_->release();
		carousel_ = nullptr;
	}
}

/* create a new processor of the same kind attached to the same carousel */
Processor *
Processor::clone(void)
{
	return Processor::newProcessorOfKind(kind_, carousel_);
}

void
Processor::setFrame(Frame *newFrame)
{
	if(newFrame == frame_)
	{
		return;
	}
	if(nullptr != frame_)
	{
		frame_->release();
	}
	frame_ = newFrame;
	if(nullptr != frame_)
	{
		frame_->retain();
	}
}

Frame *
Processor::currentFrame(void)
{
	if(nullptr == frame_)
	{
		frame_ = this->defaultFrame();
		assert(nullptr != frame_);
	}
	return frame_;
}

/* return a *retained* reference to the default frame */
Frame *
Processor::defaultFrame(void)
{
	Frame *frame = carousel_->subpageNumbered(0x10000, true);
	assert(nullptr != frame);
	frame->retain();
	return frame;
}

/* String output */

void
Processor::putOctets(const uint8_t *octets, size_t count)
{
	assert(nullptr != octets);

	for(; count > 0; count--, octets++)
	{
		putOctet(*octets);
	}
}

void
Processor::putch(int ch)
{
	if(ch > 0)
	{
		putOctet(ch & 0xff);
	}
}

void
Processor::puts(const char *str)
{
	assert(nullptr != str);
	putOctets(static_cast<const uint8_t *>(static_cast<const void *>(str)), strlen(str));
}

/* Basic state management  */
void
Processor::clear(void)
{
	currentFrame()->clear();	
}

void
Processor::gotoxy(int x, int y)
{
	currentFrame()->gotoxy(x, y);	
}
