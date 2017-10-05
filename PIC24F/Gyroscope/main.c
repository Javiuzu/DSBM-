/*
 * File:   main.c
 * Author: student
 *
 * Created on 6 / abril / 2017, 12:40
 */
#define _XTAL_FREQ 8000000

// FBS
#pragma config BWRP = OFF               // Table Write Protect Boot (Boot segment may be written)
#pragma config BSS = OFF                // Boot segment Protect (No boot program Flash segment)

// FGS
#pragma config GWRP = OFF               // General Segment Code Flash Write Protection bit (General segment may be written)
#pragma config GCP = OFF                // General Segment Code Flash Code Protection bit (No protection)

// FOSCSEL
#pragma config FNOSC = FRCDIV           // Oscillator Select (8 MHz FRC oscillator with divide-by-N (FRCDIV))
#pragma config IESO = ON                // Internal External Switch Over bit (Internal External Switchover mode enabled (Two-Speed Start-up enabled))

// FOSC
#pragma config POSCMOD = NONE           // Primary Oscillator Configuration bits (Primary oscillator disabled)
#pragma config OSCIOFNC = OFF           // CLKO Enable Configuration bit (CLKO output signal is active on the OSCO pin)
#pragma config POSCFREQ = HS            // Primary Oscillator Frequency Range Configuration bits (Primary oscillator/external clock input frequency greater than 8 MHz)
#pragma config SOSCSEL = SOSCHP         // SOSC Power Selection Configuration bits (Secondary oscillator configured for high-power operation)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Both Clock Switching and Fail-safe Clock Monitor are disabled)

// FWDT
#pragma config WDTPS = PS32768          // Watchdog Timer Postscale Select bits (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (WDT prescaler ratio of 1:128)
#pragma config WINDIS = OFF             // Windowed Watchdog Timer Disable bit (Standard WDT selected; windowed WDT disabled)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))

// FPOR
#pragma config BOREN = BOR3             // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware; SBOREN bit disabled)
#pragma config PWRTEN = ON              // Power-up Timer Enable bit (PWRT enabled)
#pragma config I2C1SEL = PRI            // Alternate I2C1 Pin Mapping bit (Default location for SCL1/SDA1 pins)
#pragma config BORV = V18               // Brown-out Reset Voltage bits (Brown-out Reset set to lowest voltage (1.8V))
#pragma config MCLRE = ON               // MCLR Pin Enable bit (MCLR pin enabled; RA5 input pin disabled)

// FICD
#pragma config ICS = PGx1               // ICD Pin Placement Select bits (PGC1/PGD1 are used for programming and debugging the device)

// FDS
#pragma config DSWDTPS = DSWDTPSF       // Deep Sleep Watchdog Timer Postscale Select bits (1:2,147,483,648 (25.7 Days))
#pragma config DSWDTOSC = LPRC          // DSWDT Reference Clock Select bit (DSWDT uses LPRC as reference clock)
#pragma config RTCOSC = SOSC            // RTCC Reference Clock Select bit (RTCC uses SOSC as reference clock)
#pragma config DSBOREN = ON             // Deep Sleep Zero-Power BOR Enable bit (Deep Sleep BOR enabled in Deep Sleep)
#pragma config DSWDTEN = ON             // Deep Sleep Watchdog Timer Enable bit (DSWDT enabled)

#include "xc.h"
#include <libpic30.h>        // __delayXXX() functions macros defined here


void init_bits(){
    AD1PCFG = 0xFFEC;
    /*AD1PCFGbits.PCFG0 = 0;
    AD1PCFGbits.PCFG1 = 0;
    AD1PCFGbits.PCFG4 = 0;*/
    AD1CON2 = AD1CON2 & 0x1FFF;
    //AD1CON3 = 0x1020;
    AD1CON3 = 0x1F02;
    AD1CON1 = 0x80E0;



    //MCLR
    TRISAbits.TRISA5 = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;

    // SPI
    TRISBbits.TRISB12 = 1;
    TRISBbits.TRISB13 = 0;
    TRISBbits.TRISB14 = 1;
    TRISBbits.TRISB15 = 1;

    // AD ACCELEROMETRE
    TRISAbits.TRISA0 = 1;

}

#define SS_SPI PORTBbits.RB15
#define CLK_SPI PORTBbits.RB12
#define D0_SPI PORTBbits.RB13
#define DI_SPI PORTBbits.RB14

short coordant[3];
short coordDerivat[3];

void derivador(short coord[3]) {
    coordDerivat[0] = coordant[0] - coord[0];
    coordDerivat[1] = coordant[1] - coord[1];
    coordDerivat[2] = coordant[2] - coord[2];
    coordant[0] = coord[0];
    coordant[1] = coord[1];
    coordant[2] = coord[2];
}

void coordDerivades() {
    short readVal[3];
    AD1CON1bits.ADON = 1; // turn ADC ON
    
    while (!SS_SPI);
    while (SS_SPI);

    // READ AD
    AD1CHS = 0x0000;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    readVal[0] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;


    AD1CHS = 0x0001;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    readVal[1] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    AD1CHS = 0x0004;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    readVal[2] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    derivador(readVal);

    int coords = 0;
    while (coords < 3) {
        int i = 15;
        unsigned int aux = 0x8000;
        while (i >= 0) {

            while (CLK_SPI);
            while (!CLK_SPI) {
                unsigned int tmp = aux & coordDerivat[coords];
                if (tmp > 0) D0_SPI = 1;
                else D0_SPI = 0;
            }
            aux = aux >> 1;
            --i;
        }
        ++coords;
    }
}

short offset[3];

