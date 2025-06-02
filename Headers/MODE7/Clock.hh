#ifndef MODE7_CLOCK_HH_
# define MODE7_CLOCK_HH_               202506L

# include <MODE7/Time.h>
# include <MODE7/Singleton.hh>

namespace MODE7
{
	class Source;

	/* the Clock encapsulates both timed and signaled event coordination
	 *
	 * this is because processing I/O in a timely (!) manner can interfere with
	 * monotonic timers if we're not careful or resort to threads or stack
	 * juggling (with all of the pain they potentially bring), and so all
	 * management of when "now" really is, and whether data should be ferried
	 * to and from the Source, or whether packets should be generated, or
	 * whether the process should just sleep for a while is handled by the
	 * tick() method, which expects to be called frequently but potentially
	 * irregularly
	 *
	 * there is at most one Clock instance, returned by Clock::sharedClock()
	 *
	 * tick() is typically invoked by the Generator in its processing loop
	 * (within processPendingEvents())
	 */
	class Clock: public Singleton
	{
		public:
			static Clock *sharedClock(void);
		protected:
			Clock();
		public:
			bool tick(void);

			bool addSource(Source *source);
			bool removeSource(Source *source);

			void stop(void);
		private:
			Time now_;
			Source *source_; /* only one source for now */
			int linesPerField_;
			volatile bool stop_;
		public:
			inline Time now(void) const { return now_; }
	};
}

#endif /*!MODE7_CLOCK_HH_*/
