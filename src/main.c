#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ch32v00x.h>
#ifdef USE_SPL
#include <ch32v00x_rcc.h>
#include <ch32v00x_gpio.h>
#endif
#include "twi.h"
#include "ssd1306.h"
#include "ina226.h"

const char *MILLI = "m";
const char *EMPTY = "";

static void normalizeU32(char *str, uint8_t size, uint32_t value, const char *suffix) {
    const char *prefix;

    if (value < 1000000) {
        prefix = MILLI;
    } else {
        prefix = EMPTY;
        value /= 1000;
    }
    snprintf(str, size, "%u.%03u %s%s", (uint16_t)(value / 1000), (uint16_t)(value % 1000), prefix, suffix);
}

static void normalizeI32(char *str, uint8_t size, int32_t value, const char *suffix) {
    const char *prefix;

    if (labs(value) < 1000000) {
        prefix = MILLI;
    } else {
        prefix = EMPTY;
        value /= 1000;
    }
    snprintf(str, size, "%d.%03u %s%s", (int16_t)(value / 1000), (uint16_t)(labs(value) % 1000), prefix, suffix);
}

int main(void) {
    TWI_Init(400000);

    if (! ssd1306_begin()) {
        while (1) {}
    }

    if ((! ina226_begin()) || (! ina226_measure(true, AVG64, US8244, US8244))) {
        screen_printstr_x2("INA226", 0, 8, 1);
        screen_printstr_x2("not ready!", 0, 40, 1);
        ssd1306_flush(true);
        while (1) {}
    } else {
        screen_printstr_x2("Wait...", 0, 24, 1);
        ssd1306_flush(true);
    }

    const char PROGRESS[4] = { '-', '\\', '|', '/' };

    uint64_t totalMicroAmps = 0;
    uint32_t totalTime = 0;

    while(1) {
        if (ina226_ready()) {
            char str[16];
            int32_t microAmps;
            uint32_t microWatts;
            uint16_t milliVolts;

            milliVolts = ina226_getMilliVolts();
            microAmps = ina226_getMicroAmps();
            microWatts = ina226_getMicroWatts();
            totalMicroAmps += labs(microAmps);
            ++totalTime;
            screen_clear();
            screen_printchar(PROGRESS[totalTime & 0x03], SCREEN_WIDTH - FONT_WIDTH, 0, 1);
            snprintf(str, sizeof(str), "%u.%03u V", milliVolts / 1000, milliVolts % 1000);
            screen_printstr_x2(str, 0, 0, 1);
            normalizeI32(str, sizeof(str), microAmps, "A");
            screen_printstr_x2(str, 0, 16, 1);
            normalizeU32(str, sizeof(str), microWatts, "W");
            screen_printstr_x2(str, 0, 32, 1);
            normalizeU32(str, sizeof(str), (uint32_t)((totalMicroAmps * 3600000000ULL / 1055232) / totalTime), "A/h");
            screen_printstr_x2(str, 0, 48, 1);
            ssd1306_flush(true);
        }
    }
}
