#define PIOB_PER (volatile unsigned int*) 0xFFFFF400
#define PIOB_OER (volatile unsigned int*) 0xFFFFF410
#define PIOB_CODR (volatile unsigned int*) 0xFFFFF434
#define PIOB_SODR (volatile unsigned int*) 0xFFFFF430

#define PIOC_PER (volatile unsigned int*) 0xFFFFF600
#define PIOC_ODR (volatile unsigned int*) 0xFFFFF614
#define PIOC_OER (volatile unsigned int*) 0xFFFFF610
#define PIOC_CODR (volatile unsigned int*) 0xFFFFF634
#define PIOC_SODR (volatile unsigned int*) 0xFFFFF630

#define SEG_A 1UL<<25
#define SEG_B 1UL<<24
#define SEG_C 1UL<<22
#define SEG_D 1UL<<21
#define SEG_E 1UL<<20
#define SEG_F 1UL<<27
#define SEG_G 1UL<<26
#define SEG_DP 1UL<<23

#define BRIGHTNESS 1UL<<29
#define LEFT_DISPLAY 1UL<<28
#define RIGHT_DISPLAY 1UL<<30

#define ZERO 0
#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define NINE 8

#define ON 1
#define OFF 0

#define RIGHT 1
#define LEFT 0

#define DELAY 10000
#define PERIOD 40
#define SEGMENT_TRESHOLD 100

unsigned int segment_encoded_digit[10] = {
  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F),
  (SEG_B | SEG_C),
  (SEG_A | SEG_B | SEG_D | SEG_E | SEG_G),
  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_G),
  (SEG_B | SEG_C | SEG_F | SEG_G),
  (SEG_A | SEG_C | SEG_D | SEG_F | SEG_G),
  (SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
  (SEG_A | SEG_B | SEG_C ),
  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G),
  (SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G)
};

void delay(){
  volatile unsigned int iterator;
  for(iterator = 0;iterator < DELAY; iterator++){}  
}

void display_init(){
  *PIOB_PER = BRIGHTNESS | LEFT_DISPLAY | RIGHT_DISPLAY | SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP;
  *PIOB_OER = BRIGHTNESS | LEFT_DISPLAY | RIGHT_DISPLAY | SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP;
  
  *PIOB_CODR = BRIGHTNESS;
  *PIOB_SODR = RIGHT_DISPLAY | LEFT_DISPLAY;
  *PIOB_CODR = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP;
}

void display_segment(int digit,int side){
  displayOff();
  *PIOB_SODR = segment_encoded_digit[digit];
  
  if(side == RIGHT){
    *PIOB_CODR = RIGHT_DISPLAY;
  }
  else if (side == LEFT){
    *PIOB_CODR = LEFT_DISPLAY;
  }
  
  delay();
}
void displayOff(){
  *PIOB_SODR = RIGHT_DISPLAY | LEFT_DISPLAY;
  *PIOB_CODR = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP;
}
void segment_display_number(int number){
  int leftDigit = number / 10;
  int rightDigit = number - leftDigit * 10;

  display_segment(rightDigit,RIGHT);
  display_segment(leftDigit,LEFT);
}

void dbgu_print_ascii(const char *a) {}

void main() {
  display_init();
  unsigned int number_counter = 0;
  unsigned int loop_counter = 0;
  while(1){
    segment_display_number(number_counter);
    loop_counter++;
    if(loop_counter == PERIOD){
      number_counter++;
      loop_counter = 0;
    }
    if(number_counter == SEGMENT_TRESHOLD) number_counter = 0;
    //*PIOC_CODR = USERLED2;
  }
  while(1){}
}