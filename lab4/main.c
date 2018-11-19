#ifndef _SEGMENT_DISPLAY_H

#define SEG_A AT91C_PIO_PB25
#define SEG_B AT91C_PIO_PB24
#define SEG_C AT91C_PIO_PB22
#define SEG_D AT91C_PIO_PB21
#define SEG_E AT91C_PIO_PB20
#define SEG_F AT91C_PIO_PB27
#define SEG_G AT91C_PIO_PB26
#define SEG_DP AT91C_PIO_PB23

#define BRIGHTNESS AT91C_PIO_PB29
#define LEFT_DISPLAY AT91C_PIO_PB28
#define RIGHT_DISPLAY AT91C_PIO_PB30

#define TIMER_ENABLE AT91C_PITC_PITEN
#define TIMER_ITERRUPT_ENABLE AT91C_PITC_PITIEN

#define RIGHT 1
#define LEFT 0

#define DELAY 1
#define PIV 6250
#define SEGMENT_TRESHOLD 100
#define FRAME_TRESHOLD 1000

#define _SEGMENT_DISPLAY_H
/*
* Include your AT91 Library files and specific
* compiler definitions
*/
#include "/opt/arm_user/include/AT91SAM9263-EK.h"
#include "/opt/arm_user/include/AT91SAM9263.h"
#endif // _PROJECT_H
unsigned int frameCounter = 0;

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

void display_init(){
  AT91C_BASE_PIOB->PIO_PER = BRIGHTNESS | LEFT_DISPLAY | RIGHT_DISPLAY | SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP;
  AT91C_BASE_PIOB->PIO_OER = BRIGHTNESS | LEFT_DISPLAY | RIGHT_DISPLAY | SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP;
  
  AT91C_BASE_PIOB->PIO_CODR = BRIGHTNESS;
  AT91C_BASE_PIOB->PIO_SODR = RIGHT_DISPLAY | LEFT_DISPLAY;
  AT91C_BASE_PIOB->PIO_CODR = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP;
}
void displayOff(){
  AT91C_BASE_PIOB->PIO_SODR = RIGHT_DISPLAY | LEFT_DISPLAY;
  AT91C_BASE_PIOB->PIO_CODR = SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G | SEG_DP;
}
void PIT_init(){
  AT91C_BASE_PITC->PITC_PIMR &= (~AT91C_PITC_PITEN);
  AT91C_BASE_PITC->PITC_PIMR &= (~AT91C_PITC_PITIEN);
  AT91C_BASE_PITC->PITC_PIMR = PIV;
  AT91C_BASE_PITC->PITC_PIVR;
}
void delay_ms(unsigned int miliseconds){
  unsigned int counter;
  for(counter = miliseconds; counter>0; counter--){
    AT91C_BASE_PITC->PITC_PIMR |= AT91C_PITC_PITEN;
    while(0==(AT91C_BASE_PITC->PITC_PISR & (~AT91C_PITC_PITEN))){}
    AT91C_BASE_PITC->PITC_PIVR;
  }
}

void delay(){
  volatile unsigned int iterator;
  for(iterator = 0;iterator < DELAY; iterator++){}  
}

void display_segment(int digit,int side){
  displayOff();
  AT91C_BASE_PIOB->PIO_SODR = segment_encoded_digit[digit];
  
  if(side == RIGHT){
    AT91C_BASE_PIOB->PIO_CODR = RIGHT_DISPLAY;
  }
  else if (side == LEFT){
    AT91C_BASE_PIOB->PIO_CODR = LEFT_DISPLAY;
  }
  frameCounter++;
  delay_ms(DELAY);
}

void segment_display_number(int number){
  int leftDigit = number / 10;
  int rightDigit = number - leftDigit * 10;

  display_segment(rightDigit,RIGHT);
  
  display_segment(leftDigit,LEFT);
}

void dbgu_print_ascii(const char *a) {}

void main() {
  PIT_init();
  display_init();
  
  unsigned int number_counter = 0;
  
  while(1){
    segment_display_number(number_counter);
    if(frameCounter == FRAME_TRESHOLD){
      number_counter++;
      frameCounter = 0;
    }
    if(number_counter == SEGMENT_TRESHOLD) number_counter = 0;
  }
  while(1){}
}