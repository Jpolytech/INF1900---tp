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

void initialisationUART ( void ) {
    // 2400 bauds. Nous vous donnons la valeur des deux
    // premiers registres pour vous éviter des complications.
    UBRR0H = 0;
    UBRR0L = 0xCF;
    // permettre la réception et la transmission par le UART0
    UCSR0A = 0;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    // Format des trames: 8 bits, 1 stop bits, sans parité
    UCSR0C = (3<<UCSZ00);
}

// Du USART vers le PC
void transmissionUART ( uint8_t donnee ) {
    while ( !( UCSR0A & (1<<UDRE0)) ){}
    UDR0 = donnee;
}

int main() {
    DDRA = DDR_OUT;
    eteint();

    initialisationUART();

    uint8_t donnee[] = "Polytechnique";
    uint8_t size = sizeof(donnee)/sizeof(uint8_t);
    uint8_t* motRetour(0);
    Memoire24CXXX memoire;
    memoire.ecriture(0, donnee, size);
    _delay_ms(20);
    uint8_t adrCourante = 0x0000; //adresse de depart

    while(*motRetour!=0xFF) {
        memoire.lecture(adrCourante, motRetour);
        transmissionUART(*motRetour);
        adrCourante+=1;
    }
}