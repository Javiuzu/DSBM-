#include <wiringPi.h>
#include <stdio.h>

#define CS_SPI digitalWrite (15, LOW);
#define CLK_SPI digitalWrite (7, LOW); delay(1); digitalWrite (7, HIGH); delay(1);
#define DI_SPI_0 digitalWrite (13, LOW);
#define DO_SPI_1 digitalWrite (13, HIGH);

int main (void)
{
  wiringPiSetupPhys ();
  pinMode (15, OUTPUT);	// CS
  pinMode (7, OUTPUT);	// CLK
  pinMode (13, OUTPUT);	// DI
  pinMode (11, INPUT);	// DO

  //CS_SPI

while(1) {
  digitalWrite (15, LOW);
  DO_SPI_1	// Start
  CLK_SPI
  
  DO_SPI_1	// SCL/DIFF (Single Channel Diff)
  CLK_SPI
  
  DI_SPI_0 	// D2
  CLK_SPI

  DI_SPI_0 	// D1
  CLK_SPI

  DI_SPI_0 	// D0
  CLK_SPI

  CLK_SPI
  CLK_SPI

  
  	int readVal = 0, i;
  	for (i = 0; i < 12; ++i) {
		CLK_SPI    
		readVal = readVal << 1;
    	if (digitalRead(11)) {
	  		readVal += 1;
		}
  	}
  	printf("%d\n",readVal);
  	digitalWrite (15, HIGH);
    delay(20);	
    
  }

  //printf("%d\n",readVal);
  
  return 0 ;
}
