#ifndef MODE7_SOURCE_HH_
# define MODE7_SOURCE_HH_              202506L

# include <MODE7/Object.hh>

namespace MODE7
{
	class Processor;
	
	/* a Source represents the process generating the Teletext frames,
	 * generally a child process attached to a pseudoterminal
	 */
	class Source: public Object
	{
		/* Class methods */
		public:
			/* spawn a sub-process source consuming arguments passed to main() */
			static Source *subProcessSource(Processor *processor, int argc, char **argv);

		/* Constuctors and destructor */
		protected:
			Source(Processor *processor);
			virtual ~Source();

		/* Instance methods*/
		public:
			/* return false to terminate the generator - MUST NOT BLOCK! */
			virtual bool processPendingEvents(void);

		/* Instance data */
		private:
			Processor *processor_;
			Carousel *carousel_;
		
		/* Inline accessors */
		public:
			inline Processor *processor(void) const { return processor_; }
			inline Carousel *carousel(void) const { return carousel_; }
	};
}

#endif /*!MODE7_SOURCE_HH_*/