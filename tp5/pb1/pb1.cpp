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
    uint8_t donnee[] = "Polytechnique";
    uint8_t size = sizeof(donnee)/sizeof(uint8_t);
    uint8_t motRetour[size] = "";
    Memoire24CXXX memoire;
    memoire.ecriture(0, donnee, size);
    _delay_ms(20);
    memoire.lecture(0, motRetour, size);

    if(donnee[0] == motRetour[0]) {
        vert();
    }
    else {
        rouge();
    }
}