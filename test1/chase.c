#include <avr/io.h>
#include <util/delay.h>
#include "peri.h"

int main() {
	init_peripheral();
	while(1) {
		set_led_value(0b001);
		_delay_ms(500);
		set_led_value(0b010);
		_delay_ms(500);
		set_led_value(0b0100);
		_delay_ms(500);
	}
}
