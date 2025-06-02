#include "../p_MODE7.h"

#include <MODE7/Internal/SerialTeletextGenerator.hh>

using namespace MODE7;
using namespace MODE7::Internal;

SerialTeletextGenerator::SerialTeletextGenerator(Carousel *carousel, PacketBuffer *packetBuffer): TeletextGenerator(carousel, packetBuffer)
{
	cursor_ = carousel->newCursor();
}

SerialTeletextGenerator::~SerialTeletextGenerator()
{
	cursor_->release();
	cursor_ = nullptr;
}

void
SerialTeletextGenerator::generate(void)
{
	Packet *packet;

	packet = this->generateNextRow(cursor_, row_);
	if(!packet)
	{
		this->generateFiller(cursor_->magazine());
		cursor_->nextPage();
		resetRow();
		return;
	}
	packet->deliver();
	nextRow();
}

void
SerialTeletextGenerator::nextRow(void)
{
	row_++;
	if(row_ > MODE7_PKT_TEXTMAX)
	{
		cursor_->nextPage();
		resetRow();
	}
}

void
SerialTeletextGenerator::resetRow(void)
{
	row_ = 0;
}
