#define PIOB_PER (volatile unsigned int*) 0xFFFFF400
#define PIOB_OER (volatile unsigned int*) 0xFFFFF410
#define PIOB_CODR (volatile unsigned int*) 0xFFFFF434
#define PIOB_SODR (volatile unsigned int*) 0xFFFFF430

#define PIOC_PER (volatile unsigned int*) 0xFFFFF600
#define PIOC_ODR (volatile unsigned int*) 0xFFFFF614
#define PIOC_OER (volatile unsigned int*) 0xFFFFF610
#define PIOC_CODR (volatile unsigned int*) 0xFFFFF634
#define PIOC_SODR (volatile unsigned int*) 0xFFFFF630
#define PIOC_PDSR (volatile unsigned int*) 0xFFFFF63C
#define PIOC_PUER (volatile unsigned int*) 0xFFFFF664

#define PMC_PCER (volatile unsigned int*) 0xFFFFFC10

#define DELAY 200000
#define PERIOD 10
#define CLOCK_ENABLE 1UL<<4
#define USERLED1 1UL<<8
#define USERLED2 1UL<<29
#define RIGHTCLICK 1UL<<4
#define LEFTCLICK 1UL<<5

void delay(){
  volatile unsigned int iterator;
  for(iterator = 0;iterator < DELAY; iterator++){}  
}
void led_init(){
  *PIOB_PER = USERLED1;
  *PIOB_OER = USERLED1;
  
  *PIOC_PER = USERLED2;
  *PIOC_OER = USERLED2;
  
  *PIOB_SODR = USERLED1;
  *PIOC_SODR = USERLED2;
}

void button_init(){
  *PIOC_PER = RIGHTCLICK | LEFTCLICK;
  *PIOC_ODR = RIGHTCLICK | LEFTCLICK;
  *PIOC_PUER = RIGHTCLICK | LEFTCLICK;
  *PMC_PCER = CLOCK_ENABLE;
}

void dbgu_print_ascii(const char *a) {}

void main() {
  led_init();
  button_init();
  unsigned int counter = 0;
  while(1){
    if(((*PIOC_PDSR) & RIGHTCLICK) == 0){
      *PIOC_CODR = USERLED2;
    }
    if(((*PIOC_PDSR) & LEFTCLICK) == 0){
      *PIOC_SODR = USERLED2;
    }
    delay();
    counter++;
    if(counter == PERIOD){
      *PIOB_CODR = USERLED1;
    }
    if(counter == PERIOD*2){
      *PIOB_SODR = USERLED1;
      counter = 0;
    }
  }
  while(1){}
}
