#include <avr/io.h>
#include <util/delay.h>
#include "peri.h"

int main() {
        init_peripheral();
	uint8_t count = 0;
        set_led_value(0);
	while(1) {
		while(!IS_SWITCH_PRESSED()) {	
			//set_led_value(count);
			//_delay_ms(200);
			if(IS_SWITCH_PRESSED())
				break;
		}
		while(IS_SWITCH_PRESSED()){
	   		//set_led_value(count);
		//	_delay_ms(500);
			if(!IS_SWITCH_PRESSED()){
				break;
			}
		}
		count++;
		set_led_value(count);
		_delay_ms(100);
   	}
}

