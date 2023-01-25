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

//t en s
//pourcentage entre 0 et 1
//frequence en hz
void pwm(int t, double pourcentage, double frequence) {
    double periode = (1/frequence) *1000000; //periode en us
    double a = periode * pourcentage;
    t = t*frequence;
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
    pwm(2, 0, 60);
    pwm(2, 0.25, 60);
    pwm(2, 0.50, 60);
    pwm(2, 0.75, 60);
    pwm(2, 1, 60);

    //400hz
    pwm(2, 0, 400);
    pwm(2, 0.25, 400);
    pwm(2, 0.50, 400);
    pwm(2, 0.75, 400);
    pwm(2, 1, 400);
    
    eteint();
}