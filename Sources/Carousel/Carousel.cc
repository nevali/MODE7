#include "../p_MODE7.h"

using namespace MODE7;

Carousel *
Carousel::newCarousel(void)
{
	return new Carousel();
}

Carousel::Carousel()
{
	memset(&magazines_, 0, sizeof(magazines_));
	memset(teletextHeader_, ' ', sizeof(teletextHeader_));
	DPRINTF(">>> new Carousel constructed");
}

Carousel::~Carousel()
{
	size_t c;

	for(c = 0; c < MODE7_MAGAZINES; c++)
	{
		if(magazines_[c])
		{
			delete magazines_[c];
			magazines_[c] = nullptr;
		}
	}
	DPRINTF("<<< Carousel destroyed");
}

void
Carousel::refresh(void)
{
	/* perform any periodic refresh tasks */
}

Magazine *
Carousel::magazineNumbered(int number, bool create)
{
	number %= MODE7_MAGAZINES;
	if(nullptr == magazines_[number])
	{
		if(create)
		{
			DPRINTF("--- Carousel: creating new magazine %d", number);
			magazines_[number] = new Magazine(this, number);
		}
	}
//	DPRINTF("--- Carousel: returning magazine %d", number);
	return magazines_[number];
}

/* 'number' here is magazine + page number, e.g., 0x100 for Page 100, 0x088 for Page 888 */
Page *
Carousel::pageNumbered(int number, bool create)
{
	Magazine *mag;

	if(nullptr == (mag = magazineNumbered(number >> 8, create)))
	{
		return nullptr;
	}
	return mag->pageNumbered(number & 0xff);
}

Frame *
Carousel::subpageNumbered(int number, bool create)
{
	Magazine *mag;
	Page *page;

	if(nullptr == (mag = magazineNumbered(number >> 16, create)))
	{
		return nullptr;
	}
	if(nullptr == (page = mag->pageNumbered((number & 0xff00) >> 8, create)))
	{
		return nullptr;
	}
	return page->frameNumbered(number & 0xff, create);
}

Cursor *
Carousel::newCursor(void)
{
	return new Cursor(this);
}

void
Carousel::setTeletextHeader(const char *str)
{
	size_t l = strlen(str);
	if(l > sizeof(teletextHeader_))
	{
		l = sizeof(teletextHeader_);
	}
	memcpy(teletextHeader_, str, l);
}

void
Carousel::copyTeletextHeader(uint8_t *dest, size_t size)
{
	if(size > sizeof(teletextHeader_))
	{
		size = sizeof(teletextHeader_);
	}
	memcpy(dest, teletextHeader_, size);
}
