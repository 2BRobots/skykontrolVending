/* 
 * File:   main.c
 * Author: dannimakes
 *
 * Created on 13 de enero de 2020, 06:30 PM
 */

// PIC12LF1840 Configuration Bit Settings

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = ON        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = ON       // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is RESET)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config PLLEN = ON       // PLL Enable (4x PLL enabled)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset adc Selection (Brown-out Reset adc (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-adc Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// Includes and definitions

#define _XTAL_FREQ 32000000
#define DEVICE_ID 0x02
#define I2C_slave_address 0x3E // any value from 0 to 127, this will be the default address
#define VCAL 234.26F
#define ICAL 111.11F

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

//EEPROM default values and function prototypes for memory access on program

__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //set ctRatio, ctRl, trRatio
__EEPROM_DATA(0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF); //samples
unsigned char eeprom_read(unsigned char address);
void eeprom_write(unsigned char address, unsigned char value);

// Global variables declation

float adc0 = 0;
float adc1 = 0;
float ical = 0;
float vcal = 0;
float iacc = 0;
float vacc = 0;
float pacc = 0;
float pf = 0;

volatile union _I2C_buffer {

    struct _data {
        unsigned char ID;
        unsigned char RESET;
        unsigned char SAVE;
        int ctRatio;
        int ctRl;
        float trRatio;
        int samples;
        float Irms;
        float Vrms;
        float Power;
        float AvPower;
        float PwFactor;
    } data;
    unsigned char byte[];
} I2C_buffer;

const unsigned char RX_ELMNTS = sizeof (I2C_buffer);
unsigned char first_i2c = 1;
unsigned char index_i2c = 0;

// Functions

unsigned int ADC_read(unsigned char channel) {
    ADCON0 = (unsigned char) (channel << 2); //select channel
    ADCON0bits.ADON = 1; //enable ADC
    __delay_ms(1); //wait to charge holding capacitor
    ADCON0bits.GO_nDONE = 1; //start conversion
    while (ADCON0bits.GO_nDONE); //wait ADC to finish
    ADCON0bits.ADON = 0; //disable ADC
    return (((unsigned int) ADRESH) << 8) | ((unsigned int) ADRESL & 0xFF); //return ADC result
}

void init_I2C_buffer() //load default values of vars
{
    I2C_buffer.data.ID = DEVICE_ID;
    I2C_buffer.data.RESET = 0;
    I2C_buffer.data.SAVE = 0;
    I2C_buffer.byte[0x03] = eeprom_read(0); //ctRatio
    I2C_buffer.byte[0x04] = eeprom_read(1);
    I2C_buffer.byte[0x05] = eeprom_read(2); //ctRl;
    I2C_buffer.byte[0x06] = eeprom_read(3);
    I2C_buffer.byte[0x07] = eeprom_read(4); //trRatio;
    I2C_buffer.byte[0x08] = eeprom_read(5);
    I2C_buffer.byte[0x09] = eeprom_read(6);
    I2C_buffer.byte[0x0A] = eeprom_read(7);
    I2C_buffer.byte[0x0B] = eeprom_read(8); //samples;
    I2C_buffer.byte[0x0C] = eeprom_read(9);
    I2C_buffer.data.Irms = 0;
    I2C_buffer.data.Vrms = 0;
    I2C_buffer.data.Power = 0;
    I2C_buffer.data.AvPower = 0;
    I2C_buffer.data.PwFactor = 1.0F;
}

// Interrupts

void __interrupt isr() {

    static unsigned char junk = 0;

    if (PIR1bits.SSP1IF) // check to see if SSP interrupt
    {
        INTCONbits.IOCIE = 0; //disable on change interrupts
        PIE1bits.TMR1IE = 0; //disable timer1 interrupt
        if (SSP1STATbits.R_nW) // Master read (R_nW = 1)
        {
            if (!SSP1STATbits.D_nA) // Last byte was an address (D_nA = 0)
            {
                SSP1BUF = I2C_buffer.byte[index_i2c++]; // load with value from array
                SSP1CON1bits.CKP = 1; // Release CLK
            }
            if (SSP1STATbits.D_nA) // Last byte was data (D_nA = 1)
            {
                SSP1BUF = I2C_buffer.byte[index_i2c++]; // load with value from array
                SSP1CON1bits.CKP = 1; // Release CLK
            }
        }
        if (!SSP1STATbits.R_nW) //  Master write (R_nW = 0)
        {
            if (!SSP1STATbits.D_nA) // Last byte was an address (D_nA = 0)
            {
                first_i2c = 1; //last byte was address, next will be data location
                junk = SSP1BUF; // read buffer to clear BF
                SSP1CON1bits.CKP = 1; // Release CLK
            }
            if (SSP1STATbits.D_nA) // Last byte was data (D_nA = 1)
            {
                if (first_i2c) {
                    index_i2c = SSP1BUF; // load index with array location
                    first_i2c = 0; // now clear this since we have 
                }//location to read from/write to

                else {
                    if (index_i2c < RX_ELMNTS) // make sure index is not
                    { //out of range of array
                        I2C_buffer.byte[index_i2c++] = SSP1BUF; //load array with data
                    } else {
                        junk = SSP1BUF; //array location not valid, discard data
                    }
                }
                if (SSP1CON1bits.WCOL) // Did a write collision occur?
                {
                    SSP1CON1bits.WCOL = 0; //  clear WCOL
                    junk = SSP1BUF; // dummy read to clear BF bit
                }
                SSP1CON1bits.CKP = 1; // Release CLK
            }
        }
        PIR1bits.SSP1IF = 0; // clear SSPIF flag bit
        INTCONbits.IOCIE = 1; //enable on change interrupts
        PIE1bits.TMR1IE = 1; //enable timer1 interrupt
    }

    if (PIR2bits.BCL1IF) // Did a bus collision occur?
    {
        junk = SSP1BUF; // dummy read SSPBUF to clear BF bit
        PIR2bits.BCL1IF = 0; // clear bus collision Int Flag bit
        SSP1CON1bits.CKP = 1; // Release CLK
        PIR1bits.SSP1IF = 0; // clear SSPIF flag bit
    }
}

