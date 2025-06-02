/* Portions (especially lower-level code) adapted from Peter Kwan's vbit2:
 *
 * Copyright (C) 2016, Peter Kwan
 *
 * Permission to use, copy, modify, and distribute this software
 * and its documentation for any purpose and without fee is hereby
 * granted, provided that the above copyright notice appear in all
 * copies and that both that the copyright notice and this
 * permission notice and warranty disclaimer appear in supporting
 * documentation, and that the name of the author not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 *
 * The author disclaims all warranties with regard to this
 * software, including all implied warranties of merchantability
 * and fitness.  In no event shall the author be liable for any
 * special, indirect or consequential damages or any damages
 * whatsoever resulting from loss of use, data or profits, whether
 * in an action of contract, negligence or other tortious action,
 * arising out of or in connection with the use or performance of
 * this software.
 *************************************************************************** 
 **/

#include "../p_MODE7.h"

#include <MODE7/Internal/T45Packets.hh>

using namespace MODE7;
using namespace MODE7::Internal;

/*** Packet buffer ***/

T45PacketBuffer::T45PacketBuffer(Packet::Format format, Destination *destination): PacketBuffer(format, destination),
	packet_(nullptr)
{
	DPRINTF(">>> new T45PacketBuffer created for destination");
}

T45PacketBuffer::~T45PacketBuffer()
{
	delete packet_;
	DPRINTF("<<< T45PacketBuffer destroyed");
}

void
T45PacketBuffer::deliver(T45Packet *packet)
{
//	DPRINTF("--- T45PacketBuffer: Delivering packet");
	destination()->write(packet);
}

Packet *
T45PacketBuffer::headerPacketForFrame(Frame *frame)
{
	assert(nullptr != frame);
	if(nullptr == packet_)
	{
		packet_ = createPacket();
		assert(nullptr != packet_);
	}
	packet_->resetToHeader(frame);
	return packet_;
}

Packet *
T45PacketBuffer::fillerPacket(Magazine *magazine)
{
	int mag = 0;

	if(nullptr != magazine)
	{
		mag = magazine->number() % MODE7_MAGAZINES;
	}
	if(nullptr == packet_)
	{
		packet_ = createPacket();
		assert(nullptr != packet_);
	}
	packet_->resetToFiller(mag);
	return packet_;
}

Packet *
T45PacketBuffer::packetForTextRow(Frame *frame, uint8_t row)
{
	assert(nullptr != frame);
	assert(row > 0);
	assert(row < 25);
	if(nullptr == packet_)
	{
		packet_ = createPacket();
		assert(nullptr != packet_);
	}
	packet_->resetToTextRow(frame, row);
	return packet_;
}

T45Packet *
T45PacketBuffer::createPacket(void)
{
	return new T45Packet(this);
}

/*** Packet ***/

T45Packet::T45Packet(T45PacketBuffer *buffer):
	packetBuffer_(buffer)
{
	reset();
}


T45Packet::~T45Packet()
{
}

void
T45Packet::reset()
{
	row_ = 0xff;
	mag_ = 8;
	page_ = nullptr;
	frame_ = nullptr;
	magazine_ = nullptr;
	memset(buffer_, ' ', sizeof(buffer_));
}

/* send the packet to the destination via the packet buffer */
void
T45Packet::deliver(void)
{
	update();
	packetBuffer_->deliver(this);
}

/* reset to a header packet for the given page */
void
T45Packet::resetToHeader(Frame *frame)
{
	assert(nullptr != frame);
	reset();
	frame_ = frame;
	subcode_ = frame->number();
	page_ = frame->page();
	assert(nullptr != page_);
	pageNumber_ = page_->number();
	row_ = 0;
	magazine_ = page_->magazine();
	assert(nullptr != magazine_);
	mag_ = magazine_->number() % MODE7_MAGAZINES;
}

/* reset to being a filler packet */
void
T45Packet::resetToFiller(uint8_t mag)
{
	reset();
	mag_ = mag % MODE7_MAGAZINES;
	pageNumber_ = 0xff;
	subcode_ = 0x3f7f;
}

/* reset to a text row packet for the given frame and row */
void
T45Packet::resetToTextRow(Frame *frame, uint8_t row)
{
	assert(nullptr != frame);
	assert(row > 0);
	assert(row < 25);
	reset();
	frame_ = frame;
	subcode_ = frame->number();
	row_ = row;
	page_ = frame->page();
	assert(nullptr != page_);
	magazine_ = page_->magazine();
	assert(nullptr != magazine_);
	mag_ = magazine_->number() % MODE7_MAGAZINES;
}

/* calculate checksums/parity/etc. - invoked immediately before the packet is
 * delivered to the destination
 */
void
T45Packet::update(void)
{
	if(0 == row_)
	{
		updateM0();
		return;
	}
	updateFraming();
	updateParity(MODE7_BYTE_TEXT);
}

