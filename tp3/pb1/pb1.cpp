#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20

void rouge(){
    PORTA &= ~(1 << PORTA0);
    PORTA |= (1 << PORTA1);
}

void vert(){
    PORTA |= (1 << PORTA0);
    PORTA &= ~(1 << PORTA1);
}

void eteint(){
    PORTA &= ~(1 << PORTA0);
    PORTA &= ~(1 << PORTA1);
}

void busyWaitDelay(uint16_t delay) {
  for(uint8_t i = 0; i < delay; ++i){
    _delay_us(1);
    //_delay_ms(1); //Au choix
    }
}

int main() {
    DDRA = DDR_OUT;
    double n = 1000;
    for(uint8_t i = 0; i < 3000; i++){
        n -= 1/3;

        rouge();

        busyWaitDelay(n);

        eteint();

        busyWaitDelay(1000 - n);
    }
}