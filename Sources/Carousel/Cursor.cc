#include "../p_MODE7.h"

using namespace MODE7;

Cursor::Cursor(Carousel *carousel):
	mseq_(0), pseq_(0),
	carousel_(carousel),
	magazine_(nullptr),
	page_(nullptr),
	frame_(nullptr)
{
}

Cursor::~Cursor()
{
}

Magazine *
Cursor::magazine(void)
{
	if(nullptr == magazine_)
	{
		nextMagazine();
	}
	return magazine_;
}

Page *
Cursor::page(void)
{
	if(nullptr == page_)
	{
		nextPage();
	}
	return page_;
}

Frame *
Cursor::frame(void)
{
	if(nullptr == page_)
	{
		nextPage();
	}
	return frame_;
}

bool
Cursor::nextMagazine(void)
{
	uint8_t prev = mseq_;

	magazine_ = nullptr;
	page_ = nullptr;
	frame_ = nullptr;
	for(++mseq_; mseq_ != prev; mseq_ = (mseq_ + 1) % MODE7_MAGAZINES)
	{
		if(nullptr != carousel_->magazines_[mseq_])
		{
			magazine_ = carousel_->magazines_[mseq_];
			return firstPage();
		}
	}
//	DPRINTF("!!! Cursor::nextMagazine(): no more magazines");
	if(nullptr != carousel_->magazines_[mseq_])
	{
		magazine_ = carousel_->magazines_[mseq_];
		return firstPage();
	}
	return false;
}

bool
Cursor::firstPage(void)
{
	page_ = nullptr;
	frame_ = nullptr;
	pseq_ = 0;
	if(nullptr == magazine_)
	{
		/* this will call firstPage() if there is a magazine */
		return nextMagazine();
	}
//	DPRINTF("+++ Cursor::firstPage(): magazine is now %d", (int) mseq_);
	/* this is (potentially) incredibly inefficient */
	for(pseq_ = 0; pseq_ < MODE7_PAGES; pseq_++)
	{
		Page *p = magazine_->pageNumbered(pseq_, false);

		if(nullptr != p)
		{
//			DPRINTF("+++ Cursor::firstPage(): magazine is now %d, page is now %d", (int) mseq_, (int) pseq_);
			page_ = p;
			frame_ = p->activeFrame();
			return true;
		}
	}

//	DPRINTF("!!! Cursor::firstPage(): no pages found");
	return false;
}

bool
Cursor::nextPage(void)
{
	page_ = nullptr;
	frame_ = nullptr;
	if(nullptr == magazine_)
	{
		/* this will call firstPage() if there is a magazine */
		return nextMagazine();		
	}
//	DPRINTF("+++ Cursor::nextPage(): magazine is now %d, page index is %d", (int) mseq_, (int) pseq_);
	/* this too */
	for(++pseq_; pseq_ < MODE7_PAGES; pseq_++)
	{
		Page *p = magazine_->pageNumbered(pseq_, false);

		if(nullptr != p)
		{
//			DPRINTF("+++ Cursor::nextPage(): magazine is now %d, page is now %d", (int) mseq_, (int) pseq_);
			page_ = p;
			p->cycle();
			frame_ = p->activeFrame();
			return true;
		}
	}
//	DPRINTF("!!! Cursor::nextPage(): no more pages in magazine");
	return nextMagazine();
}