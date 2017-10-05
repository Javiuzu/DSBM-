#include <wiringPi.h>
#include <stdio.h>
#include <string.h>

#define CS_SPI digitalWrite (15, LOW);
#define CLK_SPI digitalWrite (7, LOW); delay(1); digitalWrite (7, HIGH); delay(1);
#define DI_SPI_0 digitalWrite (13, LOW);
#define DI_SPI_1 digitalWrite (13, HIGH);
#define DO_SPI_1 digitalWrite (13, HIGH);

#define CS_Pantalla_0 digitalWrite (16, LOW);
#define CS_Pantalla_1 digitalWrite (16, HIGH);
#define Reset_Pantalla_0 digitalWrite (22, LOW);
#define Reset_Pantalla_1 digitalWrite (22, HIGH);
#define CLK_SPI_Pantalla digitalWrite (18, LOW); digitalWrite (18, HIGH);

#define CoordXNeg_0 digitalWrite (38, LOW);
#define CoordXPos_1 digitalWrite (36, HIGH);
#define CoordYNeg_0 digitalWrite (37, LOW);
#define CoordYPos_1 digitalWrite (35, HIGH);

// Taula per la generacio dels caracters utilitzant un font 5x7 pixels
// A la taula estan els caracters ASCII del 32 al 125
// A les posicions posteriors hi ha simbol no estandard ASCII
 
