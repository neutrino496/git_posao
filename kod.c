#include <stdio.h>
#include <stdlib.h>
#include <p30fxxxx.h>

#define BUFFER_SIZE 4 // DOROS = 5 - 1 = 4

#define kuca 6

_FOSC(CSW_FSCM_OFF & XT_PLL4);
_FWDT(WDT_OFF);

unsigned char tempRX;
unsigned char buffer[BUFFER_SIZE];
unsigned char target[BUFFER_SIZE] = "DOROS";
unsigned char n;
unsigned char flag;
unsigned int broj1,broj2;

void initUART1(void)
{
    U1BRG = 0x0040; // BAUDRATE = 9600

    U1MODEbits.ALTIO = 1; // koristimo alternativne pinove

    IEC0bits.U1RXIE = 1; // omogucavamo rx1 interupt

    U1STA &= 0xfffc; // inicijalno resetujemo bit za gresku i flag

    U1MODEbits.UARTEN = 1; // ukljucujemo ovaj modul

    U1STAbits.UTXEN = 1; // ukljucujemo predaju
	
	n = 0;
}

void __attribute__((__interrupt__)) _U1RXInterrupt(void) 
{
    IFS0bits.U1RXIF = 0;
    // tempRX = U1RXREG;
	
	buffer[n] = U1RXREG;
	if(n < BUFFER_SIZE)	n++;
	else n = 0;
} 

void WriteUART1(unsigned int data)
{
	while(!U1STAbits.TRMT);

    if(U1MODEbits.PDSEL == 3)
        U1TXREG = data;
    else
        U1TXREG = data & 0xFF;
}

void RS232_putst(register const char *str)
{
	while((*str) != 0)
	{
		WriteUART1(*str);
		if(*str == 13) WriteUART1(10);
		if(*str == 10) WriteUART1(13);
		str++;
	}
}


int main(int argc, char** argv) {
    
    
    initUART1();

	while(1)
	{
		flag = 1;
		
		for(broj1 = 0; broj1 < BUFFER_SIZE; broj1++)
		{
			if(buffer[broj1] != target[broj1]) 
			{
				flag = 0;
				break;
			}
		}
		
        
		if(flag) // primljeno je DOROS
		{
			RS232_putst("Jel te koleginice, sta kolega hoce od vas?");
			buffer[0] = 'X';	// menjam sadrzaj buffera da bi izbjegao ponovni ispis poruke iznad
		}


            for(broj1=0;broj1<3000;broj1++)
            for(broj2=0;broj2<1000;broj2++);

    }//od whilea

    return (EXIT_SUCCESS);
}
