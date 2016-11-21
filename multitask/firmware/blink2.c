#include <stddef.h>         /* for NULL */
#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */

#include "pt/pt.h"
#include "peri.h"
#include "timer.h"

// นิยามมาโคร PT_DELAY ไว้เพื่อจำลองการหน่วงเวลาแบบมัลติทาสกิ้ง
#define PT_DELAY(pt, ms, ts) \
    ts = timer_millis(); \
    PT_WAIT_WHILE(pt, timer_millis()-ts < (ms));

struct pt pt_taskGreen;
struct pt pt_taskRed;

////////////////////////////////////////////////
PT_THREAD(taskGreen(struct pt* pt))
{
    static uint32_t ts;

    PT_BEGIN(pt);

    for (;;)
    {
        set_led(LED_GREEN,1);
        PT_DELAY(pt, 1000, ts);
        set_led(LED_GREEN,0);
        PT_DELAY(pt, 500, ts);
    }

    PT_END(pt);
}

////////////////////////////////////////////////
PT_THREAD(taskRed(struct pt* pt))
{
    static uint32_t ts;

    PT_BEGIN(pt);

    for (;;)
    {
        set_led(LED_RED,1);
        PT_DELAY(pt, 700, ts);
        set_led(LED_RED,0);
        PT_DELAY(pt, 300, ts);
    }

    PT_END(pt);
}

////////////////////////////////////////////////
int main()
{
    init_peripheral();
    timer_init();
    sei();

    PT_INIT(&pt_taskGreen);
    PT_INIT(&pt_taskRed);

    for (;;)
    {
        taskGreen(&pt_taskGreen);
        taskRed(&pt_taskRed);
    }
}
