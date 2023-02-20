//TP6-probleme 2
//code écrit par Thibaut Delahaie (2157153) et Jean-Philippe Salis Routhier (2201275)
//Pour le cours de INF1900-Projet integrateur 1: Systemes embarqués
//à la session d'hiver 2023

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "can.h"

#define DDR_IN 0x00
#define DDR_OUT 0xff

void rouge(){
    PORTB &= ~(1 << PORTB0);
    PORTB |= (1 << PORTB1);
}

void vert(){
    PORTB |= (1 << PORTB0);
    PORTB &= ~(1 << PORTB1);
}

void eteint(){
    PORTB &= ~(1 << PORTB0);
    PORTB &= ~(1 << PORTB1);
}

void ambree() {
    vert();
    _delay_us(60);
    rouge();
    _delay_us(5);
}

void initialisation ( void ) {
    DDRB = DDR_OUT;// PORT B est en mode sortie
    DDRA = DDR_IN; // PORT A est en mode entrée
}

int main() {
    initialisation();

    can adc = can();
    uint8_t adcRetour;

    while(true) {
        adcRetour = adc.lecture(1);
        adcRetour = adcRetour >> 2; //On garde 8 bits
        if(adcRetour < 25) {
            vert();
        }
        else if(adcRetour >= 25 && adcRetour < 50){
            ambree();
        }
        else {
            rouge();
        }
    }
}