void offsetFun() {
    short readVal[3];
    AD1CON1bits.ADON = 1; // turn ADC ON

    while (!SS_SPI);
    while (SS_SPI);

    // READ AD
    AD1CHS = 0x0000;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    readVal[0] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    AD1CHS = 0x0001;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    readVal[1] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    AD1CHS = 0x0004;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    readVal[2] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    int coords = 0;
    while (coords < 3) {
        int i = 15;
        unsigned int aux = 0x8000;
        while (i >= 0) {

            while (CLK_SPI);
            while (!CLK_SPI) {
                unsigned int tmp = aux & (readVal[coords] - offset[coords]);
                if (tmp > 0) D0_SPI = 1;
                else D0_SPI = 0;
            }
            aux = aux >> 1;
            --i;
        }
        ++coords;
    }
}

short filtreMatrix [8][3];
int pos = 0;
int it = 0;

void filtreFun() {
    int readVal[3] = {0,0,0};
    AD1CON1bits.ADON = 1; // turn ADC ON

    while (!SS_SPI);
    while (SS_SPI);

    // READ AD
    AD1CHS = 0x0000;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    filtreMatrix[pos][0] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    AD1CHS = 0x0001;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    filtreMatrix[pos][1] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    AD1CHS = 0x0004;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    filtreMatrix[pos][2] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    pos += 1;
    pos = pos % 8;
    if (it < 8) ++it;

    int j;
    for (j = 0; j < it; ++j){
        readVal[0] += filtreMatrix[j][0];
        readVal[1] += filtreMatrix[j][1];
        readVal[2] += filtreMatrix[j][2];
    }

    readVal[0] /= it;
    readVal[1] /= it;
    readVal[2] /= it;

    int coords = 0;
    while (coords < 3) {
        int i = 15;
        unsigned int aux = 0x8000;
        while (i >= 0) {

            while (CLK_SPI);
            while (!CLK_SPI) {
                unsigned int tmp = aux & readVal[coords];
                if (tmp > 0) D0_SPI = 1;
                else D0_SPI = 0;
            }
            aux = aux >> 1;
            --i;
        }
        ++coords;
    }
}

short filtreCoord[3];
short offsetCoord[3];

void generalFunc ()
{
    AD1CON1bits.ADON = 1; // turn ADC ON

    // READ AD
    AD1CHS = 0x0000;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    filtreMatrix[pos][0] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    AD1CHS = 0x0001;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    filtreMatrix[pos][1] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    AD1CHS = 0x0004;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    filtreMatrix[pos][2] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    // Calcul de Filtre
    if (it < 8) ++it;
    int j;
    for (j = 0; j < it; ++j){
        filtreCoord[0] += filtreMatrix[j][0];
        filtreCoord[1] += filtreMatrix[j][1];
        filtreCoord[2] += filtreMatrix[j][2];
    }

    filtreCoord[0] /= it;
    filtreCoord[1] /= it;
    filtreCoord[2] /= it;

    // Calcul offset
    offsetCoord[0] = filtreMatrix[pos][0] - offset[0];
    offsetCoord[1] = filtreMatrix[pos][1] - offset[1];
    offsetCoord[2] = filtreMatrix[pos][2] - offset[2];

    // Calcul Derivador
    derivador(filtreMatrix[pos]);

    pos += 1;
    pos = pos % 8;
}

int main(void) {
    init_bits();

    // TEST
    /*while(1){
        PORTBbits.RB2 = 1;
        //__delay_ms(500);
        int i;
        for (i = 0; i < 10000; ++i);
        PORTBbits.RB2 = 0;
        //__delay_ms(500);
        for (i = 0; i < 10000; ++i);
        for (i = 0; i < 10000; ++i);
    }*/

    // SPI
    // int bits[16] = {0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1};
    short readVal[3];
    AD1CON1bits.ADON = 1; // turn ADC ON

    // READ AD
    AD1CHS = 0x0000;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    readVal[0] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    AD1CHS = 0x0001;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    readVal[1] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    AD1CHS = 0x0004;
    AD1CON1bits.SAMP = 1;
    while(!AD1CON1bits.DONE); //Wait for A/D convert complete
    readVal[2] = ADC1BUF0; //Get the value from the A/D
    AD1CON1bits.DONE = 0; //Clear A/D Interrupt Flag
    AD1CON1bits.SAMP = 0;

    offset[0] = readVal[0];
    offset[1] = readVal[1];
    offset[2] = readVal[2];

    while(1) {

        TRISBbits.TRISB13 = 1;
        while (!SS_SPI);
        while (SS_SPI);

        TRISBbits.TRISB13 = 0;

        int i = 2, op = 0;
        while (i > 0) {
            while (CLK_SPI);
            while (!CLK_SPI);
            op = op << 1;
            op += DI_SPI;
            --i;
        }

        /*generalFunc();

        while (!SS_SPI);
        while (SS_SPI);

        int coords = 0;
        while (coords < 3) {
            int i = 15;
            unsigned int aux = 0x8000;
            while (i >= 0) {

                while (CLK_SPI);
                while (!CLK_SPI) {
                    unsigned int tmp;

                    if (op == 0) tmp = aux & filtreCoord[coords];
                    else if (op == 1) tmp = coordDerivat[coords];
                    else tmp = aux & offsetCoord[coords];

                    if (tmp > 0) D0_SPI = 1;
                    else D0_SPI = 0;
                }
                aux = aux >> 1;
                --i;
            }
            ++coords;
        }*/

        if (op == 0) filtreFun();
        else if (op == 1) coordDerivades();
        else offsetFun();
    }

    return 0;
}
