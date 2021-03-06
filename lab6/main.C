#ifndef _PROJECT_H
#define _PROJECT_H
#include "/opt/arm_user/include/AT91SAM9263-EK.h"
#include "/opt/arm_user/include/AT91SAM9263.h"
#endif // _PROJECT_H


#define PIOB_OER  	(volatile unsigned int*) 0xFFFFF410
#define PIOB_SODR 	(volatile unsigned int*) 0xFFFFF430
#define PIOB_CODR 	(volatile unsigned int*) 0xFFFFF434
#define PIOB_PDSR 	(volatile unsigned int*) 0xFFFFF43C
#define PIOB_PER  	(volatile unsigned int*) 0xFFFFF400

#define PIOC_OER 	(volatile unsigned int*) 0xFFFFF610
#define PIOC_SODR 	(volatile unsigned int*) 0xFFFFF630
#define PIOC_CODR 	(volatile unsigned int*) 0xFFFFF634
#define PIOC_PDSR	(volatile unsigned int*) 0xFFFFF63C
#define PIOC_PER  	(volatile unsigned int*) 0xFFFFF600

#define USERLED1	1<<8
#define USERLED2	1<<29
#define DELAY		2000000
#define BAUD_RATE 54UL
#define BUFFERSIZE 0xF

typedef struct FIFO{
    char buffer[BUFFERSIZE+1];
    unsigned int head;
    unsigned int tail; 
}FIFO;
 
void FIFO_init(FIFO *fifo){
    fifo->head = 0;
    fifo->tail = 0;
}
int FIFO_push(FIFO *fifo,char data){
    if((fifo->tail-fifo->head)==1 || (fifo->head - fifo->tail) == BUFFERSIZE){return -1;}
    fifo->buffer[fifo->head] = data;
    fifo->head = (fifo->head + 1) & BUFFERSIZE;
    return 1;
}
int FIFO_pop(FIFO* fifo,char* data){
    if(fifo->head != fifo->tail){
        *data = fifo->buffer[fifo->tail];
        fifo->tail = (fifo->tail + 1) & BUFFERSIZE;
        return 1;
    }
    return 0;
}
 
void dbgu_print_ascii(const char *a) {}
 
 
 
void delay() {
    volatile unsigned int i;
    for(i=0; i<DELAY; i++) {}
}
 
 
int DGBU_init() {
    AT91C_BASE_DBGU->DBGU_IDR = (~(0UL));//deactivate all interrupts
    AT91C_BASE_DBGU->DBGU_CR |= AT91C_US_RSTRX | AT91C_US_RSTTX;//reset&disable transmitter
    AT91C_BASE_DBGU->DBGU_BRGR = BAUD_RATE;//configure baudrate
    AT91C_BASE_PIOC->PIO_ASR = AT91C_PC30_DRXD | AT91C_PC31_DTXD;//Configure RxD and TxD DBGU as input peripheral ports
    AT91C_BASE_PIOC->PIO_PDR = AT91C_PC30_DRXD | AT91C_PC31_DTXD;
    AT91C_BASE_DBGU->DBGU_MR |= AT91C_US_CHMODE_NORMAL | AT91C_US_PAR_NONE;//configure operation mode
    AT91C_BASE_DBGU->DBGU_IER |= AT91C_US_RXRDY | AT91C_US_TXRDY;//enable interrupts for transmitter and receiver being ready
    AT91C_BASE_DBGU->DBGU_CR |= AT91C_US_RXEN | AT91C_US_TXEN;//enable receiver and transmitter
    return 0;
}
 
int DBGU_send_data(const char* buffer) {
    volatile unsigned int i;
    for(i = 0; buffer[i] !='\0'; i++) {
        while(!(AT91C_US_TXRDY == (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY))) { };
        AT91C_BASE_DBGU->DBGU_THR = buffer[i];
    }
    return 0;
}
int DBGU_send_char(char buffer) {
    while(!(AT91C_US_TXRDY == (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_TXRDY))) { };
    AT91C_BASE_DBGU->DBGU_THR = buffer;
    return 0;
}
char DBGU_get_char(){
    char letter;
    while(!(AT91C_US_RXRDY == (AT91C_BASE_DBGU->DBGU_CSR & AT91C_US_RXRDY))){};
    letter = AT91C_BASE_DBGU->DBGU_RHR;
    return letter;
}
void printABC(){
	int lowerCase = 0;
	int i = 0;
	for(i = 0;i<('Z'-'A'+1);i++){
		DBGU_send_char((lowerCase)?('a'+i):('A'+i));
		if('A'+i == 'Z'){
			lowerCase++;
			i=0;
		}
	}
}
void FIFO_dump(FIFO *fifo){
    char character;
    while(fifo->head != fifo->tail){
        FIFO_pop(fifo,&character);
		DBGU_send_char(character);
    }
}
void main() {
 
    DGBU_init();
 
    DBGU_send_data("test\n\r");
    DBGU_send_char('T');
    printABC();
    FIFO *queue;
    FIFO_init(queue);
 
    *PIOB_OER = USERLED1;
    *PIOB_PER = USERLED1;
 
    *PIOC_OER = USERLED2;
    *PIOC_PER = USERLED2;
    *PIOC_CODR = USERLED2;
	DBGU_send_data("\n\rInput: ");
	
    while(1){
        char letter = DBGU_get_char();
        if(letter == 13){
            DBGU_send_data("enter pressed\n\r");
            FIFO_dump(queue);
            FIFO_init(queue);
            DBGU_send_data("\n\rInput: ");
        }
        if(letter >= 'A' && letter <= 'Z'){
            if(FIFO_push(queue,'a'-'A'+letter)==-1){
                DBGU_send_data("overflow\n\r");
                FIFO_dump(queue);
                FIFO_init(queue);
                DBGU_send_data("\n\rInput: ");
            }
        }
        if(letter >= 'a' && letter <= 'z'){
            if (FIFO_push(queue,letter-('a'-'A'))==-1){
                DBGU_send_data("overflow\n\r");
                FIFO_dump(queue);
                FIFO_init(queue);
                DBGU_send_data("\n\rInput: ");
            }
        }
    }
     
    while(1) {}
}