unsigned const char font5x7[500] = {
0x00, 0x00, 0x00, 0x00, 0x00,// (space) ASCII 32
0x00, 0x00, 0x5F, 0x00, 0x00,// !
0x00, 0x07, 0x00, 0x07, 0x00,// "
0x14, 0x7F, 0x14, 0x7F, 0x14,// #
0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
0x23, 0x13, 0x08, 0x64, 0x62,// %
0x36, 0x49, 0x55, 0x22, 0x50,// &
0x00, 0x05, 0x03, 0x00, 0x00,// '
0x00, 0x1C, 0x22, 0x41, 0x00,// (
0x00, 0x41, 0x22, 0x1C, 0x00,// )
0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
0x08, 0x08, 0x3E, 0x08, 0x08,// +
0x00, 0x50, 0x30, 0x00, 0x00,// ,
0x08, 0x08, 0x08, 0x08, 0x08,// -
0x00, 0x30, 0x30, 0x00, 0x00,// .
0x20, 0x10, 0x08, 0x04, 0x02,// /
0x3E, 0x51, 0x49, 0x45, 0x3E,// 0 ASCII 48
0x00, 0x42, 0x7F, 0x40, 0x00,// 1
0x42, 0x61, 0x51, 0x49, 0x46,// 2
0x21, 0x41, 0x45, 0x4B, 0x31,// 3
0x18, 0x14, 0x12, 0x7F, 0x10,// 4
0x27, 0x45, 0x45, 0x45, 0x39,// 5
0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
0x01, 0x71, 0x09, 0x05, 0x03,// 7
0x36, 0x49, 0x49, 0x49, 0x36,// 8
0x06, 0x49, 0x49, 0x29, 0x1E,// 9
0x00, 0x36, 0x36, 0x00, 0x00,// :
0x00, 0x56, 0x36, 0x00, 0x00,// ;
0x00, 0x08, 0x14, 0x22, 0x41,// <
0x14, 0x14, 0x14, 0x14, 0x14,// =
0x41, 0x22, 0x14, 0x08, 0x00,// >
0x02, 0x01, 0x51, 0x09, 0x06,// ?
0x32, 0x49, 0x79, 0x41, 0x3E,// @
0x7E, 0x11, 0x11, 0x11, 0x7E,// A ASCII 65
0x7F, 0x49, 0x49, 0x49, 0x36,// B
0x3E, 0x41, 0x41, 0x41, 0x22,// C
0x7F, 0x41, 0x41, 0x22, 0x1C,// D
0x7F, 0x49, 0x49, 0x49, 0x41,// E
0x7F, 0x09, 0x09, 0x01, 0x01,// F
0x3E, 0x41, 0x41, 0x51, 0x32,// G
0x7F, 0x08, 0x08, 0x08, 0x7F,// H
0x00, 0x41, 0x7F, 0x41, 0x00,// I
0x20, 0x40, 0x41, 0x3F, 0x01,// J
0x7F, 0x08, 0x14, 0x22, 0x41,// K
0x7F, 0x40, 0x40, 0x40, 0x40,// L
0x7F, 0x02, 0x04, 0x02, 0x7F,// M
0x7F, 0x04, 0x08, 0x10, 0x7F,// N
0x3E, 0x41, 0x41, 0x41, 0x3E,// O
0x7F, 0x09, 0x09, 0x09, 0x06,// P
0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
0x7F, 0x09, 0x19, 0x29, 0x46,// R
0x46, 0x49, 0x49, 0x49, 0x31,// S 0  ASCII 73
0x01, 0x01, 0x7F, 0x01, 0x01,// T 5
0x3F, 0x40, 0x40, 0x40, 0x3F,// U 10
0x1F, 0x20, 0x40, 0x20, 0x1F,// V
0x7F, 0x20, 0x18, 0x20, 0x7F,// W 20
0x63, 0x14, 0x08, 0x14, 0x63,// X
0x03, 0x04, 0x78, 0x04, 0x03,// Y 30
0x61, 0x51, 0x49, 0x45, 0x43,// Z
0x00, 0x00, 0x7F, 0x41, 0x41,// [
0x02, 0x04, 0x08, 0x10, 0x20,// "\"
0x41, 0x41, 0x7F, 0x00, 0x00,// ] 50
0x04, 0x02, 0x01, 0x02, 0x04,// ^
0x40, 0x40, 0x40, 0x40, 0x40,// _
0x00, 0x01, 0x02, 0x04, 0x00,// `
0x20, 0x54, 0x54, 0x54, 0x78,// a 70  ASCII 97
0x7F, 0x48, 0x44, 0x44, 0x38,// b
0x38, 0x44, 0x44, 0x44, 0x20,// c
0x38, 0x44, 0x44, 0x48, 0x7F,// d
0x38, 0x54, 0x54, 0x54, 0x18,// e 90
0x08, 0x7E, 0x09, 0x01, 0x02,// f 
0x08, 0x14, 0x54, 0x54, 0x3C,// g 100
0x7F, 0x08, 0x04, 0x04, 0x78,// h
0x00, 0x44, 0x7D, 0x40, 0x00,// i
0x20, 0x40, 0x44, 0x3D, 0x00,// j
0x00, 0x7F, 0x10, 0x28, 0x44,// k 120
0x00, 0x41, 0x7F, 0x40, 0x00,// l
0x7C, 0x04, 0x18, 0x04, 0x78,// m
0x7C, 0x08, 0x04, 0x04, 0x78,// n
0x38, 0x44, 0x44, 0x44, 0x38,// o 140
0x7C, 0x14, 0x14, 0x14, 0x08,// p 
0x08, 0x14, 0x14, 0x18, 0x7C,// q
0x7C, 0x08, 0x04, 0x04, 0x08,// r
0x48, 0x54, 0x54, 0x54, 0x20,// s 160
0x04, 0x3F, 0x44, 0x40, 0x20,// t
0x3C, 0x40, 0x40, 0x20, 0x7C,// u
0x1C, 0x20, 0x40, 0x20, 0x1C,// v
0x3C, 0x40, 0x30, 0x40, 0x3C,// w 180
0x44, 0x28, 0x10, 0x28, 0x44,// x
0x0C, 0x50, 0x50, 0x50, 0x3C,// y
0x44, 0x64, 0x54, 0x4C, 0x44,// z
0x00, 0x08, 0x36, 0x41, 0x00,// { 200 ASCII 123
0x00, 0x00, 0x7F, 0x00, 0x00,// |
0x00, 0x41, 0x36, 0x08, 0x00,// }
0x08, 0x08, 0x2A, 0x1C, 0x08,// ->      NO ASCII 
0x08, 0x1C, 0x2A, 0x08, 0x08,// <- 220  NO ASCII
0x08, 0x00, 0xAA, 0x00, 0x08, // Creu   NO ASCII
0X00, 0x38, 0x38, 0x38, 0x00 // punt 3x3 pixels  NO ASCII 129
};


