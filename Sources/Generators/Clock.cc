#include "../p_MODE7.h"

using namespace MODE7;

static Clock *sharedClock_;

Clock::Clock(): Singleton(),
	source_(nullptr),
	linesPerField_(16),
	stop_(false)
{
	struct timeval wall;

	gettimeofday(&wall, nullptr);
	now_.secs = wall.tv_sec;
	now_.field = wall.tv_usec / 20000;
	now_.line = linesPerField_ - 1;
}

Clock *
Clock::sharedClock(void)
{
	if(!sharedClock_)
	{
		sharedClock_ = new Clock();
	}
	return sharedClock_;
}

void
Clock::stop(void)
{
	stop_ = true;
}

/* following vbit2's approach:
 *
 * we assume 50 fields per second, with <n> (typically n=16) VBI lines per
 * field (one field is therefore 20000us or 20ms)
 *
 * on each call to tick(), we step the line counter
 *
 * when we reach the start of a new field, we check that the wall clock is
 * in sync with our internal counters; if we're lagging (or the wall clock
 * has changed significantly, for example due to suspend/resume), we can
 * resync; otherwise, we sleep until the wall clock tells us the new field
 * has begun
 */
bool
Clock::tick(void)
{
	struct timeval wall;
	uint64_t field;

	if(stop_)
	{
		return false;
	}

	gettimeofday(&wall, nullptr);
	field = wall.tv_usec / 20000;

	/* step the line counter */
	now_.line = (now_.line + 1) % linesPerField_;

	if(0 == now_.line)
	{
		/* new field */
		now_.field = (now_.field + 1) % 50;
//		DPRINTF("!!! new field = %d (wall field = %lld)", (int) now_.field, field);
		if(now_.field == 0)
		{
			/* new second */
			now_.secs++;
//			DPRINTF("!!! new second tick - now.secs = %ld", now_.secs);
		}
	}

	if(0 == now_.line)
	{
		/* internal field counter is behind wall clock */
		if(field != (uint64_t) now_.field)
		{
//			DPRINTF("!!! internal field = %llu, wall field = %llu", (uint64_t) now_.field, 	field);
//			uint64_t c = 0;
			do
			{
				
				usleep(10000); /* half a frame */
//				c++;
				gettimeofday(&wall, nullptr);
				field = wall.tv_usec / 20000;
			}
			while(!stop_ && field != (uint64_t) now_.field);
//			DPRINTF("!!! slept %llu times until field = %llu, now.field = %d (now.secs = %ld wall.tv_sec= %ld)", c, field, now_.field, now_.secs, wall.tv_sec);
			now_.field = field;
		}
		if(0 == now_.field)
		{
			if(now_.secs != wall.tv_sec)
			{
				DPRINTF("!!! resynchronising clock - now.secs = %lu, wall = %lu", (unsigned long) now_.secs, (unsigned long) wall.tv_sec);
				now_.secs = wall.tv_sec;
				now_.field = field;
			}
		}
		if(stop_)
		{
			return false;
		}
		if(source_)
		{
			return source_->processPendingEvents();
		}
		return false;
	}
	/* don't bail out mid-field */
	return true;
}

bool
Clock::addSource(Source *source)
{
	/* only one source at a time */
	if(nullptr != source_)
	{
		return false;
	}
	source_ = source;
	DPRINTF("+++ Clock: source %p added", source);
	return true;
}

bool
Clock::removeSource(Source *source)
{
	if(source != source_)
	{
		return false;
	}
	source_ = nullptr;
	DPRINTF("--- Clock: source %p removed", source);
	return true;
}
