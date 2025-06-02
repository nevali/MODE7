#include "../p_MODE7.h"

using namespace MODE7;

Packet::Packet()
{
}

Packet::~Packet()
{
}

void
Packet::setText(const char *text)
{
	size_t count = strlen(text);

	if(count > MODE7_WIDTH)
	{
		count = MODE7_WIDTH;
	}
	this->setTextOctets(static_cast<const uint8_t *>(static_cast<const void *>(text)), count);
}
