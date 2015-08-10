#include <p18f2550.h>
#include <stdio.h>
#include <delays.h>
#include <spi.h>
#include <usart.h>
#include <adc.h>

/*********PIC18F4550's settings*********/
 /*#pragma config WDT = OFF
#pragma config FOSC = INTOSCIO_EC //Internal oscillator, port function on RA6, EC used by USB
//#pragma config FOSC = HSPLL_HS //Internal oscillator, port function on RA6, EC used by USB
#pragma config MCLRE = OFF
#pragma config LVP = OFF

*/
/*********Setting USART Properties**********/ 
void setup(void)
{
	/*****Port Set Up*****/
	ADCON1 = 0x0E;//VSS,VDD ref. AN0 analog only
	ADCON0 = 0x01;//clear ADCON0 to select channel 0 (AN0)
	ADCON2 = 0b00110000;//ADCON2 setup: Left justified, Tacq=2Tad, Tad=2*Tosc (or Fosc/2)
	ADCON0bits.ADON = 0x01;//Enable A/D module
	//TRISD = 0;
	LATA = 0;
	LATB = 0;
	LATC = 0;
	TRISA = 0b00000001;
	TRISB = 0b00000000; //set RB0 and RB1 as input for SPI communication
	TRISC = 0b10000000; // RX is an input, TX is output
 
	/********Interrupt Setu*******/
	INTCON = 0x00; /* Clear Registers */
	PIR1 = 0x00;
	PIE1 = 0x00;
	TMR1L = 0x00;
	TMR1H = 0x00;
	T1CON = 0x00;
	RCON = 0x00; 
 
    /* RS232 Enable */
	//RCSTA = 0b10000000;
	//TXSTA = 0b00100000;
	//BAUDCON = 0b00000000;		
	//SPBRG = 129; //FOSC = 8.000 MHz,SYNC = 0, BRGH = 0, BRG16 = 0

    /********* Baud Rate Settings*************/
	OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 51);
 
 
 
	/********Clock Setup************/ 
	OSCCON = 0b01110110; //select 8 MHz clock
}
 
void main()
{
	setup();	
	while (1)
	{
		ADCON0bits.GO_DONE = 1;                 //Start A/D Conversion
		while(ADCON0bits.GO_DONE != 0);         //Loop here until A/D conversion completes	
		while(BusyUSART());                     //Waits for USART to send current Data
		WriteUSART(ADRESH);                     //Writes Data to ADRESH Register(TX Port)
		Delay1KTCYx(30);                        //Delay for between 8 Bit data
	

   	//refresh(red_byte_array,green_byte_array,blue_byte_array);
	}
}