#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20

void avancer(){
    PORTB &= ~(1 << PORTB1);
    PORTB |= (1 << PORTB0);
}

void reculer(){
    PORTB |= (1 << PORTB1);
    PORTB |= (1 << PORTB0);
}

void eteint(){
    PORTB &= ~(1 << PORTB0);
    PORTB &= ~(1 << PORTB1);
}

void busyWaitDelay(uint16_t delay) {
  for(uint16_t i = 0; i < delay; ++i){
    _delay_us(1);
    //_delay_ms(1); //Au choix
    }
}

//t = temps*frequence (en s)
//pourcentage entre 0 et 1
//periode en us
void pwm(int t, double pourcentage, double periode) {
    //double periode = (1/frequence) *1000000; //periode en us
    double a = periode * pourcentage;
    for(int j=0; j < t; j++){
        avancer();
        busyWaitDelay(a);
        eteint();
        busyWaitDelay(periode - a);
    }
}

int main() {
    DDRB = DDR_OUT;

    //60hz
    pwm(120, 0, 16666);
    pwm(120, 0.25, 16666);
    pwm(120, 0.50, 16666);
    pwm(120, 0.75, 16666);
    pwm(120, 1, 16666);

    //400hz
    pwm(800, 0, 2500);
    pwm(800, 0.25, 2500);
    pwm(800, 0.50, 2500);
    pwm(800, 0.75, 2500);
    pwm(800, 1, 2500);

    eteint();
}