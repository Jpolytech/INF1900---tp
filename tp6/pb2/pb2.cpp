//TP6-probleme 2
//code écrit par Thibaut Delahaie (2157153) et Jean-Philippe Salis Routhier (2201275)
//Groupe #01 du groupe de lab #1
//Pour le cours de INF1900-Projet integrateur 1: Systemes embarqués
//à la session d'hiver 2023

//Description du programme: 
//Si la lumière est basse (en cachant la photorésistance), la DEL prendra la couleur verte.
//Si la lumière est à un bon niveau (lumière ambiante), la DEL tournera à l'ambré.
//Si la lumière est trop forte (photorésistance sous une lampe de poche), la DEL devient rouge.

//Identifications matérielles:
//Une led branchée au Port B sur B0-B1
//un circuit externe contenant un photoresistor
//Le circuit externe est alimenté par les port gnd et Vcc de la carte
//Le circuit externe a sa tension (tension du photoresistor) mesurée sur le Port A sur A0.

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
    _delay_us(150);
    rouge();
    _delay_us(1);
}

void initialisation ( void ) {
    DDRB = DDR_OUT;// PORT B est en mode sortie
    DDRA = DDR_IN; // PORT A est en mode entrée
}

int main() {
    initialisation();

    can adc = can();
    uint16_t adcRetour;

    while(true) {
        adcRetour = adc.lecture(1);
        adcRetour = adcRetour >> 2; //On garde 8 bits
        uint8_t adcRetour8b = adcRetour;
        if(adcRetour8b < 200) {
            vert();
        }
        else if(adcRetour8b >= 200 && adcRetour8b < 250){
            ambree();
        }
        else {
            rouge();
        }
    }
}