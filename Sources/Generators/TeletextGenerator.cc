#include "../p_MODE7.h"

#include <MODE7/Internal/TeletextGenerator.hh>

using namespace MODE7;
using namespace MODE7::Internal;

static int offsetOf(const uint8_t *buf, size_t bufsize, const char *needle)
{
	char *ch;
	const char *haystack = static_cast<const char *>(static_cast<const void *>(buf));

	ch = strnstr(haystack, needle, bufsize);
	if(nullptr == ch)
	{
		return -1;
	}
	return ch - haystack;
}

TeletextGenerator::TeletextGenerator(Carousel *carousel, PacketBuffer *packetBuffer): Generator(carousel, packetBuffer)
{
}

TeletextGenerator::~TeletextGenerator()
{
}

void
TeletextGenerator::generate(void)
{
	DPRINTF("!!! TeletextGenerator::generate() is not implemented");
}

void
TeletextGenerator::generateFiller(Magazine *mag)
{
	Packet *packet;

	packet = packetBuffer()->fillerPacket(mag);
	packet->deliver();
}

void
TeletextGenerator::populateHeader(Packet *packet, Page *page)
{
	uint8_t header[MODE7_HEADER];

	(void) page;

	memset(header, ' ', sizeof(header));
	if(page && page->magazine() && page->magazine()->carousel())
	{
		page->magazine()->carousel()->copyTeletextHeader(header, sizeof(header));
	}
	performHeaderSubstitutions(header, sizeof(header), page);
	packet->setTextOctets(header, sizeof(header));
}

void
TeletextGenerator::populateTextRow(Packet *packet, Frame *frame, uint8_t row)
{
	packet->setTextOctets(frame->octetsAtRow(row), MODE7_WIDTH);
}

Packet *
TeletextGenerator::generateNextRow(Cursor *cursor, uint8_t row)
{
	Page *page = cursor->page();
	Packet *packet;

	if(nullptr == page)
	{
		return nullptr;
	}
	Frame *frame = cursor->frame();
	if(!frame)
	{
		DPRINTF("!!! no active frame available from cursor!");
		return nullptr;
	}
	if(0 == row)
	{
		/* M/0 - page header */
		packet = packetBuffer()->headerPacketForFrame(frame);
		this->populateHeader(packet, page);
		return packet;
	}
	if(row <= MODE7_PKT_TEXTMAX)
	{
		/* X/1 .. X/24 - text */
		packet = packetBuffer()->packetForTextRow(frame, row);
		this->populateTextRow(packet, frame, row);
		return packet;
	}
	/* unsupported */
	DPRINTF("!!! TeletextGenerator::generateNextRow() X/%d packets not supported", row);
	return nullptr;
}

/* Support for essentially the same set of substitutions as vbit2:
 *
 * %%# (or %%_)    Magazine & Page number
 * %%a             Short day name (Mon/Tue/etc.)
 * %%b             Short month name (Jan/Feb/etc.)
 * %d              Day of month with leading zero
 * %e              Day of month with leading space
 * %m              Month number with leading zero
 * %y              Two-digit year
 * %H              Hour with leading zero
 * %M              Minute
 * %S              Second
 *
 * All times use the local timezone. Note that all replacements are the same
 * width as the template patterns they replace.
 */
void
TeletextGenerator::performHeaderSubstitutions(uint8_t *buffer, size_t bufsize, Page *page)
{
	Clock *clock = Clock::sharedClock();
	time_t now = clock->now().secs;
    struct tm timebuf;
	char *buf;
    int off;
	/* there are two local variables here whose names differ only by case
	 * please accept my apologies
	 */
	char t_page[4], t_day[4], t_mon[4], t_d[3], t_e[3], t_m[3], t_y[3], t_H[3], t_M[3], t_S[3];

	buf = static_cast<char *>(static_cast<void *>(buffer)); /* trust me, i'm a doctor */
	localtime_r(&now, &timebuf);
	if(nullptr != page)
	{
		snprintf(t_page, sizeof(t_page), "%03x", page->pageNumber());
	}
	else
	{
		t_page[0] = t_page[1] = t_page[2] = ' ';
		t_page[3] = 0;
	}
	strftime(t_day, sizeof(t_day), "%a", &timebuf);
	strftime(t_mon, sizeof(t_mon), "%b", &timebuf);
	strftime(t_d, sizeof(t_d), "%d", &timebuf);
	strftime(t_e, sizeof(t_e), "%e", &timebuf);
	strftime(t_m, sizeof(t_m), "%m", &timebuf);
	strftime(t_y, sizeof(t_y), "%y", &timebuf);
	strftime(t_H, sizeof(t_H), "%H", &timebuf);
	strftime(t_M, sizeof(t_M), "%M", &timebuf);
	strftime(t_S, sizeof(t_S), "%S", &timebuf);

	if(-1 != (off = offsetOf(buffer, bufsize, "%%#")))
	{
		memcpy(&(buf[off]), t_page, strlen(t_page));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%%_")))
	{
		memcpy(&(buf[off]), t_page, strlen(t_page));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%%a")))
	{
		memcpy(&(buf[off]), t_day, strlen(t_day));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%%b")))
	{
		memcpy(&(buf[off]), t_mon, strlen(t_mon));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%d")))
	{
		memcpy(&(buf[off]), t_d, strlen(t_d));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%e")))
	{
		memcpy(&(buf[off]), t_e, strlen(t_e));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%m")))
	{
		memcpy(&(buf[off]), t_m, strlen(t_m));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%y")))
	{
		memcpy(&(buf[off]), t_y, strlen(t_y));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%H")))
	{
		memcpy(&(buf[off]), t_H, strlen(t_H));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%M")))
	{
		memcpy(&(buf[off]), t_M, strlen(t_M));
	}
	if(-1 != (off = offsetOf(buffer, bufsize, "%S")))
	{
		memcpy(&(buf[off]), t_S, strlen(t_S));
	}
}
