#include "../p_MODE7.h"
#include <ctype.h>

using namespace MODE7;

/* note that x and y use 1-based indices (1..80, 1..24) */
static inline size_t bufpos(uint8_t x, uint8_t y)
{
	/* bounds check */
	x = (x < 1 ? 1 : (x > MODE7_WIDTH ? MODE7_WIDTH : x));
	y = (y < 1 ? 1 : (y > MODE7_TEXTROWS ? MODE7_TEXTROWS : y));
	/* one byte per character cell */
	size_t offset = (((size_t) y - 1) * MODE7_WIDTH) + ((size_t) x - 1);
	assert(offset < (MODE7_WIDTH * MODE7_TEXTROWS));
	return offset;
}

Frame::Frame(Page *page, uint16_t number):
	page_(page),
	number_(number),
	x_(1), y_(1),
	dirty_(true)
{
	memset(buffer_, ' ', sizeof(buffer_));
	DPRINTF(">>> new Frame %p %03x.%04x constructed", this, page_->pageNumber(), number);
}

Frame::~Frame()
{
	DPRINTF("<<< Frame %p %03x.%04x destroyed", this, page_->pageNumber(), number_);
}

const uint8_t *
Frame::octetsAtRow(uint8_t row) const
{
	assert(row >= MODE7_PKT_TEXT1);
	assert(row <= MODE7_PKT_TEXTMAX);
	if(row < MODE7_PKT_TEXT1 || row > MODE7_PKT_TEXTMAX)
	{
		return nullptr;
	}
	return &(buffer_[bufpos(1, row)]);
}

void
Frame::nputch(uint8_t octet)
{
	dirty_ = true;
	buffer_[bufpos(x_, y_)] = octet;
//	DPRINTF("- - Frame::nputch(%d, %d) => %u => 0x%02x (%d) '%c'", x_, y_, bufpos(x_, y_), octet, octet, (octet >= 32 && octet <= 126) ? octet : ' ');
}

void
Frame::clear(void)
{
	dirty_ = true;
	memset(buffer_, ' ', sizeof(buffer_));
	x_ = 1;
	y_ = 1;
}

void
Frame::scrollup(int lines)
{
	if(lines < 1)
	{
		return;
	}
	if(lines >= MODE7_TEXTROWS)
	{
		dirty_ = true;
		memset(buffer_, ' ', sizeof(buffer_));
		return;
	}
	dirty_ = true;
	/* shift all but the last (40 * lines) characters up */
	memmove(buffer_, &(buffer_[bufpos(1, lines + 1)]), sizeof(buffer_) - (lines * MODE7_WIDTH));
	/* fill the last (40 * lines) characters with spaces */
	memset(&(buffer_[bufpos(1, MODE7_TEXTROWS - (lines - 1))]), ' ', (lines * MODE7_WIDTH));
}

void
Frame::newline(void)
{
	x_ = 1;
	y_++;
	if(y_ > MODE7_TEXTROWS)
	{
		y_ = MODE7_TEXTROWS;
		scrollup(1);
	}
}

void
Frame::putch(uint8_t octet)
{
	if(octet == 0)
	{
		return;
	}
	if(octet == '\r')
	{
		x_ = 1;
		return;
	}
	if(octet == '\n')
	{
		newline();
		return;
	}
	nputch(octet);
	x_++;
	if(x_ > MODE7_WIDTH)
	{
		newline();
	}
}

void
Frame::write(const uint8_t *octets, size_t size)
{
	while(size)
	{
		putch(octets[0]);
		octets++;
		size--;
	}
}

void
Frame::puts(const char *str)
{
//	DPRINTF("[puts: '%s']", str);
	write(static_cast<const uint8_t *>(static_cast<const void *>(str)), strlen(str));
//	DPRINTF("[x = %d, y = %d]", x_, y_);
}


void
Frame::gotoxy(int x, int y)
{
	x = (x < 1 ? 1 : (x > MODE7_WIDTH ? MODE7_WIDTH : x));
	y = (y < 1 ? 1 : (y > MODE7_TEXTROWS ? MODE7_TEXTROWS : y));
	x_ = x;
	y_ = y;
}

void
Frame::clearLine(void)
{
	assert(y_ >= 1);
	assert(y_ <= MODE7_TEXTROWS);
	x_ = 1;
	memset(&(buffer_[bufpos(1, y_)]), ' ', MODE7_WIDTH);
}

void
Frame::left(bool wrap)
{
	if(x_ == 1)
	{
		if(wrap)
		{
			x_ = MODE7_WIDTH;
		}
		return;
	}
	x_--;
}

void
Frame::up(bool wrap)
{
	if(y_ == 1)
	{
		if(wrap)
		{
			y_ = MODE7_TEXTROWS;
		}
		return;
	}
	y_--;
}

void
Frame::right(bool wrap)
{
	if(x_ == MODE7_WIDTH)
	{
		if(wrap)
		{
			x_ = 1;
		}
		return;
	}
	x_++;
}

void
Frame::down(bool wrap)
{
	if(y_ == MODE7_TEXTROWS)
	{
		if(wrap)
		{
			y_ = 1;
		}
		return;
	}
	y_++;
}