void channel0() {
	  
	  DO_SPI_1	// SCL/DIFF (Single Channel Diff)
	  CLK_SPI
	  
	  DI_SPI_0 	// D2
	  CLK_SPI

	  DI_SPI_0 	// D1
	  CLK_SPI

	  DI_SPI_0 	// D0
	  CLK_SPI
}

void channel1() {
	  
	  DO_SPI_1	// SCL/DIFF (Single Channel Diff)
	  CLK_SPI
	  
	  DI_SPI_0 	// D2
	  CLK_SPI

	  DI_SPI_0 	// D1
	  CLK_SPI

	  DI_SPI_1 	// D0
	  CLK_SPI
}

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

void Send_SPI_8 (char val) {
	int i;
	for (i = 0; i < 8; ++i) {
		if(val &128) DI_SPI_1
		else DI_SPI_0
		CLK_SPI_Pantalla
		val = val << 1;
	}
}

void Write_SPI_TFT_Cmd(char reg) {
	CS_Pantalla_0
	Send_SPI_8(0x70);
	Send_SPI_8(reg);
	CS_Pantalla_1
}

void Write_SPI_TFT_Dat(short val) {
	CS_Pantalla_0
	Send_SPI_8(0x72);
	Send_SPI_8((char)(val >> 8));
	Send_SPI_8((char)val);
	CS_Pantalla_1
}

void Write_SPI_TFT_Reg(char reg, short val) {
	Write_SPI_TFT_Cmd(reg);
	Write_SPI_TFT_Dat(val);
}


void SPI_TFT_pixel (short x, short y, short color) {
    //printf("Pixel %d %d\n", x, y);
	Write_SPI_TFT_Reg(0x03,(x>>0));
  	Write_SPI_TFT_Reg(0x02,(x>>8));
  	Write_SPI_TFT_Reg(0x07,(y>>0));
  	Write_SPI_TFT_Reg(0x06,(y>>8));
  	Write_SPI_TFT_Cmd(0x22);
  	Write_SPI_TFT_Dat(color);
}

short Read_SPI_TFT_Reg(short reg) {
	Write_SPI_TFT_Cmd(reg);
	//Caldria llegir 16 bits
	return 0x0000;

}

//TFT Level Functions
short Orig_X=0;
short Orig_Y=0;
short Size_X=240;
short Size_Y=320;

