#include "../p_MODE7.h"

using namespace MODE7;

Magazine::Magazine(Carousel *carousel, int num):
	carousel_(carousel),
	number_(num % 8), /* magazine 8 is stored/sent as 0 */
	pages_(nullptr), pageCount_(0)
{
	DPRINTF(">>> new Magazine %d constructed", number_);
}

Magazine::~Magazine()
{
	if(pages_)
	{
		size_t i;

		for(i = 0; i < pageCount_; i++)
		{
			if(pages_[i])
			{
				pages_[i]->release();
			}
			pages_[i] = nullptr;
		}
		free(pages_);
		pages_ = nullptr;
		pageCount_ = 0;
	}
	DPRINTF("<<< Magazine %d destroyed", number_);
}

Page *
Magazine::pageNumbered(uint8_t number, bool create)
{
	size_t i;

	/* this is not very efficient, but that may not be
	 * much of a problem
	 */
	for(i = 0; pages_ && i < pageCount_; i++)
	{
		if(pages_[i]->number() == number)
		{
			return pages_[i];
		}
	}
	if(create)
	{
		DPRINTF("--- Magazine #%d: creating new page %02x", (number_ ? number_ : 8), number);
		pages_ = (Page **) xrealloc(pages_, sizeof(Page *) * (pageCount_ + 1));
		i = pageCount_;
		pageCount_++;
		pages_[i] = new Page(this, number);
		return pages_[i];
	}
	return nullptr;
}
