#include <stdint.h>
#include "oled.h"

typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
} time_t;

void increase_time(time_t *time);
void decrease_time(time_t *time);
void display_time(time_t *time, int x, int y);
int is_zero(time_t *time);