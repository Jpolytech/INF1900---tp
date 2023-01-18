/* 
+------+---+--------------+---+
| État | B | État suivant | S |
+------+---+--------------+---+
|  E0  | 0 |      E0      | 0 |
|      +---+--------------+---+
|      | 1 |      E1      | 0 |
+------+---+--------------+---+
|  E1  | 0 |      E1      | 0 |
|      +---+--------------+---+
|      | 1 |      E2      | 0 |
+------+---+--------------+---+
|  E2  | 0 |      E2      | 0 |
|      +---+--------------+---+
|      | 1 |      E3      | 0 |
+------+---+--------------+---+
|  E3  | 0 |      E0      | 1 |
|      +---+--------------+---+
|      | 1 |      E0      | 1 |
+------+---+--------------+---+
*/


#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20
enum State {E0, E1, E2, E3};
enum Button {INIT, PRESSING, RELEASE};

bool isButtonPressed(){
  if (PIND & (1<<PD2)){
          _delay_ms(DEBOUNCE_TIME);
          if(PIND & (1<<PD2)) {
              return true;
          }
      }
      return false;
}

bool isButtonReleased(){
    // if (PIND & 0X04) {
    auto button = Button::INIT;
    switch(button){
      case INIT:
        if (isButtonPressed()){
          button = Button::PRESSING;
        }

        return false;
        break;

      case PRESSING:
        if(!isButtonPressed()){
        button = Button::RELEASE;
        }

        return false;
        break;

      case RELEASE:
        return true;
        button = Button::INIT;

        break;
      }
}

int main() {
  uint8_t ETEINT = 0x00;
  uint8_t VERT = 0X01;
  //uint8_t ROUGE = 0X02;
  DDRA = DDR_OUT;
  DDRD = DDR_IN;

  auto state = State::E0;
  while(true){
    switch(state){
      case E0:
        PORTA = ETEINT;
        if(isButtonReleased()){
        state = State::E1;
        }
        break;

      case E1:
        PORTA = ETEINT;
        if(isButtonReleased()){
          state = State::E2;
        }
        break;

      case E2:
        PORTA = ETEINT;
        if(isButtonReleased()){
          state = State::E3;
        }
        break;

      case E3:
        PORTA = VERT;
        _delay_ms(2000);
        PORTA = ETEINT;
        state = State::E0;

        break;
      }
  }
}
