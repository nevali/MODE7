#include <sys/utsname.h>
#include <string.h>

#include <MODE7/MODE7.h>

#include <MODE7/Tools/DemoSource.hh>

using namespace MODE7;
using namespace MODE7::Tools;

DemoSource::DemoSource(Processor *processor): Source(processor),
	epoch_(0), lastTime_(0), now_(0), demoProcessor_(nullptr),
	infoProcessor_(nullptr), engProcessor_(nullptr)
{
	now_ = Clock::sharedClock()->now().secs;
	epoch_ = now_;
	lastTime_ = epoch_;
	gmtime_r(&now_, &utc_);
	localtime_r(&now_, &localtime_);
	carousel()->setTeletextHeader("  Demo  %%#  %%a %e %%b %H:%M/%S");
}

DemoSource::~DemoSource()
{
	if(nullptr != demoProcessor_)
	{
		demoProcessor_->release();
		demoProcessor_ = nullptr;
	}
	if(nullptr != infoProcessor_)
	{
		infoProcessor_->release();
		infoProcessor_ = nullptr;
	}
	if(nullptr != engProcessor_)
	{
		engProcessor_->release();
		engProcessor_ = nullptr;
	}
}

bool
DemoSource::processPendingEvents(void)
{
	now_ = Clock::sharedClock()->now().secs;
	gmtime_r(&now_, &utc_);

	if(now_ == lastTime_)
	{
		/* update the demo frame on every field */
		updateDemoFrame();
		return true;
	}
	localtime_r(&now_, &localtime_);
	updateDemoFrame();
	updateInfoFrame();
	updateEngineeringTest();
	lastTime_ = now_;

	return true;
}

void
DemoSource::generateEngineeringTest(void)
{
#if 0
	Frame *frame = carousel()->subpageNumbered(0x19700);
	frame->clear();
	/* row 1 */
	for(int i = 0; i < 19; i++)
	{
		frame->alpha(1);
		frame->alpha(0);
	}
	frame->puts("01");
	/* row 2 */
	frame->mosaic(7);
	frame->holdMosaic();
	frame->doubleSize();
#endif
}

void
DemoSource::updateEngineeringTest(void)
{
}

void
DemoSource::generateDemoFrame(void)
{
	if(nullptr == demoProcessor_)
	{
		demoProcessor_ = Processor::newProcessorOfKind(Processor::Raw, carousel());
		demoProcessor_->setFrame(carousel()->subpageNumbered(0x10000, true));
	}
	demoProcessor_->clear();
	for(int y = 1; y <= MODE7_TEXTROWS; y++)
	{
		processor()->gotoxy(1, y);
		/* white yellow cyan green magenta red blue black */
		processor()->puts("\207\235   \203\235   \206\235   \202\235   \205\235   \201\235   \204\235    \234   ");
	}
	updateDemoFrame();
}

void
DemoSource::updateDemoFrame(void)
{
	char buf[MODE7_WIDTH];

	if(nullptr == demoProcessor_)
	{
		return;
	}
	demoProcessor_->gotoxy(13, 20);
	/* <BBG> <DH> <A7> HH:MM:SS <A1> <NBG> */
	strftime(buf, sizeof(buf), "\234\215\207%H:%M:%S", &utc_);
	demoProcessor_->puts(buf);
	demoProcessor_->gotoxy(13 + strlen(buf), 20);
	snprintf(buf, sizeof(buf), ".%02d  \201\235", Clock::sharedClock()->now().field);
	demoProcessor_->puts(buf);
}

void
DemoSource::generateInfoFrame(void)
{
	if(nullptr == infoProcessor_)
	{
		infoProcessor_ = Processor::newProcessorOfKind(Processor::Teletext, carousel());
		infoProcessor_->setFrame(carousel()->subpageNumbered(0x10100, true));
	}
	struct utsname name;
	char *t;
	char buf[128];

	uname(&name);
	infoProcessor_->clear();
	if(nullptr != (t = strchr(name.nodename, '.')))
	{
		*t = 0;
	}
	snprintf(buf, sizeof(buf), "\nHello\r\n\nThis is the second page of the Teletext demo on %s (%s %s)\r\n\n", name.nodename, name.sysname, name.machine);
	infoProcessor_->puts(buf);
}

void
DemoSource::updateInfoFrame(void)
{
	char buf[MODE7_WIDTH];

	if(nullptr == infoProcessor_)
	{
		return;
	}
	strftime(buf, sizeof(buf), "The time now is %H:%M:%S UTC", &utc_);
	infoProcessor_->gotoxy(1, 8);
	infoProcessor_->puts(buf);	
	strftime(buf, sizeof(buf), "   ... which is %H:%M:%S %Z", &localtime_);
	infoProcessor_->gotoxy(1, 10);
	infoProcessor_->puts(buf);	
}
