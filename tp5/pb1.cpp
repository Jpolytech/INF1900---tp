#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>
#include <memoire_24.h>

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

int main() {
    DDRA = DDR_OUT;
    eteint();
    const uint8_t chr = 'o';
    uint8_t size;
    Memoire24CXXX memoire;
    memoire.ecriture(0, chr);
    _delay_ms(20);
    memoire.lecture(0, &size);
    if(chr == size) {
            vert();
    }
    else {
        rouge();
    }
}