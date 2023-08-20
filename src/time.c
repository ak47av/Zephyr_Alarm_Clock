#include "time.h"
#include <stdlib.h>
#include <stdio.h> 

/* This function increases the clock timer */
void increase_time(time_t *time)
{
	time->second++;
	if (time->second == 60) {
		time->minute++;
		time->second = 0;
		if (time->minute == 60) {
			time->hour++;
			time->minute = 0;
			if (time->hour == 24) {
				time->hour = 0;
			}
		}
	}
}

/* This function decreases the alarm timer */
void decrease_time(time_t *time)
{
	if ((time->hour == 0) && (time->minute == 0) && (time->second == 0)) {
		return;
	}

	if (time->second == 0) {
		time->second = 59;
		if (time->minute == 0) {
			time->minute = 59;
			if (time->hour != 0) {
				time->hour--;
			}
		} else {
			time->minute--;
		}
	} else {
		time->second--;
	}
}

void display_time(time_t *time, int x, int y)
{
    static char msg_buf[15];
    sprintf(msg_buf, "%02d:%02d:%02d", time->hour, time->minute, time->second);
    print_message(msg_buf, x, y);
}

int is_zero(time_t *time)
{
    if((time->second==0) && (time->minute==0) && (time->hour==0))
        return 1;
} 