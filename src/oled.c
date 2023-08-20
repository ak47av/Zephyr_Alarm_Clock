#include "oled.h"
#include <zephyr/logging/log.h>
#include <zephyr/display/cfb.h>
#include <zephyr/kernel.h>

LOG_MODULE_DECLARE(display);

#define CLEAR_DISPLAY 1
#define OLED_DEBUG 0

int print_message(char *str, int x, int y)
{
    printk(str);
#if OLED_DEBUG
	int ret;
	ret = cfb_print(display, str, 0, 0);
	if (ret != 0) {
		LOG_ERR("Could not print to display\n");
		return ret;
	}

//	ret = cfb_invert_area(display, 0, 0, 128, 64);
//	if (ret != 0) {
//		LOG_ERR("Could not print to display\n");
//		return ret;
//	}

	ret = cfb_framebuffer_finalize(display);
	if (ret != 0) {
		LOG_ERR("Could not finalize to display\n");
		return ret;
	}

	return 0;
#endif
}

int display_clear(void)
{
#if OLED_DEBUG
    int ret;
    ret = cfb_framebuffer_clear(display, CLEAR_DISPLAY);
    if(ret!=0)
    {
        LOG_ERR("Could not clear display\n");
        return ret;
    }
    return 0;
#endif
}