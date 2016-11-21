#include <stddef.h>         /* for NULL */
#include <avr/io.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <avr/pgmspace.h>   /* required by usbdrv.h */

#include "pt/pt.h"
#include "peri.h"
#include "timer.h"
#include "usbdrv.h"

#define RQ_SET_LED           0
#define RQ_SET_LED_VALUE     1
#define RQ_GET_SWITCH        2
#define RQ_GET_LIGHT         3
#define RQ_SET_CHASE         4
#define RQ_GET_SWITCH_COUNT  5

// นิยามมาโคร PT_DELAY ไว้เพื่อจำลองการหน่วงเวลาแบบมัลติทาสกิ้ง
#define PT_DELAY(pt, ms, ts) \
    ts = timer_millis(); \
    PT_WAIT_WHILE(pt, timer_millis()-ts < (ms));
 
struct pt pt_taskChase;
struct pt pt_taskCountSwitch;

uint16_t chase_delay;
uint8_t  switch_count;

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */
usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
    usbRequest_t *rq = (void *)data;

    /* declared as static so they stay valid when usbFunctionSetup returns */
    static uint8_t switch_state;  
    static uint16_t light_value;

    if (rq->bRequest == RQ_SET_LED)
    {
        uint8_t led_val = rq->wValue.bytes[0];
        uint8_t led_no  = rq->wIndex.bytes[0];
        set_led(led_no, led_val);
        return 0;
    }

    else if (rq->bRequest == RQ_SET_LED_VALUE)
    {
        uint8_t led_value = rq->wValue.bytes[0];
        set_led_value(led_value);
        return 0;
    }

    else if (rq->bRequest == RQ_GET_SWITCH)
    {
        switch_state = IS_SW_PRESSED();

        /* point usbMsgPtr to the data to be returned to host */
        usbMsgPtr = &switch_state;

        /* return the number of bytes of data to be returned to host */
        return 1;
    }

    else if (rq->bRequest == RQ_GET_LIGHT)
    {
        light_value = get_light();
        usbMsgPtr = (uchar*) &light_value;
        return sizeof(light_value);
    }

    else if (rq->bRequest == RQ_SET_CHASE)
    {
        chase_delay = rq->wValue.word;
        return 0;
    }

    else if (rq->bRequest == RQ_GET_SWITCH_COUNT)
    {
        static uint8_t count;

        count = switch_count;
        switch_count = 0;
        usbMsgPtr = &count;
        return 1;
    }

    /* default for not implemented requests: return no data back to host */
    return 0;
}

/* ------------------------------------------------------------------------- */
PT_THREAD(taskChase(struct pt* pt))
{
    static uint32_t ts;

    PT_BEGIN(pt);
    chase_delay = 0;

    for (;;)
    {
        PT_WAIT_UNTIL(pt,chase_delay > 0);
        set_led_value(0b001);
        PT_DELAY(pt,chase_delay,ts);
        set_led_value(0b010);
        PT_DELAY(pt,chase_delay,ts);
        set_led_value(0b100);
        PT_DELAY(pt,chase_delay,ts);
    }

    PT_END(pt);
}

/* ------------------------------------------------------------------------- */
PT_THREAD(taskCountSwitch(struct pt* pt))
{
    static uint32_t ts;

    PT_BEGIN(pt);

    switch_count = 0;
    for (;;)
    {
        PT_WAIT_UNTIL(pt,IS_SW_PRESSED());
        PT_DELAY(pt,10,ts); // debouncing
        switch_count++;
        PT_WAIT_WHILE(pt,IS_SW_PRESSED());
        PT_DELAY(pt,10,ts); // debouncing
    }

    PT_END(pt);
}

/* ------------------------------------------------------------------------- */
int main(void)
{
    init_peripheral();

    usbInit();

    /* enforce re-enumeration, do this while interrupts are disabled! */
    usbDeviceDisconnect();
    _delay_ms(300);
    usbDeviceConnect();

    timer_init();

    /* enable global interrupts */
    sei();

    PT_INIT(&pt_taskChase);
    PT_INIT(&pt_taskCountSwitch);

    /* main event loop */
    for(;;)
    {
        usbPoll();
        taskChase(&pt_taskChase);
        taskCountSwitch(&pt_taskCountSwitch);
    }

    return 0;
}

/* ------------------------------------------------------------------------- */
