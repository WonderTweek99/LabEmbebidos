/*
 * MyPrint.c
 *
 * Created: 5/7/2021 8:13:09 PM
 * Author : jopc1
 */ 

#include "sam.h"
#include "myprintf.h"
#include "myprintf.c"
#include "stdbool.h"
#include "RTCControl.h"
#include "RTCControl.c"
#define SLAVE_ADDR 0x68u
#define BUF_SIZE 4

void UARTInit(void);

int main(void)
{
	UARTInit();
	I2Cinit();
	
	time_type Tiempo;
	
	Tiempo.seconds.reg = 0b00000000;
	Tiempo.minutes.reg = 0b00110011;
	Tiempo.hours.reg = 0b00100001;
	Tiempo.day.reg = 0b00000100;
	Tiempo.date.reg = 0b00000111;
	Tiempo.month.reg = 0b00010000;
	Tiempo.year.reg = 0b00100001;
		
	sendI2CDataArray(0x68, SECONDS_ADDR, &Tiempo.seconds.reg, 7);
	
	receiveI2CDataArray(0x68, SECONDS_ADDR, &Tiempo.seconds.reg, 7);
	
	char buff[31]; buff[30]= '\0';
	
	while(1){
		mysnprintf(buff, sizeof buff, "%x/%x/%x/%x/%x/%x", Tiempo.date.reg, Tiempo.month.reg, Tiempo.year.reg, Tiempo.hours.reg, Tiempo.minutes.reg, Tiempo.seconds.reg);
		myprintf("\n%s",buff);
		receiveI2CDataArray(0x68, SECONDS_ADDR, &Tiempo.seconds.reg, 7);
	}
	return 0;
}

void UARTInit(void) {
	    	/* Initialize the SAM system */
    SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;
	
	/* port mux configuration*/
	PORT->Group[0].DIR.reg |= (1 << 10);                  /* Pin 10 configured as output */
	PORT->Group[0].PINCFG[PIN_PA11].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	PORT->Group[0].PINCFG[PIN_PA10].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	
	/*PMUX: even = n/2, odd: (n-1)/2 */
	PORT->Group[0].PMUX[5].reg |= 0x02;                   /* Selecting peripheral function C */
	PORT->Group[0].PMUX[5].reg |= 0x20;                   /* Selecting peripheral function C */
	
	/* APBCMASK */
	//PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;			  /* SERCOM 0 enable*/
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;

	/*GCLK configuration for sercom0 module: using generic clock generator 0, ID for sercom0, enable GCLK*/

	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE) |
	GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

	
	/* configure SERCOM0 module for UART as Standard Frame, 8 Bit size, No parity, BAUDRATE:9600*/

	SERCOM0->USART.CTRLA.reg =
	SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
	SERCOM_USART_CTRLA_RXPO(3/*PAD3*/) | SERCOM_USART_CTRLA_TXPO(1/*PAD2*/);
	
	uint64_t br = (uint64_t)65536 * (8000000 - 16 * 9600) / 8000000;
	
	SERCOM0->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);

	SERCOM0->USART.BAUD.reg = (uint16_t)br;

	SERCOM0->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}