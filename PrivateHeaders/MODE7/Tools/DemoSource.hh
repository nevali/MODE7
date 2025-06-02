#ifndef MODE7_TOOLS_DEMOSOURCE_HH_
# define MODE7_TOOLS_DEMOSOURCE_HH_    202506L

# include <sys/time.h>

# include <MODE7/Source.hh>

namespace MODE7
{
	namespace Tools
	{
		class DemoSource: public Source
		{
			public:
				DemoSource(Processor *processor);
			protected:
				virtual ~DemoSource();
			public:
				virtual bool processPendingEvents(void);

				void generateDemoFrame(void);
				void generateInfoFrame(void);
				void generateEngineeringTest(void);
			protected:
				void updateDemoFrame(void);
				void updateInfoFrame(void);
				void updateEngineeringTest(void);
			private:
				time_t epoch_;
				time_t lastTime_;
				time_t now_;
				struct tm utc_;
				struct tm localtime_;
				/* One processor for each frame we want to write to
				 * concurrently 
				 */
				Processor *demoProcessor_;
				Processor *infoProcessor_;
				Processor *engProcessor_;
		};
	}
}

#endif /*!MODE7_TOOLS_DEMOSOURCE_HH_*/