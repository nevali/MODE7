#include "../p_MODE7.h"

using namespace MODE7;

Page::Page(Magazine *mag, uint8_t number):
	magazine_(mag),
	number_(number),
	pageNum_(mag->number() << 8 | number),
	frames_(nullptr),
	frameCount_(0)
{
	DPRINTF(">>> new Page %03x constructed", pageNum_);
}

Page::~Page()
{
	magazine_ = nullptr;
	for(size_t i = 0; i < frameCount_; i++)
	{
		if(frames_[i])
		{
			frames_[i]->release();
			frames_[i] = nullptr;
		}
	}
	delete frames_;
	frames_ = nullptr;
	DPRINTF("<<< Page %03x destroyed", pageNum_);
}

uint8_t
Page::number(void) const
{
	return number_;
}

int
Page::pageNumber(void) const
{
	return pageNum_;
}

Frame *
Page::frameNumbered(uint8_t number, bool create)
{
	uint8_t i;

	/* again, not efficient */
	for(i = 0; frames_ && i < frameCount_; i++)
	{
		if(frames_[i]->number() == number)
		{
			return frames_[i];
		}
	}
	if(create)
	{
		DPRINTF("--- Page #%03x: creating new frame %d", pageNum_, number);
		frames_ = (Frame **) xrealloc(frames_, sizeof(Frame *) * (frameCount_ + 1));
		i = frameCount_;
		frameCount_++;
		frames_[i] = new Frame(this, number);
		return frames_[i];
	}
	return nullptr;
}

Frame *
Page::activeFrame(void)
{
	/* XXX */
	if(frameCount_)
	{
		return frames_[0];
	}
	return nullptr;
}

void
Page::cycle(void)
{
}
