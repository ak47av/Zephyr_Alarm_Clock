#include "state_machine.h"
#include <zephyr/kernel.h>

#define ZERO_TIME {0, 0, 0}
#define ALARM_TIME {0, 45, 1}

void alarm_init(alarm_t *mobj){
    event_t ee;
    e_handler_t ehandler;
    ee.sig = ENTRY;
    mobj->active_state = IDLE;

    time_t zero_time = {.second=0, .minute=0, .hour=0};
    time_t alarm_time = {.second=0, .minute=1, .hour=0};
    mobj->curr_time = zero_time;
    mobj->remaining_time = alarm_time; 
    ehandler = (e_handler_t) mobj->state_table[IDLE * MAX_SIGNALS + ENTRY];
    (*ehandler)(mobj,&ee);
}

event_status_t IDLE_ENTRY(alarm_t *const mobj, event_t const *const e)     
{
    printk("IDLE_ENTRY");
    display_time(&mobj->curr_time, 0, 0);
    //print_message("Next alarm in: ", 0, 21);
    //display_time(&mobj->remaining_time, 0, 42);
    return EVENT_HANDLED;
}

event_status_t IDLE_EXIT(alarm_t *const mobj, event_t const *const e)
{
    printk("IDLE_EXIT\n");
    display_clear();
    return EVENT_HANDLED;
}

event_status_t IDLE_TIME_TICK(alarm_t *const mobj, event_t const *const e)
{
    printk("IDLE_TIME_TICK\n");
    if( ((tick_event_t *)(e))->ss == 10){
        increase_time(&mobj->curr_time);
        decrease_time(&mobj->remaining_time);
        if(is_zero(&mobj->remaining_time))
        {
            mobj->active_state = ALARM;
            return EVENT_TRANSITION;
        }
        display_time(&mobj->curr_time, 0, 0);
       // print_message("Next alarm in: ", 0, 21);
       // display_time(&mobj->remaining_time, 0, 42);
        return EVENT_HANDLED;
    }
    return EVENT_IGNORED;
}

event_status_t ALARM_ENTRY(alarm_t *const mobj, event_t const *const e)
{
    printk("ALARM_ENTRY\n");
    display_time(&mobj->curr_time, 0, 32);
    return EVENT_HANDLED;
}  

event_status_t ALARM_EXIT(alarm_t *const mobj, event_t const *const e)
{
    printk("ALARM_EXIT\n");
    display_clear();
    return EVENT_HANDLED;
}

event_status_t ALARM_SNOOZE(alarm_t *const mobj, event_t const *const e) 
{
    printk("ALARM_SNOOZE\n");
    mobj->active_state = IDLE;
    return EVENT_TRANSITION;
}
