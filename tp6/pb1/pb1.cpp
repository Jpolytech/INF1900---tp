//TP6-probleme 1
//code écrit par Thibaut Delahaie (2157153) et Jean-Philippe Salis Routhier (2201275)
//Pour le cours de INF1900-Projet integrateur 1: Systemes embarqués
//à la session d'hiver 2023

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff

volatile uint8_t buttonIsPushed = false;
volatile uint8_t timerIsRunning = false;

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

void vertClignote() {
    vert();
    _delay_ms(50);
    eteint();
    _delay_ms(50);
}

void initialisation ( void ) {
    cli();
    DDRA = DDR_OUT;// PORT A est en mode sortie
    DDRD = DDR_IN; // PORT D est en mode entrée

    // ajustement du registre EIMSK
    EIMSK |= (1 << INT0) ;

    // Ajustement de EICRA
    EICRA |= (1 << ISC00); //Chaque changement d'etat du bouton va generer un signal

    // sei permet de recevoir à nouveau des interruptions.
    sei();

}

void partirMinuterie ( uint16_t duree ) {
    timerIsRunning = true;
    // mode CTC du timer 1 avec horloge divisée par 1024
    // interruption après la durée spécifiée
    //mode CTC, TCNT1 et compare match
    TCNT1 = 0;
    OCR1A = duree;
    TCCR1A = (1 << COM1A0) ;
    TCCR1B = (1 << CS12) | (1 << CS10) ;
    TCCR1C = 0;
    TIMSK1 = (1 << OCIE1A) ;
}

//ISR pour le timer
ISR(TIMER1_COMPA_vect) {
    timerIsRunning = false;
}

//ISR pour le bouton
ISR(INT0_vect) {
    _delay_ms(10);
    if(PIND & (1<<PD2)) {
        buttonIsPushed = true;
    } else{
        buttonIsPushed = false;
    }
    //buttonIsPushed = false;
    EIFR |= (1<<INTF0);
}

int main() {
    uint8_t i = 0;
    initialisation();
    while(true) {
        while(buttonIsPushed && i<120) {
            if(!timerIsRunning) {
                partirMinuterie(781); //1 seconde de minuterie
                i+=1;
            }
        }
        if(i>0){
            partirMinuterie(391); // 1/2 seconde de minuterie
            while(timerIsRunning){
                vertClignote();
            }
            eteint();
            partirMinuterie(1562); //2secondes de minuterie
            while(timerIsRunning) {
                _delay_ms(20);
            }
            uint8_t j = 0;
            while(j < i/2) {
                //lumiere rouge clignotante au rythme de deux fois par seconde
                j++;
                rouge();
                _delay_ms(250);
                eteint();
                _delay_ms(250);
            }
            vert();
            _delay_ms(1000);
            eteint();
            i = 0;
        }
    }
}