#ifndef P_MODE7_H_
# define P_MODE7_H_                    202505L

/* this file is included by both C and C++ sources */

# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <string.h>
# include <assert.h>
# include <string.h>
# include <errno.h>

# include <unistd.h>
# include <time.h>

# include <MODE7/MODE7.h>

# if defined(__cplusplus)

/* Packets & buffers */
#  include <MODE7/Internal/T45Packets.hh>
#  include <MODE7/Internal/T42Packets.hh>
#  include <MODE7/Internal/DebugPackets.hh>

static inline void DPRINTF(const char *format, ...)
{
#if NDEBUG
	(void) format;
#else /*NDEBUG*/
	va_list ap;
	char buf[256];

//	fputs("\033[90m[Debug] ", stderr);
	va_start(ap, format);
	vsnprintf(buf, sizeof(buf) - 2, format, ap);
	va_end(ap);
	strcat(buf, "\r\n");
	/* don't use stdio for this, it gets flaky once we flip the terminal into
	 * raw mode
	 */
	for(size_t i = 0; i < strlen(buf); i++)
	{
		ssize_t r;
		do
		{
			r = write(STDERR_FILENO, &buf[i], 1);
		}
		while(r == -1 && errno == EINTR);
	}
//	fwrite(buf, strlen(buf), 1, stderr);
//	fputs("\033[0m\r\n", stderr);
#endif /*!NDEBUG*/
}

static inline void *xalloc(size_t bytes)
{
	void *ptr;

	ptr = calloc(1, bytes);
	assert(ptr != nullptr);
	if(!ptr)
	{
		abort();
	}
	return ptr;
}

static inline void *xrealloc(void *ptr, size_t bytes)
{
	void *newptr;

	newptr = realloc(ptr, bytes);
	assert(newptr != nullptr);
	if(!newptr)
	{
		abort();
	}
	return newptr;
}

# endif /*__cplusplus*/

#endif /*!P_MODE7_H_*/
