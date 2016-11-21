#include <avr/io.h>
#include "peri.h"

#define RED    PC0
#define YELLOW PC1
#define GREEN  PC2

#define ON     1
#define OFF    0

void init_peripheral()
{
    // วางโค้ดจาก elab
	DDRC |= (1<<PC0) | (1<<PC1) | (1<<PC2);
	PORTC = PORTC & ~(1<<2 | 1<<1 | 1<<0);
	DDRC = DDRC & ~(1<<PC3);
	PORTC = PORTC | (1<<PC3);
	DDRC = DDRC & ~(1<<PC4);
	PORTC = PORTC & ~(1<<PC4);
}

void set_led(uint8_t pin, uint8_t state)
{
    // วางโค้ดจาก elab
	if(state == ON) {
   if(pin == RED) {
       PORTC = PORTC | (ON << RED);
   }
   else if(pin == YELLOW) {
       PORTC = PORTC | (ON << YELLOW);
   }
   else if(pin == GREEN) {
       PORTC = PORTC | (ON << GREEN);
   }
}
else if(state == OFF) {
  	if(pin == RED) {
       		PORTC = PORTC & ~(ON << RED);
	}
	else if(pin == YELLOW) {
       		PORTC = PORTC & ~(ON << YELLOW);
   	}
   	else if(pin == GREEN) {
      		PORTC = PORTC & ~(ON<< GREEN);
	}
}
}

void set_led_value(uint8_t value)
{
    // วางโค้ดจาก elab
	PORTC &= ~((1<<PC0) | (1<<PC1) | (1<<PC2));
	PORTC |= ((1<<PC0) | (1<<PC1) | (1<<PC2)) & value;
}

uint16_t read_adc(uint8_t channel)
{
    // วางโค้ดจากสไลด์บรรยาย
	ADMUX = (0<<REFS1)|(1<<REFS0)
		| (0<<ADLAR)
		| (channel & 0b1111);
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0)
		| (1<<ADSC);
	while ((ADCSRA & (1<<ADSC)));
		return ADCL + ADCH*256; 
	}	

uint16_t get_light()
{
    // วางโค้ดจาก elab
    read_adc(PC4);
}
