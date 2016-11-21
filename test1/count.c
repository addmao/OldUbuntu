#include <avr/io.h>
#include <util/delay.h>
#include "peri.h"

int main() {
        init_peripheral();
        while(1) {
                set_led_value(1);
                _delay_ms(500);
                set_led_value(2);
                _delay_ms(500);
                set_led_value(3);
                _delay_ms(500);
		set_led_value(4);
                _delay_ms(500);
                set_led_value(5);
                _delay_ms(500);
                set_led_value(6);
                _delay_ms(500);
		set_led_value(7);
                _delay_ms(500);
        }
}

