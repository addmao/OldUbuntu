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
struct pt pt_taskSwitch;

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
PT_THREAD(taskSwitch(struct pt* pt))
{
    static uint32_t ts;
    static uint8_t  status;

    PT_BEGIN(pt);

    status = 0;
    for (;;)
    {
        PT_WAIT_UNTIL(pt,IS_SW_PRESSED());
        PT_DELAY(pt,10,ts); // debouncing
        status = (status+1) % 2;
        set_led(LED_YELLOW,status);
        PT_WAIT_WHILE(pt,IS_SW_PRESSED());
        PT_DELAY(pt,10,ts); // debouncing
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
    PT_INIT(&pt_taskSwitch);

    for (;;)
    {
        taskGreen(&pt_taskGreen);
        taskRed(&pt_taskRed);
        taskSwitch(&pt_taskSwitch);
    }
}
