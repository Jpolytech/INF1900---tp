#define F_CPU 8000000UL
#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

using namespace std;

#define DDR_IN 0x00;
#define DDR_OUT 0xff;
#define DEBOUNCE_TIME 20;

volatile uint8_t gEtat = 0;

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

ISR(INT0_vect) {
    _delay_ms(10);
    if(PIND & (1<<PD2)) {
        gEtat++;
    }
    EIFR |= (1<<INTF0);
}

void initialisation ( void ) {
    cli();
    DDRA = DDR_OUT;// PORT A est en mode sortie
    DDRD = DDR_IN; // PORT D est en mode entrée


    // cette procédure ajuste le registre EIMSK
    // de l’ATmega324PA pour permettre les interruptions externes
    EIMSK |= (1 << INT0) ;


    // il faut sensibiliser les interruptions externes aux
    // changements de niveau du bouton-poussoir
    // en ajustant le registre EICRA
    EICRA |= (1 << ISC00) | (1 << ISC01);

    // sei permet de recevoir à nouveau des interruptions.
    sei();

}

int main()
{
    initialisation();
    while(true){
         switch(gEtat){
            case 0:
                vert();

            break;

            case 1:
                while(PIND & (1<<PD2)){
                    vert();
                    _delay_ms(10);
                    rouge();
                    _delay_ms(10);
                }
                vert();

            break;

            case 2:
                rouge();
            break;

            case 3:
                eteint();
            break;

            case 4:
                vert();
                gEtat = 0;
            break;
                
        }
    }
}