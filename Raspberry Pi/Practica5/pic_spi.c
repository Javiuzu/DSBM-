#include <wiringPi.h>
#include <stdio.h>

#define CS_SPI_0 digitalWrite (29, LOW);
#define CS_SPI_1 digitalWrite (29, HIGH);
#define CLK_SPI digitalWrite (7, LOW); delay(1); digitalWrite (7, HIGH); delay(1);
#define DI_SPI_0 digitalWrite (13, LOW);
#define DO_SPI_1 digitalWrite (13, HIGH);


int main ()
{

  wiringPiSetupPhys ();
  pinMode (29, OUTPUT);	// CS
  pinMode (7, OUTPUT);	// CLK
  pinMode (13, OUTPUT);	// DI
  pinMode (11, INPUT);	// DO

  CS_SPI_1

   while (1) {
  
   printf("Selecciona opcio [Filtre = 0, Derivador = 1, Offset = 2]: ");
   int op;
   scanf("%d",&op);

  int f;
  for (f = 100; f > 0; --f) {

   CS_SPI_0
   delay(1);

   if ((op & 2) == 0) digitalWrite(13,LOW);
   else digitalWrite(13,HIGH);

   CLK_SPI

   if ((op & 1) == 0) digitalWrite(13,LOW);
   else digitalWrite(13,HIGH);

   CLK_SPI

   CS_SPI_1
   delay(1);



	  CS_SPI_0
	  delay(1);

	  	int i, j;
		short readVal[3] = {0,0,0};
		for (j = 0; j < 3; ++j) {
		  	for (i = 0; i < 16; ++i) {
				CLK_SPI    
				readVal[j] = readVal[j] << 1;
				if (digitalRead(11)) {
			  		readVal[j] += 1;
				}
		  	}
	    }
	  printf("X: %d, Y: %d, Z: %d\n", readVal[0], readVal[1], readVal[2]);
      CS_SPI_1
      delay(1);

	}

  }

  return 0;
}
