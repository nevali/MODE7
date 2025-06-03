#include "../p_MODE7.h"

#include <MODE7/Internal/SubProcessSource.hh>

#include <spawn.h>
#include <fcntl.h>
#include <ctype.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

using namespace MODE7;
using namespace MODE7::Internal;

SubProcessSource::SubProcessSource(Processor *processor, int argc, char **argv): Source(processor),
	frame_(nullptr), parent_(-1), child_(-1), bufsize_(1023), buffer_(nullptr)
{
	(void) argc;
	(void) argv;

	buffer_ = (uint8_t *) xalloc(bufsize_ + 1);
	devname_[0] = 0;
	frame_ = carousel()->subpageNumbered(0x10000);
	if((parent_ = posix_openpt(O_RDWR|O_NOCTTY)) < 0)
	{
		fprintf(stderr, "ERROR: SubProcessSource: posix_openpt() failed: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
	if(grantpt(parent_) < 0)
	{
		fprintf(stderr, "ERROR: SubProcessSource: grantpt() failed: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
	if(unlockpt(parent_) < 0)
	{
		fprintf(stderr, "ERROR: SubProcessSource: unlockpt() failed: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
	if(ptsname_r(parent_, devname_, sizeof(devname_)) < 0)
	{
		fprintf(stderr, "ERROR: SubProcessSource: ptsname_r() failed: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
	int flags = fcntl(parent_, F_GETFL, 0);
	if(fcntl(parent_, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		fprintf(stderr, "ERROR: SubProcessSource: fcntl(parent) failed: %s (#%d)\n", strerror(errno), errno);
		abort();
	}

	DPRINTF("--- SubProcess: pseudoteminal device is '%s'", devname_);
	child_ = open(devname_, O_RDWR|O_NOCTTY);
	if(child_ < 0)
	{
		fprintf(stderr, "ERROR: SubProcessSource: open() failed: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
	flags = fcntl(child_, F_GETFL, 0);
	if(fcntl(child_, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		fprintf(stderr, "ERROR: SubProcessSource: fcntl(parent) failed: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
	flags = fcntl(0, F_GETFL, 0);
	if(fcntl(0, F_SETFL, flags | O_NONBLOCK) < 0)
	{
		fprintf(stderr, "ERROR: SubProcessSource: fcntl(stdin) failed: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
	posix_spawn_file_actions_t actions;
	posix_spawnattr_t attr;

	posix_spawn_file_actions_init(&actions);
	posix_spawn_file_actions_adddup2(&actions, child_, 0);
	posix_spawn_file_actions_adddup2(&actions, child_, 1);
	posix_spawn_file_actions_adddup2(&actions, child_, 2);
	posix_spawn_file_actions_addclose(&actions, parent_);
	posix_spawn_file_actions_addclose(&actions, child_);

	posix_spawnattr_init(&attr);

	if(posix_spawnp(&pid_, argv[0], &actions, &attr, argv, nullptr) != 0)
	{
		fprintf(stderr, "ERROR: SubProcessSource: posix_spawn() failed: %s (#%d)\n", strerror(errno), errno);
		abort();
	}
	posix_spawn_file_actions_destroy(&actions);
	posix_spawnattr_destroy(&attr);
	close(child_);
	child_ = -1;
	/* now set stdin to raw mode */
	if(isatty(STDIN_FILENO))
	{
	    if (tcgetattr(STDIN_FILENO, &termstate_) == -1)
		{
			fprintf(stderr, "ERROR: SubProcessSource: tcgetattr() failed: %s (#%d)\n", strerror(errno), errno);
			abort();
	    }
		struct termios newstate = termstate_;

		/* input modes - no break, no CR to NL, no parity check, no strip char,
		* no start/stop output control. */
		newstate.c_iflag &= ~(0u | IGNBRK | BRKINT | PARMRK | INPCK | ISTRIP | ICRNL | 
						INLCR | IGNCR | IXON);

		/* local modes - echoing off, canonical off, no extended functions, 
		* no signal chars (^Z, ^C) */
		newstate.c_lflag &= ~(0u | ISIG | IEXTEN | ECHO | ICANON);

		/* control modes - set 8 bit chars. */
		newstate.c_cflag |= (0u | CS8);

		/* output modes - disable post processing. */
//		newstate.c_oflag &= ~(0u | OPOST);

		/* control chars - set return condition: min number of bytes and timer.
		* We want read(2) to return every single byte, without timeout. */
		newstate.c_cc[VMIN] = 1;         /* 1 byte */
		newstate.c_cc[VTIME] = 0;        /* No timer */

		/* Change attributes when output has drained; also flush pending input. */
		if(tcsetattr(STDIN_FILENO, TCSAFLUSH, &newstate) < 0)
		{
			fprintf(stderr, "ERROR: SubProcessSource: tcsetattr() failed: %s (#%d)\n", strerror(errno), errno);
			abort();
		}
	}
}

SubProcessSource::~SubProcessSource()
{
	if(isatty(STDIN_FILENO))
	{
	    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &termstate_) == -1)
		{
			fprintf(stderr, "ERROR: SubProcessSource: warning: tcsetattr() failed: %s (#%d)\n", strerror(errno), errno);
	    }
	}
	if(child_ >= 0)
	{
		close(child_);
	}
	if(parent_ >= 0)
	{
		close(parent_);
	}
	if(pid_ > 0)
	{
		int sig = SIGHUP, count, status;
		pid_t waitres;

		count = 0;
		while((waitres = waitpid(pid_, &status, WNOHANG)) == 0)
		{
			int r;

			DPRINTF("<<< SubProcessSource: sending signal #%d to child %ld\n", sig, (long) pid_);
			r = kill(pid_, sig);
			count++;
			if(0 == r)
			{
				usleep(250000);
				if(count > 4)
				{
					sig = SIGKILL;
				}
				else if(count > 2)
				{
					sig = SIGTERM;
				}
				continue;
			}
			if(-1 == r && errno == ESRCH)
			{
				pid_ = -1;
				break;
			}
			fprintf(stderr, "kill() failed: %s\n", strerror(errno));
			break;
		}
		if(waitres > 0)
		{
			if(WIFEXITED(status))
			{
				fprintf(stderr, "SubProcessSource: child exited with status %d\n", WEXITSTATUS(status));
			}
			else if(WIFSIGNALED(status))
			{
				fprintf(stderr, "SubProcessSource: child terminated due to signal %d\n", WTERMSIG(status));
			}
		}
		else if(waitres < 0)
		{
			fprintf(stderr, "SubProcessSource: WARNING: child disappeared\n");
		}
	}
	free(buffer_);
}

/* read from stdin and send to parent
 * read from parent and send to frame and stderr
 */
bool
SubProcessSource::processPendingEvents(void)
{
	ssize_t r;
	int status;
	pid_t waitres;

	if(pid_ < 1)
	{
		return false;
	}
	waitres = waitpid(pid_, &status, WNOHANG);
	if(waitres < 0)
	{
		fprintf(stderr, "SubProcessSource: child vanished\n");
		pid_ = -1;
		return false;
	}
	if(waitres > 0)
	{
		if(WIFEXITED(status))
		{
			fprintf(stderr, "SubProcessSource: child exited with status %d\n", WEXITSTATUS(status));
			pid_ = -1;
			return false;
		}
		else if(WIFSIGNALED(status))
		{
			fprintf(stderr, "SubProcessSource: child terminated due to signal %d\n", WTERMSIG(status));
			pid_ = -1;
			return false;
		}
	}
	if((r = readFrom(parent_)) < 0)
	{
		Clock::sharedClock()->stop();
		return false;
	}
	if(r > 0)
	{
		processor()->putOctets(buffer_, r);
		for(ssize_t i = 0; i < r; i++)
		{
			ssize_t res;
			do
			{
				res = write(STDERR_FILENO, &buffer_[i], 1);
			}
			while(res == -1 && errno == EINTR);
		}
//		DPRINTF("... read %zd from subprocess", r);
	}
	if((r = readFrom(STDIN_FILENO)) < 0)
	{
		Clock::sharedClock()->stop();
		return false;
	}
	if(r > 0)
	{
		ssize_t wr, i;

		for(i = 0; i < r; i++)
		{
			if(buffer_[i] == '\r' || (buffer_[i] > 31 && buffer_[i] < 127))
			{
				do
				{
					wr = write(parent_, buffer_, 1);	
				}
				while(wr == -1 && errno == EINTR);
				if(wr != 1)
				{
					fprintf(stderr, "ERROR: SubProcess: failed to write to stream (read=%zd, written=%zd): %s (#%d)\n",
						r, i, strerror(errno), errno);
					return false;
				}
			}
			else if(buffer_[i] == 0x03)
			{
				/* Ctrl+C */
				DPRINTF("!!! Break");
				if(pid_ > 0)
				{
					kill(pid_, SIGHUP);
				}
				return false;
			}
		}
//		DPRINTF("... wrote %zd bytes to parent", i);
	}
	return true;
}

ssize_t
SubProcessSource::readFrom(int fd)
{
	ssize_t r;

	r = read(fd, buffer_, bufsize_);
	if(r < 0)
	{
		switch(errno)
		{
			case EAGAIN:
			case EINTR:
				return 0;
			default:
				DPRINTF("!!! SubProcessSource: read(parent): %s (#%d)", strerror(errno), errno);
				return -1;
		}
	}
	else if(r > 0)
	{
		return r;
	}
	return 0;
}
