#ifndef MODE7_TIME_H_
# define MODE7_TIME_H_                 202505L

# include <stdint.h>
# include <time.h>
# include <sys/time.h>

typedef struct MODE7_Time_struct MODE7_Time;

# if defined(__cplusplus)
namespace MODE7
{
	typedef struct ::MODE7_Time_struct Time;
}
# endif

struct MODE7_Time_struct
{
	time_t secs;   /* reference time as a unixtime */
	int field;     /* current field (0..49 for 50 Hz) */
	int line;      /* current line (typically 0..15) */
};

#endif /*!MODE7_TIME_H_*/