void SPI_TFT_Reset() {

	//Reset the TFT 	
	Reset_Pantalla_0
  	delay(250); 
  	Reset_Pantalla_1     
  	delay(6);   
  
  	short driverCode = Read_SPI_TFT_Reg(0x00);
  
  	// Start Initial Sequence
    Write_SPI_TFT_Reg(0xEA, 0x0000);    //Reset Power Control 1 
  	Write_SPI_TFT_Reg(0xEB, 0x0020);    //Power Control 2   
  	Write_SPI_TFT_Reg(0xEC, 0x000C);    //Power Control 3   
  	Write_SPI_TFT_Reg(0xED, 0x00C4);    //Power Control 4   
  	Write_SPI_TFT_Reg(0xE8, 0x0040);    //Source OPON_N     
  	Write_SPI_TFT_Reg(0xE9, 0x0038);    //Source OPON_I     
	Write_SPI_TFT_Reg(0xF1, 0x0001);                
	Write_SPI_TFT_Reg(0xF2, 0x0010);                
  	Write_SPI_TFT_Reg(0x27, 0x00A3);   //Display Control 2 

	// Power On sequence 
 	Write_SPI_TFT_Reg(0x1B, 0x001B);    //Power Control 2 
  	Write_SPI_TFT_Reg(0x1A, 0x0001);    //Power Control 1
  	Write_SPI_TFT_Reg(0x24, 0x002F);    //Vcom Control 2
  	Write_SPI_TFT_Reg(0x25, 0x0057);    //Vcom Control 3                       
  	Write_SPI_TFT_Reg(0x23, 0x008D);    //Vcom Control 1
  
  	// Gamma settings
  	Write_SPI_TFT_Reg(0x40,0x00);
  	Write_SPI_TFT_Reg(0x41,0x00);
  	Write_SPI_TFT_Reg(0x42,0x01);
  	Write_SPI_TFT_Reg(0x43,0x13);
  	Write_SPI_TFT_Reg(0x44,0x10);
  	Write_SPI_TFT_Reg(0x45,0x26);
  	Write_SPI_TFT_Reg(0x46,0x08);
  	Write_SPI_TFT_Reg(0x47,0x51);
  	Write_SPI_TFT_Reg(0x48,0x02);
  	Write_SPI_TFT_Reg(0x49,0x12);
  	Write_SPI_TFT_Reg(0x4A,0x18);
  	Write_SPI_TFT_Reg(0x4B,0x19);
  	Write_SPI_TFT_Reg(0x4C,0x14);
  	Write_SPI_TFT_Reg(0x50,0x19);
  	Write_SPI_TFT_Reg(0x51,0x2F);
  	Write_SPI_TFT_Reg(0x52,0x2C);
  	Write_SPI_TFT_Reg(0x53,0x3E);
  	Write_SPI_TFT_Reg(0x54,0x3F);
  	Write_SPI_TFT_Reg(0x55,0x3F);
  	Write_SPI_TFT_Reg(0x56,0x2E);
 	Write_SPI_TFT_Reg(0x57,0x77);
  	Write_SPI_TFT_Reg(0x58,0x0B);
  	Write_SPI_TFT_Reg(0x59,0x06);
  	Write_SPI_TFT_Reg(0x5A,0x07);
  	Write_SPI_TFT_Reg(0x5B,0x0D);
  	Write_SPI_TFT_Reg(0x5C,0x1D);
  	Write_SPI_TFT_Reg(0x5D,0xCC);
  
  	//Power + Osc 
  	Write_SPI_TFT_Reg(0x18,0x0036);      //OSC Control 1 
  	Write_SPI_TFT_Reg(0x19,0x0001);      //OSC Control 2
  	Write_SPI_TFT_Reg(0x01,0x0000);      //Display Mode Control
  	Write_SPI_TFT_Reg(0x1F,0x0088);      //Power Control 6
  	delay(5);             
  	Write_SPI_TFT_Reg(0x1F,0x0080);     //Power Control 6              
  	delay(5);                                      
  	Write_SPI_TFT_Reg(0x1F,0x0090);      //Power Control 6
  	delay(5);             
  	Write_SPI_TFT_Reg(0x1F,0x00D0);      //Power Control 6
  	delay(5);                                        
 
	//Appearance
  	Write_SPI_TFT_Reg(0x17,0x0005);      //Colmod 16Bit/Pixel
  	Write_SPI_TFT_Reg(0x36,0x0000);      //Panel Characteristic
  	Write_SPI_TFT_Reg(0x28,0x0038);      //Display Control 3        
  	delay(4);                 
  	Write_SPI_TFT_Reg(0x28,0x003C);      //Display Control 3
  	delay(2);             
  	Write_SPI_TFT_Reg(0x16,0x0008);	    //Orientation
  	Write_SPI_TFT_Reg(0x03,(Orig_X>>0)); //Set Dimensions
  	Write_SPI_TFT_Reg(0x02,(Orig_X>>8));
  	Write_SPI_TFT_Reg(0x05,((Size_X-1)>>0));
  	Write_SPI_TFT_Reg(0x04,((Size_X-1)>>8));
  	Write_SPI_TFT_Reg(0x07,(Orig_Y>>0));
  	Write_SPI_TFT_Reg(0x06,(Orig_Y>>8));
  	Write_SPI_TFT_Reg(0x09,((Size_Y-1)>>0));
  	Write_SPI_TFT_Reg(0x08,((Size_Y-1)>>8));

}