int main(int argc, char** argv) {
    OSCCON = 0b11110000; //configure internal oscilator for 32Mhz
    init_I2C_buffer();
    TRISA = 0b00011111; //configure IO
    ANSELA = 0b00010001; //analog functions of pins enabled
    WPUA = 0b00001110; //configure weak pull-ups on input pins
    OPTION_REGbits.nWPUEN = 0; //enable weak pull-ups
    ADCON1 = 0b11110000; //configure ADC
    SSP1STAT = 0b10000000; // Slew rate control disabled for standardspeed mode (100 kHz and 1 MHz)
    SSP1CON1 = 0b00110110; // Enable serial port, I2C slave mode, 7-bit address
    SSP1CON2bits.SEN = 1; // Clock stretching is enabled
    SSP1CON3bits.BOEN = 1; // SSPBUF is updated and NACK is generated for a received address/data byte, ignoring the state of the SSPOV bit only if the BF bit = 0
    SSP1CON3bits.SDAHT = 1; // Minimum of 300 ns hold time on SDA after the falling edge of SCL
    SSP1CON3bits.SBCDE = 1; // Enable slave bus collision detect interrupts
    SSP1ADD = (char) (I2C_slave_address << 1); // Load the slave address
    PIR1bits.SSP1IF = 0; // Clear the serial port interrupt flag
    PIR2bits.BCL1IF = 0; // Clear the bus collision interrupt flag
    PIE2bits.BCL1IE = 1; // Enable bus collision interrupts
    PIE1bits.SSP1IE = 1; // Enable serial port interrupts
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    INTCON = 0b01000000; //enables interrupts
    INTCONbits.GIE = 1; //run interrupts

    LATAbits.LATA5 = 1; //turn led ON

    while (1) {
        asm("CLRWDT");
        if (I2C_buffer.data.RESET == 1) {
            asm("RESET");
        }

        if (I2C_buffer.data.SAVE == 1) { //Save non-volatile data into the EEPROM
            eeprom_write(0, I2C_buffer.byte[0x03]); //ctRatio
            eeprom_write(1, I2C_buffer.byte[0x04]);
            eeprom_write(2, I2C_buffer.byte[0x05]); //ctRl
            eeprom_write(3, I2C_buffer.byte[0x06]);
            eeprom_write(4, I2C_buffer.byte[0x07]); //trRatio
            eeprom_write(5, I2C_buffer.byte[0x08]);
            eeprom_write(6, I2C_buffer.byte[0x09]);
            eeprom_write(7, I2C_buffer.byte[0x0A]);
            eeprom_write(8, I2C_buffer.byte[0x0B]); //samples
            eeprom_write(9, I2C_buffer.byte[0x0C]);
            I2C_buffer.data.SAVE = 0;
            __delay_ms(10);
        }

        iacc = 0;
        vacc = 0;
        pacc = 0;

        for (int i = 0; i < I2C_buffer.data.samples; i++) //rms current in transformer primary
        {
            asm("CLRWDT");

            adc0 = (float) ADC_read(0x00); //read adc for current transformer
            adc1 = (float) ADC_read(0x03); //read adc for voltage transformer

            ical = ((adc0 * 3.3F) / 1024.0F) - 1.65F; //convert ADC to voltage
            ical = (ical / (float) I2C_buffer.data.ctRl) * (float) I2C_buffer.data.ctRatio; //conver to current in the line
            iacc += fabs(ical * ical); //acumulate RMS value for sampling

            vcal = ((adc1 * 3.3F) / 1024.0F) - 1.65F; //convert ADC to voltage
            vcal = vcal * I2C_buffer.data.trRatio; //multiply voltage for the transformer relation 
            vacc += fabs(vcal * vcal); //acumulate RMS value for sampling

            pacc += fabs(vcal * ical);
        }

        I2C_buffer.data.Irms = sqrt((iacc / (float) I2C_buffer.data.samples)); //RMS current
        I2C_buffer.data.Vrms = sqrt((vacc / (float) I2C_buffer.data.samples)); //RMS voltage
        I2C_buffer.data.PwFactor = (pacc / (float) I2C_buffer.data.samples) / (I2C_buffer.data.Vrms * I2C_buffer.data.Irms); //pf;
        I2C_buffer.data.Power = I2C_buffer.data.Vrms * I2C_buffer.data.Irms * I2C_buffer.data.PwFactor; //apparent Power 
        I2C_buffer.data.AvPower = (I2C_buffer.data.Power + I2C_buffer.data.AvPower) / 2.0F; //average apparent power
        LATAbits.LATA5 = (char) !LATAbits.LATA5; //toggle led

    }

    return (EXIT_SUCCESS);
}

