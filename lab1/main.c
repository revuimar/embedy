#define PIOB_OER (volatile unsigned int*) 0xFFFFF400
#define PIOB_CODR (volatile unsigned int*) 0xFFFFF434
#define PIOB_SODR (volatile unsigned int*) 0xFFFFF430
#define PIOB_PER (volatile unsigned int*) 0xFFFFF410
#define PIOB_PDSR (volatile unsigned int*) 0xFFFFF43C

#define PIOC_OER (volatile unsigned int*) 0xFFFFF600
#define PIOC_CODR (volatile unsigned int*) 0xFFFFF634
#define PIOC_SODR (volatile unsigned int*) 0xFFFFF630
#define PIOC_PER (volatile unsigned int*) 0xFFFFF610
#define PIOC_PDSR (volatile unsigned int*) 0xFFFFF63C

#define DELAY (volatile unsigned int) 2000000
#define USERLED1 (volatile unsigned int) 1<<8
#define USERLED2 (volatile unsigned int) 1<<29

void delay(){
  volatile unsigned int iterator;
  for(iterator = 0;iterator < DELAY; iterator++){}  
}

void dbgu_print_ascii(const char *a) {}

void main() {
  *PIOB_OER = USERLED1;
  *PIOB_PER = USERLED1;
 
  *PIOC_OER = USERLED2;
  *PIOC_PER = USERLED2;
  
  while(1){
  *PIOB_CODR = USERLED1;
  *PIOC_CODR = USERLED2;
  delay();
  *PIOB_SODR = USERLED1;
  *PIOC_SODR = USERLED2;
  delay();
  }
  while(1){}
}