int oldX, oldY;

void pintaPilota(int x, int y) {
	int i, j;
    
	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 8; ++j) SPI_TFT_pixel((oldX+i), (oldY+j), 0xffff);
	}
	for (i = 0; i < 8; ++i) {
		for (j = 0; j < 8;++j) SPI_TFT_pixel((x+i), (y+j), 0);
	}
	oldX = x;
	oldY = y;
}

void pintaLletra(int x, int y, int lletra){
	int i, j;
	short valors, mask;
	//char pixel;
	for (i = 0; i < 5; ++i) {
		valors = font5x7[(lletra-32)*5 + i];
		mask = 128;
		for (j = 7; j >= 0; --j) {
			if ((valors & mask) > 0) 
				SPI_TFT_pixel((x+i), (y+j), 0);
			else SPI_TFT_pixel((x+i), (y+j), 0xffff);
			mask >>= 1;
			//printf("%d\n",j);
		}
	}
}

void pintaString(int x, int y, char* s, int size){
	int i;
	int space = 0;
	for(i = 0; i < size; ++i) {
		pintaLletra(x+(i*5)+space,y,s[i]);
		space += 3;
	}
}


int main (void)
{
  wiringPiSetupPhys ();
  pinMode (15, OUTPUT);	// CS
  pinMode (7, OUTPUT);	// CLK
  pinMode (13, OUTPUT);	// DI
  pinMode (11, INPUT);	// DO
  pinMode (16, OUTPUT); // CS_Pantalla
  pinMode (22, OUTPUT); // Reset_Pantalla
  pinMode (18, OUTPUT); // CLK_Pantalla

  SPI_TFT_Reset();

  int coords[3];
  oldX = 0;
  oldY = 0;

  int coordx, coordy;
  printf("Cleaning\n");
  for (coordx = 0; coordx < 240; ++coordx) {
		for (coordy = 0; coordy < 320; ++coordy) SPI_TFT_pixel(coordx,coordy,0xffff);	
	}

  while(1) {

      int it; 
	  for(it = 0; it < 3; ++it) {

		  digitalWrite (15, LOW); //CS

	  	  DO_SPI_1	// Start
	  	  CLK_SPI

		  if (it == 0) channel0();
		  else if (it == 1) channel1();
		  else channel2();

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
			coords[it] = readVal;
		  	//printf("Channel%d: %d\t",it,readVal);
		  	digitalWrite (15, HIGH); //CS
			delay(20);	

	  }
	  //printf("\n");

	  //SPI_TFT_pixel(coords[0]%240, coords[1]%320, 0);

	  pintaPilota(coords[1],coords[0]);

	  char s[100] = "Javi Puto Amo";
	  pintaString(20,20,s,strlen(s));

	  int tactil[2];
	  while(1){
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
		  
		  	int readVal = 0, i;
		  	for (i = 0; i < 12; ++i) {
				CLK_SPI    
				readVal = readVal << 1;
				if (digitalRead(11)) {
			  		readVal += 1;
				}
		  	}
			tactil[it] = readVal;
		  	digitalWrite (15, HIGH); //CS
			delay(20);	
	  	}
	  	printf("CoordX: %d, CoordY: %d\n",tactil[0], tactil[1]);
	  	if (tactil[0] > 460 && tactil[1] >1750) {
	  		pintaPilota((tactil[0]-460)*240/3040,(tactil[1]-1750)*320/350-50);
	  	}
	  }

  }
  
  return 0 ;
}
