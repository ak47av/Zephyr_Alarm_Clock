#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/rtc.h>
#include <zephyr/display/cfb.h>
#include "oled.h"
#include "state_machine.h"

///////////////////////////// VARIABLES /////////////////////////


///////////////////////////// VARIABLES /////////////////////////

///////////////////////////// FUNCTIONS /////////////////////////

static int init_devices();
static void event_dispatcher(alarm_t *const mobj, event_t const *const e);
static void state_table_init(alarm_t *const mobj);

///////////////////////////// FUNCTIONS /////////////////////////