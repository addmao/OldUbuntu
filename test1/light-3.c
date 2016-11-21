#include <avr/io.h>
#include <util/delay.h>
#include "peri.h"

int main() {
        init_peripheral();
	while(1) {
		uint16_t result = read_adc(PC4);
		_delay_ms(200);
		uint16_t result1 = 1024 / 3;
		uint16_t result2 = (1024 / 3) * 2;	
		if(result < result1) {
			set_led_value(1);
			_delay_ms(500);
		}
		else if(result >= result1 && result < result2) {
			set_led_value(2);
			_delay_ms(500);
		}
		else if(result >= result2) {
			set_led_value(0b100);
			_delay_ms(500);
		}
        }
}