/* update an M/0 packet */
void
T45Packet::updateM0(void)
{
	uint8_t cbit;
	uint16_t control = 0;
	uint16_t subcode = subcode_;

    updateFraming();

    buffer_[5] = MODE7_Hamming8EncodeTable[pageNumber_ % 0x10];
    buffer_[6] = MODE7_Hamming8EncodeTable[pageNumber_ / 0x10];
    buffer_[7] = MODE7_Hamming8EncodeTable[(subcode & 0x0f)];         // S1 four bits

    subcode >>= 4;

    // Map the page settings control bits from MiniTED to actual teletext packet.
    // To find the MiniTED settings look at the tti format document.
    // To find the target bit position these are in reverse order to tx and not hammed.
    // So for each bit in ETSI document, just divide the bit number by 2 to find the target location.
    // Where ETSI says bit 8,6,4,2 this maps to 4,3,2,1 (where the bits are numbered 1 to 8)
    cbit=0;
    if(control & 0x4000) cbit=0x08;                        // C4 Erase page
    buffer_[8] = MODE7_Hamming8EncodeTable[(subcode & 0x07) | cbit];  // S2 (3 bits) add C4
    subcode >>= 4;
    buffer_[9] = MODE7_Hamming8EncodeTable[(subcode & 0x0f)];         // S3 four bits
    subcode >>= 4;
    
    cbit=0;
    if(control & 0x0001) cbit=0x04;                        // C5 Newsflash
    if(control & 0x0002) cbit|=0x08;                       // C6 Subtitle
    buffer_[10] = MODE7_Hamming8EncodeTable[(subcode & 0x03) | cbit]; // S4 C6, C5
    
    cbit=0;
    if(control & 0x0004)  cbit=0x01;                       // C7 Suppress Header
    if(control & 0x0008) cbit|=0x02;                       // C8 Update
    if(control & 0x0010) cbit|=0x04;                       // C9 Interrupted sequence
    if(control & 0x0020) cbit|=0x08;                       // C10 Inhibit display
    buffer_[11] = MODE7_Hamming8EncodeTable[cbit];                  // C7 to C10
    
    cbit = (control & 0x0380) >> 6;                           // Shift the language bits C12,C13,C14.
    
    // if (control & 0x0040) cbit|=0x01;                    // C11 serial/parallel *** We only work in parallel mode, Serial would mean a different packet ordering.
    buffer_[12] = MODE7_Hamming8EncodeTable[cbit];                  // C11 to C14 (C11=0 is parallel, C12,C13,C14 language)

    updateParity(MODE7_BYTE_HDRTEXT);
}

/* update the parity bits in the text of a packet 
 * offset is usually MODE7_BYTE_TEXT or MODE7_BYTE_HDRTEXT
 */
void
T45Packet::updateParity(size_t offset)
{
    size_t i;

    for(i = offset; i < MODE7_PACKETSIZE; i++)
    {
        buffer_[i] = MODE7_OddParityTable[buffer_[i] & 0x7f];
    }
}

/* update the run-in, framing code, magazine ID */
void
T45Packet::updateFraming(void)
{
	buffer_[0] = MODE7_RUNIN;
    buffer_[1] = MODE7_RUNIN;
    buffer_[2] = MODE7_FRAMINGCODE;
    buffer_[3] = MODE7_Hamming8EncodeTable[mag_ + ((row_ % 2) << 3)]; // mag + bit 3 is the lowest bit of row
    buffer_[4] = MODE7_Hamming8EncodeTable[((row_ >> 1) & 0x0f)];
}

void
T45Packet::setTextOctets(const uint8_t *octets, size_t count)
{
	assert(nullptr != octets);
	assert(count <= MODE7_WIDTH);
	assert(count + MODE7_BYTE_TEXT <= MODE7_PACKETSIZE);

	if(0 == row_)
	{
		/* header text starts at column 9 (1-indexed), packet byte 13 (0-indexed) */
		if(count >= MODE7_WIDTH - (MODE7_BYTE_HDRTEXT - MODE7_BYTE_TEXT))
		{
			count = MODE7_WIDTH - (MODE7_BYTE_HDRTEXT - MODE7_BYTE_TEXT);
		}
		memcpy(&(buffer_[MODE7_BYTE_HDRTEXT]), octets, count);
		return;
	}
	
	if(row_ >= MODE7_PKT_TEXT1 && row_ <= MODE7_PKT_TEXTMAX)
	{
		memcpy(&(buffer_[MODE7_BYTE_TEXT]), octets, count);
		return;
	}

    DPRINTF("!!! UNSUPPORTED row %d Packet %p: octets = '%s', count = %d\n", (int) row_, static_cast<const char *>((const void *) octets), (int) count);
}

const uint8_t *
T45Packet::buffer(void) const
{
	return buffer_;
}

size_t
T45Packet::size(void) const
{
	return MODE7_PACKETSIZE;
}
