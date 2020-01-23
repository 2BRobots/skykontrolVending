/* 
 * File:   main.c
 * Author: dannimakes
 *
 * Created on 13 de enero de 2020, 06:30 PM
 */

// PIC12LF1822 Configuration Bit Settings

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
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LVP = OFF        // Low-Voltage Programming Enable (High-voltage on MCLR/VPP must be used for programming)

// Includes and definitions

#define _XTAL_FREQ 32000000
#define DEVICE_ID 0x02
#define I2C_slave_address 0x3E // any value from 0 to 127, this will be the default address

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

// Global variables declation

volatile union _I2C_buffer {

    struct _data {
        unsigned char ID;
        unsigned char RESET;
        unsigned int AN0;
        unsigned int AN4;
    } data;
    unsigned char byte[];
} I2C_buffer;

const unsigned char RX_ELMNTS = sizeof (I2C_buffer);
unsigned char first_i2c = 1;
unsigned char index_i2c = 0;
int ledDim = 0;
char dir = 1;

// Functions

void PWM_Init(void) // start the PWM with a 0% duty
{
    /*
    //10 bit mode PWM at 1.95kHz
    PR2 = 0xFF; //load PR2 register
    CCP1CON = 0b00001100; //configure CCP1 for PWM operation
    CCPR1L = 0b00000000; //set duty to 0    
    PIR1bits.TMR2IF = 0; //clear the interrupt flag
    T2CON = 0b00000110; //start timer 2 and select prescaler as 4
     */

    /*    
    //10 bit mode PWM at 7.81kHz
    PR2 = 0xFF; //load PR2 register
    CCP1CON = 0b00001100; //configure CCP1 for PWM operation
    CCPR1L = 0b00000000; //set duty to 0    
    PIR1bits.TMR2IF = 0; //clear the interrupt flag
    T2CON = 0b00000101; //start timer 2 and select prescaler as 4
     */

    /* 
    //10 bit mode PWM at 31.25kHz
    PR2 = 0xFF; //load PR2 register
    CCP1CON = 0b00001100; //configure CCP1 for PWM operation
    CCPR1L = 0b00000000; //set duty to 0    
    PIR1bits.TMR2IF = 0; //clear the interrupt flag
    T2CON = 0b00000100; //start timer 2 and select prescaler as 1
     */

    //8 bit mode PWM at 125kHz
    PR2 = 0x3F; //load PR2 register
    CCP1CON = 0b00001100; //configure CCP1 for PWM operation
    CCPR1L = 0b00000000; //set duty to 0    
    PIR1bits.TMR2IF = 0; //clear the interrupt flag
    T2CON = 0b00000100; //start timer 2 and select prescaler as 1
}

void PWM_set_duty(unsigned int duty) // change the duty of PWM
{
    if (duty < 255) {
        //CCPR1L = (0xFF & ((unsigned int) duty >> 2)); //discard 2 LSB and write the rest to register, 10 bit mode
        //CCP1CON = (0x0C | (0x30 & ((unsigned int) duty << 4))); //filter the reamaining 2 LSB and then write it to DC1B (bit 5,4 in the register), 10 bit mode
        CCPR1L = (0xFF & ((unsigned int) duty)); //to be used in 8 bit mode
    }
}

unsigned int ADC_read(unsigned char channel) {
    ADCON0 = (unsigned char)(channel << 2); //select channel
    ADCON0bits.ADON = 1; //enable ADC
    __delay_ms(1); //wait to charge holding capacitor
    ADCON0bits.GO_nDONE = 1;//start conversion
    while (ADCON0bits.GO_nDONE); //wait ADC to finish
    ADCON0bits.ADON = 0; //disable ADC
    return (((unsigned int)ADRESH) << 8) | ((unsigned int)ADRESL & 0xFF); //return ADC result
}

void init_I2C_buffer() //load default values of vars
{
    I2C_buffer.data.ID = DEVICE_ID;
    I2C_buffer.data.RESET = 0;
    I2C_buffer.data.AN0 = 0;
    I2C_buffer.data.AN4 = 0;
}

// Interrupts

void __interrupt isr() {

    if (PIR1bits.TMR1IF == 1) //timer1 interrupt, called every 65.536ms
    {
        T1CONbits.TMR1ON = 0; //stop timer1
        if (dir == 1) {
            ledDim += 2;
            if (ledDim >= 50) {
                ledDim = 50;
                dir = 0;
            }
        } else {
            ledDim -= 2;
            if (ledDim <= 0) {
                ledDim = 0;
                dir = 1;
            }
        }
        PWM_set_duty((unsigned int) ledDim); //dim LED dinamically
        PIR1bits.TMR1IF = 0; //clear interrutp flag
        T1CONbits.TMR1ON = 1; //start timer1
    }

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
    OPTION_REGbits.nWPUEN = 1; //enable/disable weak pull-ups
    APFCONbits.CCP1SEL = 1; //select RA5 as CCP output pin
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
    PWM_Init(); //start pwm
    INTCON = 0b01001000; //enables interrupts
    T1CON = 0b00110100; //configure timer1 to run at 1 MHz
    PIE1bits.TMR1IE = 1; //enable timer1 interrupt
    T1CONbits.TMR1ON = 1; //start timer1
    INTCONbits.GIE = 1; //run interrupts

    while (1) {
        asm("CLRWDT");
        if (I2C_buffer.data.RESET == 1) {
            asm("RESET");
        }
        I2C_buffer.data.AN0 = ADC_read(0x00);
        I2C_buffer.data.AN4 = ADC_read(0x03);
        __delay_ms(1);
    }

    return (EXIT_SUCCESS);
}

