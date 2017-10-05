#include <wiringPi.h>
#include <stdio.h>

#define CS_SPI_0 digitalWrite (29, LOW);
#define CS_SPI_1 digitalWrite (29, HIGH);
#define CLK_SPI digitalWrite (7, LOW); delay(1); digitalWrite (7, HIGH); delay(1);
#define DI_SPI_0 digitalWrite (13, LOW);
#define DI_SPI_1 digitalWrite (13, HIGH);
#define DO_SPI_1 digitalWrite (13, HIGH);

#define CoordXNeg_0 digitalWrite (38, LOW);
#define CoordXPos_1 digitalWrite (36, HIGH);
#define CoordYNeg_0 digitalWrite (37, LOW);
#define CoordYPos_1 digitalWrite (35, HIGH);

void channel2 () {

	  DO_SPI_1	// SCL/DIFF (Single Channel Diff)
	  CLK_SPI
	  
	  DI_SPI_0 	// D2
	  CLK_SPI

	  DI_SPI_1 	// D1
	  CLK_SPI

	  DI_SPI_0 	// D0
	  CLK_SPI
}

void channel3 () {

	  DO_SPI_1	// SCL/DIFF (Single Channel Diff)
	  CLK_SPI
	  
	  DI_SPI_0 	// D2
	  CLK_SPI

	  DI_SPI_1 	// D1
	  CLK_SPI

	  DI_SPI_1 	// D0
	  CLK_SPI
}


int main ()
{

  wiringPiSetupPhys ();
  pinMode (29, OUTPUT);	// CS_PIC
  pinMode (7, OUTPUT);	// CLK
  pinMode (13, OUTPUT);	// DI
  pinMode (11, INPUT);	// DO
  pinMode (15, OUTPUT);	// CS_AD
  pinMode (16, OUTPUT); // CS_Pantalla
  pinMode (22, OUTPUT); // Reset_Pantalla
  pinMode (18, OUTPUT); // CLK_Pantalla

  CS_SPI_1

   //while (1) {
  
   int op = 0;

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

	  int tactil[2];
      int it;
	  for(it = 0; it < 2; ++it) {

		  if (it == 0) { // Read CoordX
			  pinMode (37, INPUT); 	// Y-
			  pinMode (35, INPUT);	// Y+
			  pinMode (36, OUTPUT); // X+
			  pinMode (38, OUTPUT); // X-
		      CoordXNeg_0
              CoordXPos_1
		  }
		  else {
			  pinMode (37, OUTPUT); // Y-
			  pinMode (35, OUTPUT);	// Y+
			  pinMode (36, INPUT);  // X+
			  pinMode (38, INPUT);  // X-
			  CoordYNeg_0
			  CoordYPos_1
		  }

		  digitalWrite (15, LOW); //CS

	  	  DO_SPI_1	// Start
	  	  CLK_SPI

		  if (it == 0) channel3();
		  else channel2();

		  CLK_SPI
		  CLK_SPI
		  
		  	int readV = 0, i;
		  	for (i = 0; i < 12; ++i) {
				CLK_SPI    
				readV = readV << 1;
				if (digitalRead(11)) {
			  		readV += 1;
				}
		  	}
			tactil[it] = readV;
		  	digitalWrite (15, HIGH); //CS
			delay(20);	
	  	}
		if (tactil[0] == 0 || tactil[1] == 0) {
			tactil[0] = 0;
			tactil[1] = 0;
		}
	  	printf("CoordX: %d, CoordY: %d\n",tactil[0], tactil[1]);

	//}

  return 0;
}
