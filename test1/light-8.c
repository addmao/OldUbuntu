#include <avr/io.h>
#include <util/delay.h>
#include "peri.h"

int main() {
        init_peripheral();
        while(1) {
                uint16_t result = read_adc(PC4);
                _delay_ms(200);
                uint16_t result1 = 1024 / 8;
                uint16_t result2 = (1024 / 8) * 2;
		uint16_t result3 = (1024 / 8) * 3;
		uint16_t result4 = (1024 / 8) * 4;
		uint16_t result5 = (1024 / 8) * 5;
		uint16_t result6 = (1024 / 8) * 6;
		uint16_t result7 = (1024 / 8) * 7;
		uint16_t result8 = (1024 / 8) * 8;
		if(result < result1) {
                        set_led_value(0b000);
                        _delay_ms(500);
                }
                else if(result >= result1 && result < result2) {
                        set_led_value(0b001);
                        _delay_ms(500);
                }
		else if(result >= result2 && result < result3) {
                        set_led_value(0b010);
                        _delay_ms(500);
                }
		else if(result >= result3 && result < result4) {
                        set_led_value(0b011);
                        _delay_ms(500);
                }
		else if(result >= result4 && result < result5) {
                        set_led_value(0b100);
                        _delay_ms(500);
                }
		else if(result >= result5 && result < result6) {
                        set_led_value(0b101);
                        _delay_ms(500);
                }
		else if(result >= result6 && result < result7) {
                        set_led_value(0b110);
                        _delay_ms(500);
                }
                else if(result >= result7) {
                        set_led_value(0b111);
                        _delay_ms(500);
                }
        }
}

