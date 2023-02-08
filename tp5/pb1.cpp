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
    uint8_t size = uint8_t(3);
    Memoire24CXXX memoire;
    memoire.ecriture(1, chr);
    uint8_t motSortie = memoire.lecture(1, &size);
    if(motSortie == 'o') {
        while(true){
            vert();
        }
    } /*else {
        while(true){
            rouge();
        }
    }*/
}