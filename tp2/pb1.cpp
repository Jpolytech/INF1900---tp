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

using namespace std;

#define DDR_IN 0x00;
#define DDR_OUT 0xff;

enum class State {
  E0, E1, E2, E3
};

bool isButtonPushed(){
  if (PIND & (1<<PD2)){
          _delay_ms(20);
          if(PIND & (1<<PD2)) {
            while(PIND & (01<<PD2))
              continue;
            return true;
          }
      }
      return false;
}

void vert(){
    PORTA |= (1 << PORTA0);
}

void eteint(){
  PORTA &= ~(1 << PORTA0);
}

int main() {
  DDRA = DDR_OUT;// PORT A est en mode sortie
  DDRD = DDR_IN; // PORT D est en mode entrée
  State state {State::E0};

  
  while(true){
    switch(state){
      case(State::E0):
        eteint();
        if(isButtonPushed()){
        state = State::E1;
        }
        break;

      case(State::E1):
        if(isButtonPushed()){
          state = State::E2;
        }
        break;

      case(State::E2):
        if(isButtonPushed()){
          state = State::E3;
        }
        break;

      case(State::E3):
        vert();
        _delay_ms(2000);
        state = State::E0;

        break;

        default:
        state = State::E0;
        break;
      }
  }
}
