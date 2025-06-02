#ifndef MODE7_INTERNAL_DEMOSOURCE_HH_
# define MODE7_INTERNAL_DEMOSOURCE_HH_ 202505L

# include <MODE7/Source.hh>
# include <termios.h>

namespace MODE7
{
	class Frame;

	namespace Internal
	{
		class SubProcessSource: public Source
		{
			public:
				SubProcessSource(Processor *processor, int argc, char **argv);
				virtual ~SubProcessSource();
			public:
				virtual bool processPendingEvents(void);
			private:
				ssize_t readFrom(int fd);
			private:
				Frame *frame_;
				int parent_;
				int child_;
				char devname_[PATH_MAX+1];
				size_t bufsize_;
				uint8_t *buffer_;
				pid_t pid_;
				struct termios termstate_;
		};
	}
}

#endif /*!MODE7_INTERNAL_DEMOSOURCE_HH_*/