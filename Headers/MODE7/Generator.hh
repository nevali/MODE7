#ifndef MODE7_GENERATOR_HH_
# define MODE7_GENERATOR_HH_           202506L

# include <MODE7/Object.hh>
# include <MODE7/PacketBuffer.hh>
# include <MODE7/Clock.hh>

namespace MODE7
{
	class Source;
	class Destination;
	class PacketBuffer;
	
	class Generator: public Object
	{
		/* Class methods */
		public:
			static Generator *serialTeletextGeneratorFor(Carousel *carousel, PacketBuffer *packetBuffer);
			static Generator *parallelTeletextGeneratorFor(Carousel *carousel, PacketBuffer *packetBuffer);

		/* Constructors and destructor */
		protected:
			Generator(Carousel *carousel, PacketBuffer *packetBuffer);
			virtual ~Generator();

		/* Instance methods */
		public:
			bool processPendingEvents(void); /* process any pending events */
			void runUntilStopped(void);      /* continually process events */
			void stop(void);                 /* may be called from a signal handler */
		protected:
			virtual void generate(void) = 0;

		/* Instance data */
		private:
			Carousel *carousel_;
			PacketBuffer *packetBuffer_;
			bool running_;
			volatile bool stop_;
			Clock *clock_;

		/* Inline accessors */
		public:
			inline Carousel *carousel(void) const { return carousel_; }
			inline Clock *clock(void) const { return clock_; }
			inline bool running(void) const { return running_; }
		protected:
			inline bool shouldStop(void) const { return stop_; }
			inline PacketBuffer *packetBuffer(void) const { return packetBuffer_; }
	};
}

#endif /*MODE7_GENERATOR_HH_*/
