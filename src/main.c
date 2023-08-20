#define DEBUG 1

#include "main.h"

#define MILLIS_PER_SECOND 100 // for testing purposes
#define DEBOUNCE_DELAY_MS 500

// get the OLED controller Device tree struct from our custom overlay file
const struct device *display = DEVICE_DT_GET(DT_NODELABEL(ssd1306));

// get the OLED controller Device tree struct from our custom overlay file
const struct gpio_dt_spec snooze_button = GPIO_DT_SPEC_GET(DT_NODELABEL(snooze_button), gpios);
static struct gpio_callback snooze_cb_data;

static alarm_t alarm;

LOG_MODULE_REGISTER(display);

void main(void)
{
	int ret;
	ret = init_devices();
	if (ret != 0) {
		LOG_ERR("could not initialize devices");
		return;
	}

	state_table_init(&alarm);
	alarm_init(&alarm);

	uint32_t last_tick = k_uptime_get_32();

	user_event_t ue;
	tick_event_t te;
	while (1)
	{
#if DEBUG
		if (k_uptime_get_32() - last_tick >= MILLIS_PER_SECOND) {
			// 100ms has passed; millis_per_second for testing quickly
			last_tick = k_uptime_get_32();
			te.super.sig = TIME_TICK;
			if (++te.ss > 10) {
				te.ss = 1;
			}
			event_dispatcher(&alarm, &te.super);
		}
#endif
	}
}

uint32_t currentMillis = 0;
uint32_t previousMillis = 0;
void hit_snooze(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    currentMillis = k_uptime_get_32();
    if((currentMillis - previousMillis > DEBOUNCE_DELAY_MS))
    {
		user_event_t ue;
		tick_event_t te;
		te.super.sig = SNOOZE;
        previousMillis = currentMillis;
		event_dispatcher(&alarm, &te.super);
    }
}

static int init_devices()
{
	int ret;
	if(!device_is_ready(snooze_button.port)) return -1;

	ret = gpio_pin_configure_dt(&snooze_button, GPIO_INPUT);
	if (ret < 0) {
		printk("Error configuring snooze pin!\n");
		return -1;
	}

	ret = gpio_pin_interrupt_configure_dt(&snooze_button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret < 0) {
		printk("Error configuring Interrupt!\n");
		return -1;
	}

	gpio_init_callback(&snooze_cb_data, hit_snooze, BIT(snooze_button.pin));
	gpio_add_callback(snooze_button.port, &snooze_cb_data);

	if (display == NULL) {
		LOG_ERR("device pointer of display is NULL");
		return -1;
	}

	if (!device_is_ready(display)) {
		LOG_ERR("display device is not ready");
		return -1;
	}

	ret = cfb_framebuffer_init(display);
	if (ret != 0) {
		LOG_ERR("could not initialize display");
		return ret;
	}

	return 0;
}

static void state_table_init(alarm_t *const mobj)
{
	static e_handler_t state_table[MAX_STATES][MAX_SIGNALS] = {
		[IDLE] = {NULL, IDLE_TIME_TICK, IDLE_ENTRY, IDLE_EXIT},
		[ALARM] = {ALARM_SNOOZE, NULL, ALARM_ENTRY, ALARM_EXIT},
	};

	mobj->state_table = (uintptr_t *)&state_table[0][0];
}

static void event_dispatcher(alarm_t *const mobj, event_t const *const e)
{
	event_status_t status;
	state_t source, target;
	e_handler_t ehandler;

	source = mobj->active_state;
	ehandler = (e_handler_t)mobj->state_table[mobj->active_state * MAX_SIGNALS + e->sig];
	if (ehandler) {
		status = (*ehandler)(mobj, e);
	}

	printk("Next handler set\n");

	if (status == EVENT_TRANSITION) {
		target = mobj->active_state;
		event_t ee;
		// 1. run the exit action for the source state
		ee.sig = EXIT;
		ehandler = (e_handler_t)mobj->state_table[source * MAX_SIGNALS + EXIT];
		if (ehandler) {
			(*ehandler)(mobj, &ee);
		}

		// 2. run the entry action for the target state
		ee.sig = ENTRY;
		ehandler = (e_handler_t)mobj->state_table[target * MAX_SIGNALS + ENTRY];
		if (ehandler) {
			(*ehandler)(mobj, &ee);
		}
	}
}
