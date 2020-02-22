/* 
 * File:   main.c
 * Author: dannimakes
 *
 * Created on 11 de febrero de 2020, 04:58 PM
 */

// PIC16F1847 Configuration Bit Settings

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

#define _XTAL_FREQ 32000000    //frequency of th MCU
#define DEVICE_ID 0x03         //this is the device ID used for I2C validation
#define I2C_slave_address 0x40 // any value from 0 to 127, this will be the default

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

//EEPROM default values and function prototypes for memory access on program

__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00); //set default program values stored on EEPROM at programming
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
__EEPROM_DATA(0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
unsigned char eeprom_read(unsigned char address);
void eeprom_write(unsigned char address, unsigned char value);

// Global variables declation

volatile union _I2C_buffer {

    struct _data {
        unsigned char ID; //0x00
        unsigned char RESET; //0x01
        unsigned char SAVE; //0x02
        unsigned char dispense; //0x03
        unsigned char selected; //0x04
        unsigned char cancel; //0x05
        unsigned long counter; //0x06
        unsigned long time; //0x0A
        unsigned char calibrate; //0x0E
        unsigned char relay1; //0x0F
        unsigned char relay2; //0x10
        unsigned char relay3; //0x11
        unsigned char relay4; //0x12

        unsigned char selected1; //0x13
        unsigned char product_id1[13]; //0x14
        unsigned char name1[21]; //0x21
        unsigned char unit1[3]; //0x36
        unsigned long time1; //0x39
        unsigned long counter1; //0x3D
        unsigned int cost1; //0x41
        unsigned int quantity1; //0x43
        unsigned int stock1; //0x45
        unsigned int capacity1; //0x47
        unsigned int emptyLevel1; //0x49
        unsigned int fullLevel1; //0x4B

        unsigned char selected2; //0x4D
        unsigned char product_id2[13]; //0x4E
        unsigned char name2[21]; //0x5B
        unsigned char unit2[3]; //0x70
        unsigned long time2; //0x73
        unsigned long counter2; //0x77
        unsigned int cost2; //0x7B
        unsigned int quantity2; //0x7D
        unsigned int stock2; //0x7F
        unsigned int capacity2; //0x81
        unsigned int emptyLevel2; //0x83
        unsigned int fullLevel2; //0x85

        unsigned char selected3; //0x87
        unsigned char product_id3[13]; //0x88
        unsigned char name3[21]; //0x95
        unsigned char unit3[3]; //0xAA
        unsigned long time3; //0xAD
        unsigned long counter3; //0xB1
        unsigned int cost3; //0xB5
        unsigned int quantity3; //0xB7
        unsigned int stock3; //0xB9
        unsigned int capacity3; //0xBB
        unsigned int emptyLevel3; //0xBD
        unsigned int fullLevel3; //0xBF

        unsigned char selected4; //0xC1
        unsigned char product_id4[13]; //0xC2
        unsigned char name4[21]; //0xCF
        unsigned char unit4[3]; //0xE4
        unsigned long time4; //0xE7
        unsigned long counter4; //0xEB
        unsigned int cost4; //0xEF
        unsigned int quantity4; //0xF1
        unsigned int stock4; //0xF3
        unsigned int capacity4; //0xF5
        unsigned int emptyLevel4; //0xF7
        unsigned int fullLevel4; //0xF9

    } data;
    unsigned char byte[];
} I2C_buffer; //this allows to access vars directly as set types or bytes. Shared with main program and I2C communication

const unsigned char RX_ELMNTS = sizeof (I2C_buffer); //used in I2C data transfer routines
volatile unsigned char first_i2c = 1;
volatile unsigned char index_i2c = 0;
unsigned char average = 0;
unsigned int stock1 = 0;
unsigned int stock2 = 0;
unsigned int stock3 = 0;
unsigned int stock4 = 0;
unsigned char readADC = 0;

// Functions

unsigned long map(unsigned long x, unsigned long in_min, unsigned long in_max, unsigned long out_min, unsigned long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

unsigned int ADC_read(unsigned char channel) {
    ADCON0 = (unsigned char) (channel << 2); //select channel
    ADCON0bits.ADON = 1; //enable ADC
    __delay_ms(1); //wait to charge holding capacitor
    ADCON0bits.GO_nDONE = 1; //start conversion
    while (ADCON0bits.GO_nDONE); //wait ADC to finish
    ADCON0bits.ADON = 0; //disable ADC
    return (((unsigned int) ADRESH) << 8) | ((unsigned int) ADRESL & 0xFF); //return ADC result
}

void init_I2C_buffer() { //load default values of vars
    I2C_buffer.data.ID = DEVICE_ID;
    I2C_buffer.data.RESET = 0;
    I2C_buffer.data.SAVE = 0;
    I2C_buffer.data.dispense = 0;
    I2C_buffer.data.selected = 0;
    I2C_buffer.data.cancel = 0;
    I2C_buffer.data.counter = 0;
    I2C_buffer.data.time = 0;
    I2C_buffer.data.calibrate = 0;
    I2C_buffer.data.relay1 = 0;
    I2C_buffer.data.relay2 = 0;
    I2C_buffer.data.relay3 = 0;
    I2C_buffer.data.relay4 = 0;
    I2C_buffer.data.selected1 = 0;
    for (unsigned char i = 0; i < 58; i++) //data for slot1 (56 bytes) (product_id, name, unit, time, counter, cost, quantity, stock, capacity, emptyLevel, fullLevel)
    {
        I2C_buffer.byte[(unsigned char) (0x14 + i)] = eeprom_read(i);
    }
    I2C_buffer.data.selected2 = 0;
    for (unsigned char i = 0; i < 58; i++) //data for slot2 (product_id, name, unit, time, counter, cost, quantity, stock, capacity, emptyLevel, fullLevel)
    {
        I2C_buffer.byte[(unsigned char) (0x4E + i)] = eeprom_read((unsigned char) (58 + i));
    }
    I2C_buffer.data.selected3 = 0;
    for (unsigned char i = 0; i < 58; i++) //data for slot3 (product_id, name, unit, time, counter, cost, quantity, stock, capacity, emptyLevel, fullLevel)
    {
        I2C_buffer.byte[(unsigned char) (0x88 + i)] = eeprom_read((unsigned char) (115 + i));
    }
    I2C_buffer.data.selected4 = 0;
    for (unsigned char i = 0; i < 58; i++) //data for slot4 (product_id, name, unit, time, counter, cost, quantity, stock, capacity, emptyLevel, fullLevel)
    {
        I2C_buffer.byte[(unsigned char) (0xC2 + i)] = eeprom_read((unsigned char) (172 + i));
    }

}

// Interrupts

void __interrupt isr() {

    if (INTCONbits.INTF == 1) {//interrupt on CNC input
        INTCONbits.INTE = 0; //disable this interrupt
        if (PORTBbits.RB0 == 0) {
            if (I2C_buffer.data.dispense == 0) {
                I2C_buffer.data.cancel = 1;
                I2C_buffer.data.selected1 = 0;
                I2C_buffer.data.selected2 = 0;
                I2C_buffer.data.selected3 = 0;
                I2C_buffer.data.selected4 = 0;
            } else {
                I2C_buffer.data.counter++;
            }
        }
        INTCONbits.INTF = 0; //clear this interrupt flag
        INTCONbits.INTE = 1; //enable this interrupt
    }

    if (INTCONbits.IOCIF == 1) {//interrupt on change
        INTCONbits.IOCIE = 0; //disable on change interrupts
        if (IOCBFbits.IOCBF2 == 1) {
            if (PORTBbits.RB2 == 0) {
                I2C_buffer.data.cancel = 0;
                I2C_buffer.data.selected1 = 1;
                I2C_buffer.data.selected2 = 0;
                I2C_buffer.data.selected3 = 0;
                I2C_buffer.data.selected4 = 0;
            }
        }
        if (IOCBFbits.IOCBF3 == 1) {
            if (PORTBbits.RB3 == 0) {
                I2C_buffer.data.cancel = 0;
                I2C_buffer.data.selected1 = 0;
                I2C_buffer.data.selected2 = 1;
                I2C_buffer.data.selected3 = 0;
                I2C_buffer.data.selected4 = 0;
            }
        }
        if (IOCBFbits.IOCBF6 == 1) {
            if (PORTBbits.RB6 == 0) {
                I2C_buffer.data.cancel = 0;
                I2C_buffer.data.selected1 = 0;
                I2C_buffer.data.selected2 = 0;
                I2C_buffer.data.selected3 = 1;
                I2C_buffer.data.selected4 = 0;
            }
        }
        if (IOCBFbits.IOCBF7 == 1) {
            if (PORTBbits.RB7 == 0) {
                I2C_buffer.data.cancel = 0;
                I2C_buffer.data.selected1 = 0;
                I2C_buffer.data.selected2 = 0;
                I2C_buffer.data.selected3 = 0;
                I2C_buffer.data.selected4 = 1;
            }
        }
        IOCBF = 0; //clear interrupt on change flags
        INTCONbits.IOCIE = 1; //enable on change interrupts
    }

    if (PIR1bits.TMR1IF == 1) //timer1 interrupt, called every 65.536 ms
    {
        T1CONbits.TMR1ON = 0; //stop timer1
        asm("CLRWDT");
        readADC = 1;
        PIR1bits.TMR1IF = 0; //clear interrutp flag
        T1CONbits.TMR1ON = 1; //start timer1
    }

    static unsigned char junk = 0; //used to discard unwanted data on I2C

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
    init_I2C_buffer(); //load default values and configurations
    OPTION_REGbits.nWPUEN = 1; //allow weak pull-ups to activate
    ADCON1 = 0b11110000; //configure ADC
    TRISA = 0b00101111; //configure IO
    ANSELA = 0b00001111; //analog functions of pins certain disabled
    TRISB = 0b11011111; //configure IO
    ANSELB = 0b00000000; //analog functions of all pins disabled
    WPUA = 0b00100000; //configure weak pull-ups on input pins
    WPUB = 0b11001101; //configure weak pull-ups on input pins
    LATA = 0b00000000; //set all to LOW
    LATB = 0b00000000; //set all to LOW
    SSP1STAT = 0b10000000; // Slew rate control disabled for standardspeed I2C mode (100 kHz and 1 MHz)
    SSP1CON1 = 0b00110110; // Enable serial port, I2C slave mode, 7-bit address
    SSP1CON2bits.SEN = 1; // Clock stretching on I2C is enabled
    SSP1CON3bits.BOEN = 1; // I2C SSPBUF is updated and NACK is generated for a received address/data byte, ignoring the state of the SSPOV bit only if the BF bit = 0
    SSP1CON3bits.SDAHT = 1; // Minimum of 300 ns hold time on SDA after the falling edge of SCL
    SSP1CON3bits.SBCDE = 1; // Enable I2C slave bus collision detect interrupts
    SSP1ADD = (char) (I2C_slave_address << 1); // Load the slave address for I2C
    PIR1bits.SSP1IF = 0; // Clear the serial port interrupt flag
    PIR2bits.BCL1IF = 0; // Clear the bus collision interrupt flag
    PIE2bits.BCL1IE = 1; // Enable bus collision interrupts
    PIE1bits.SSP1IE = 1; // Enable serial port interrupts
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    OPTION_REGbits.INTEDG = 0; //Interrupt on falling edge for CNC
    IOCBN = 0b11001100; //configure interrupt on falling edge for slot buttons
    INTCON = 0b01011000; //enables interrupts
    T1CON = 0b00110100; //configure timer 1 to run at 1 MHz, interrupt time is 65.536 ms
    PIE1bits.TMR1IE = 1; //enable timer 1 interrupt
    T1CONbits.TMR1ON = 1; //start timer 1
    INTCONbits.GIE = 1; //run interrupts

    while (1) {
        asm("CLRWDT");
        if (I2C_buffer.data.RESET == 1) { //Reset the device
            asm("RESET");
        }

        if (I2C_buffer.data.SAVE == 1) { //Save non-volatile data into the EEPROM
            //eeprom_write(0, I2C_buffer.data.ADDRESS);
            for (unsigned char i = 0; i < 58; i++) //data for slot1 (56 bytes) (product_id, name, unit, time, counter, cost, quantity, stock, capacity, emptyLevel, fullLevel)
            {
                eeprom_write(i, I2C_buffer.byte[(unsigned char) (0x14 + i)]);
            }
            for (unsigned char i = 0; i < 58; i++) //data for slot2 (product_id, name, unit, time, counter, cost, quantity, stock, capacity, emptyLevel, fullLevel)
            {
                eeprom_write((unsigned char) (58 + i), I2C_buffer.byte[(unsigned char) (0x4E + i)]);
            }
            for (unsigned char i = 0; i < 58; i++) //data for slot3 (product_id, name, unit, time, counter, cost, quantity, stock, capacity, emptyLevel, fullLevel)
            {
                eeprom_write((unsigned char) (115 + i), I2C_buffer.byte[(unsigned char) (0x88 + i)]);
            }
            for (unsigned char i = 0; i < 58; i++) //data for slot4 (product_id, name, unit, time, counter, cost, quantity, stock, capacity, emptyLevel, fullLevel)
            {
                eeprom_write((unsigned char) (172 + i), I2C_buffer.byte[(unsigned char) (0xC2 + i)]);
            }
            I2C_buffer.data.SAVE = 0;
            __delay_ms(10);
        }

        if (I2C_buffer.data.dispense == 1) {
            I2C_buffer.data.time = 0;
            I2C_buffer.data.counter = 0;
            I2C_buffer.data.cancel = 0;
            I2C_buffer.data.selected1 = 0;
            I2C_buffer.data.selected2 = 0;
            I2C_buffer.data.selected3 = 0;
            I2C_buffer.data.selected4 = 0;
            switch (I2C_buffer.data.selected) {
                case 1:
                    while (I2C_buffer.data.time < I2C_buffer.data.time1 && I2C_buffer.data.counter < I2C_buffer.data.counter1 && I2C_buffer.data.dispense == 1) {
                        LATAbits.LATA4 = 1;
                        I2C_buffer.data.time++;
                        __delay_ms(1);
                        asm("CLRWDT");
                        if (I2C_buffer.data.calibrate == 1 && I2C_buffer.data.selected1 == 1) break;
                    }
                    LATAbits.LATA4 = 0;
                    I2C_buffer.data.dispense = 0;
                    break;
                case 2:
                    while (I2C_buffer.data.time < I2C_buffer.data.time2 && I2C_buffer.data.counter < I2C_buffer.data.counter2 && I2C_buffer.data.dispense == 1) {
                        LATAbits.LATA6 = 1;
                        I2C_buffer.data.time++;
                        __delay_ms(1);
                        asm("CLRWDT");
                        if (I2C_buffer.data.calibrate == 1 && I2C_buffer.data.selected2 == 1) break;
                    }
                    LATAbits.LATA6 = 0;
                    I2C_buffer.data.dispense = 0;
                    break;
                case 3:
                    while (I2C_buffer.data.time < I2C_buffer.data.time3 && I2C_buffer.data.counter < I2C_buffer.data.counter3 && I2C_buffer.data.dispense == 1) {
                        LATAbits.LATA7 = 1;
                        I2C_buffer.data.time++;
                        __delay_ms(1);
                        asm("CLRWDT");
                        if (I2C_buffer.data.calibrate == 1 && I2C_buffer.data.selected3 == 1) break;
                    }
                    LATAbits.LATA7 = 0;
                    I2C_buffer.data.dispense = 0;
                    break;
                case 4:
                    while (I2C_buffer.data.time < I2C_buffer.data.time4 && I2C_buffer.data.counter < I2C_buffer.data.counter4 && I2C_buffer.data.dispense == 1) {
                        LATBbits.LATB5 = 1;
                        I2C_buffer.data.time++;
                        __delay_ms(1);
                        asm("CLRWDT");
                        if (I2C_buffer.data.calibrate == 1 && I2C_buffer.data.selected4 == 1) break;
                    }
                    LATBbits.LATB5 = 0;
                    I2C_buffer.data.dispense = 0;
                    break;
                default:
                    I2C_buffer.data.dispense = 0;
                    break;
            }
            I2C_buffer.data.selected = 0;
        }

        if (I2C_buffer.data.relay1 == 1) {
            LATAbits.LATA4 = 1;
        } else {
            LATAbits.LATA4 = 0;
        }

        if (I2C_buffer.data.relay2 == 1) {
            LATAbits.LATA6 = 1;
        } else {
            LATAbits.LATA6 = 0;
        }

        if (I2C_buffer.data.relay3 == 1) {
            LATAbits.LATA7 = 1;
        } else {
            LATAbits.LATA7 = 0;
        }

        if (I2C_buffer.data.relay4 == 1) {
            LATBbits.LATB5 = 1;
        } else {
            LATBbits.LATB5 = 0;
        }

        if (readADC == 1) {
            if (average < 10) {
                stock1 += ADC_read(0x00);
                stock2 += ADC_read(0x01);
                stock3 += ADC_read(0x02);
                stock4 += ADC_read(0x03);
                average++;
            } else {
                if (I2C_buffer.data.calibrate == 1) {
                    I2C_buffer.data.stock1 = stock1 / 10;
                    I2C_buffer.data.stock2 = stock2 / 10;
                    I2C_buffer.data.stock3 = stock3 / 10;
                    I2C_buffer.data.stock4 = stock4 / 10;
                } else {
                    I2C_buffer.data.stock1 = map((stock1 / 10), I2C_buffer.data.emptyLevel1, I2C_buffer.data.fullLevel1, 0, I2C_buffer.data.capacity1);
                    I2C_buffer.data.stock2 = map((stock2 / 10), I2C_buffer.data.emptyLevel2, I2C_buffer.data.fullLevel2, 0, I2C_buffer.data.capacity2);
                    I2C_buffer.data.stock3 = map((stock3 / 10), I2C_buffer.data.emptyLevel3, I2C_buffer.data.fullLevel3, 0, I2C_buffer.data.capacity3);
                    I2C_buffer.data.stock4 = map((stock4 / 10), I2C_buffer.data.emptyLevel4, I2C_buffer.data.fullLevel4, 0, I2C_buffer.data.capacity4);
                }
                stock1 = 0;
                stock2 = 0;
                stock3 = 0;
                stock4 = 0;
                average = 0;
            }
        }
    }
    return (EXIT_SUCCESS);
}


