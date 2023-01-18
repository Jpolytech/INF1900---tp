/*
+--------+---+--------------+----+
|  État  | B | État suivant |  S |
+--------+---+--------------+----+
| R_INIT | 0 |    R_INIT    | 00 |
|        +---+--------------+----+
|        | 1 |       A      | 01 |
+--------+---+--------------+----+
|    A   | 0 |       A      | 01 |
|        +---+--------------+----+
|        | 1 |      V1      | 10 |
+--------+---+--------------+----+
|   V1   | 0 |      V1      | 10 |
|        +---+--------------+----+
|        | 1 |       R      | 00 |
+--------+---+--------------+----+
|    R   | 0 |       R      | 00 |
|        +---+--------------+----+
|        | 1 |    CLOSED    | 11 |
+--------+---+--------------+----+
| CLOSED | 0 |    CLOSED    | 11 |
|        +---+--------------+----+
|        | 1 |      V2      | 10 |
+--------+---+--------------+----+
|   V2   | 0 |      V2      | 10 |
|        +---+--------------+----+
|        | 1 |    R_INIT    | 00 |
+--------+---+--------------+----+
*/
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define DDR_IN 0x00
#define DDR_OUT 0xff
#define DEBOUNCE_TIME 20
enum class Color {
    R_INIT, A, V1, R, CLOSED, V2
    };

bool isButtonPushed(){
  if (PIND & (1<<PD2)){
          _delay_ms(DEBOUNCE_TIME);
          if(PIND & (1<<PD2)) {
              return true;
          }
      }
      return false;
}

void ROUGE(){
    PORTA &= ~(1 << PORTA0);
    PORTA |= (1 << PORTA1);
}

void VERT(){
    PORTA |= (1 << PORTA0);
    PORTA &= ~(1 << PORTA1);
}

void ETEINT(){
    PORTA &= ~(1 << PORTA0);
    PORTA &= ~(1 << PORTA1);
}



int main(){
    DDRA = DDR_OUT;
    DDRD = DDR_IN;

    auto color = Color::R_INIT;
    while(true){
        switch(color){
            case(Color::R_INIT):
                ROUGE();
                if(isButtonPushed()){
                    color = Color::A;
                }

            break;

            case(Color::A):
                while(isButtonPushed()){
                    VERT();
                    _delay_ms(10);
                    ROUGE();
                    _delay_ms(10);
                }

                color = Color::V1;

            break;

            case(Color::V1):
                VERT();
                if(isButtonPushed()){
                    color = Color::R;
                }

            break;

            case(Color::R):
                ROUGE();
                if(!isButtonPushed()){
                    color = Color::CLOSED;
                }

            break;

            case(Color::CLOSED):
                ETEINT();
                if(isButtonPushed()){
                    color = Color::V2;
                }

            break;

            case(Color::V2):
                VERT();
                if(!isButtonPushed()){
                    color = Color::R_INIT;
                }

            break;
                
        }
    }
}