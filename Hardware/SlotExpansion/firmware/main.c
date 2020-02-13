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
#define I2C_slave_address 0x3F // any value from 0 to 127, this will be the default

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

//EEPROM default values and function prototypes for memory access on program

__EEPROM_DATA(I2C_slave_address, 0x00, 0x01, 0xB8, 0x0B, 0xFF, 0xFF, 0xFF); //set default program values stored on EEPROM at programming
unsigned char eeprom_read(unsigned char address);
void eeprom_write(unsigned char address, unsigned char value);

// Global variables declation

volatile union _I2C_buffer {

    struct _data {
        unsigned char ID;
        unsigned char ADDRESS;
        unsigned char SAVE;
        unsigned char RESET;
        unsigned char ENABLE;
        unsigned char BUZZER;
        unsigned int OBS_TIME;
        unsigned char OBSTRUCT;
        unsigned long UPA;
        unsigned long DWA;
        signed long NWA;
        unsigned long UPK;
        unsigned long DWK;
        signed long NWK;
    } data;
    unsigned char byte[];
} I2C_buffer; //this allows to access vars directly as set types or bytes. Shared with main program and I2C communication

const unsigned char RX_ELMNTS = sizeof (I2C_buffer); //used in I2C data transfer routines
volatile unsigned char first_i2c = 1;
volatile unsigned char index_i2c = 0;
volatile unsigned char blockStop = 1;
volatile unsigned int obstructHold = 0;
volatile unsigned char triggerUP = 0;
volatile unsigned char triggerDW = 0;
volatile unsigned char ignoreCount = 1;
volatile unsigned char middleSen = 0;
volatile unsigned char topSen = 0;

// Interrupts

void __interrupt isr() {

    if (INTCONbits.IOCIF == 1) {//interrupt on change
        INTCONbits.IOCIE = 0; //disable on change interrupts
        if (I2C_buffer.data.ENABLE == 1 && ignoreCount == 0) {
            if (IOCBFbits.IOCBF0 == 1 && IOCBFbits.IOCBF7 == 0 && triggerDW == 0) {//if up botton sensor triggers
                triggerUP = 1; //sets a flag if someone is getting up for later validation in main
                IOCBFbits.IOCBF0 = 0; //clear interrupt flag
            }
            if (IOCBFbits.IOCBF7 == 1 && IOCBFbits.IOCBF0 == 0 && triggerUP == 0) {//if down botton sensor triggers
                triggerDW = 1; //sets a flag if someone is getting down for later validation in main
                IOCBFbits.IOCBF7 = 0; //clear interrupt flag
            }
            if ((IOCBFbits.IOCBF2 == 1 || IOCBFbits.IOCBF6 == 1) && (triggerUP == 1 || triggerDW == 1)) {//if middle sensors triggers after up/down signal
                middleSen = 1; //sets a flag if someone is passing for later validation in main
                IOCBFbits.IOCBF2 = 0; //clear interrupt flag
                IOCBFbits.IOCBF6 = 0; //clear interrupt flag
            }
            if ((IOCBFbits.IOCBF3 == 1 || IOCBFbits.IOCBF5 == 1) && (triggerUP == 1 || triggerDW == 1)) {//if top sensors triggers after up/down signal
                topSen = 1; //sets a flag if someone is passing for later validation in main
                IOCBFbits.IOCBF3 = 0; //clear interrupt flag
                IOCBFbits.IOCBF5 = 0; //clear interrupt flag
            }
            IOCBF = 0; //clear interrupt on change flags
            INTCONbits.IOCIE = 1; //enable on change interrupts
        } else {
            IOCBF = 0; //clear interrupt on change flags
            INTCONbits.IOCIE = 1; //enable on change interrupts
        }
    }

    if (PIR1bits.TMR1IF == 1) //timer1 interrupt, called every 65.536 ms
    {
        INTCONbits.IOCIE = 0; //disable on change interrupts
        T1CONbits.TMR1ON = 0; //stop timer1
        asm("CLRWDT"); //reset the watchdog timer to avoid unwanted resets if system is working correctly
        if (I2C_buffer.data.ENABLE == 1) {
            blockStop = 0;
            if (PORTBbits.RB0 == 1 || PORTBbits.RB2 == 1 || PORTBbits.RB3 == 1 || PORTBbits.RB5 == 1 || PORTBbits.RB6 == 1 || PORTBbits.RB7 == 1) {//detect obstruction
                if (I2C_buffer.data.OBSTRUCT != 1) {
                    obstructHold++; //acumulates consecutive obstruction time
                }
                if ((obstructHold * 66) >= I2C_buffer.data.OBS_TIME) { //wait until the minimun configured time is reached
                    I2C_buffer.data.OBSTRUCT = 1; //counter is obstructed more than the allowed time
                } else {
                    I2C_buffer.data.OBSTRUCT = 0;
                }
            } else {
                I2C_buffer.data.OBSTRUCT = 0;
                obstructHold = 0; //clears the acumulator and resets obstruction alert
            }
            if (I2C_buffer.data.OBSTRUCT == 1 && I2C_buffer.data.BUZZER == 1) { //beep if obstruction occurs and buzzer is enabled
                LATAbits.LATA6 = 1;
            } else {
                LATAbits.LATA6 = 0;
            }
        } else {
            blockStop = 1;
            I2C_buffer.data.OBSTRUCT = 0;
            LATAbits.LATA6 = 0;
        }
        INTCONbits.IOCIE = 1; //enable on change interrupts
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

// Functions

void init_I2C_buffer() { //load default values of vars
    I2C_buffer.data.ID = DEVICE_ID;
    I2C_buffer.data.ADDRESS = eeprom_read(0);
    I2C_buffer.data.SAVE = 0;
    I2C_buffer.data.RESET = 0;
    I2C_buffer.data.ENABLE = eeprom_read(1);
    I2C_buffer.data.BUZZER = eeprom_read(2);
    I2C_buffer.byte[0x06] = eeprom_read(3); //OBS_TIME
    I2C_buffer.byte[0x07] = eeprom_read(4);
    I2C_buffer.data.OBSTRUCT = 0;
    I2C_buffer.data.UPA = 0;
    I2C_buffer.data.DWA = 0;
    I2C_buffer.data.NWA = 0;
    I2C_buffer.data.UPA = 0;
    I2C_buffer.data.DWA = 0;
    I2C_buffer.data.NWA = 0;
}

PWM_Init(void) // start the PWM with a 0% duty
{
    //10 bit mode PWM at 38kHz, CCP2
    PR2 = 211; //load PR2 register, this value gives us 38KHz frequency
    CCP2CON = 0b00001100; //configure CCP2 for PWM operation
    CCPR2L = 0b00000000; //set duty to 0    
    PIR1bits.TMR2IF = 0; //clear the interrupt flag
    T2CON = 0b00000100; //start timer 2 and select prescaler as 1
}

PWM_set_duty(int duty) // change the duty of PWM in CCP2
{
    if (duty < 1024) {
        CCPR2L = (0xFF & ((unsigned int) duty >> 2)); //discard 2 LSB and write the rest to register, 10 bit mode
        CCP2CON = (0x0C | (0x30 & ((unsigned int) duty << 4))); //filter the reamaining 2 LSB and then write it to DC1B (bit 5,4 in the register), 10 bit mode
        //CCPR1L = (0xFF & ((unsigned int)duty)); //to be used in 8 bit mode
    }
}

void main(void) {
    OSCCON = 0b11110000; //configure internal oscilator for 32Mhz
    init_I2C_buffer(); //load default values and configurations
    OPTION_REGbits.nWPUEN = 0; //allow weak pull-ups to activate
    TRISA = 0b00100000; //configure IO
    ANSELA = 0b00000000; //analog functions of pins disabled
    TRISB = 0b11111111; //configure IO
    ANSELB = 0b00000000; //analog functions of pins disabled
    WPUA = 0b00100000; //configure weak pull-ups on input pins
    WPUB = 0b11111111; //configure weak pull-ups on input pins
    PORTA = 0b00000000; //set all to LOW
    PORTB = 0b00000000; //set all to LOW
    APFCON0bits.CCP2SEL = 1; //select PWM output on RA7
    SSP1STAT = 0b10000000; // Slew rate control disabled for standardspeed I2C mode (100 kHz and 1 MHz)
    SSP1CON1 = 0b00110110; // Enable serial port, I2C slave mode, 7-bit address
    SSP1CON2bits.SEN = 1; // Clock stretching on I2C is enabled
    SSP1CON3bits.BOEN = 1; // I2C SSPBUF is updated and NACK is generated for a received address/data byte, ignoring the state of the SSPOV bit only if the BF bit = 0
    SSP1CON3bits.SDAHT = 1; // Minimum of 300 ns hold time on SDA after the falling edge of SCL
    SSP1CON3bits.SBCDE = 1; // Enable I2C slave bus collision detect interrupts
    SSP1ADD = (char) (I2C_buffer.data.ADDRESS << 1); // Load the slave address for I2C
    PIR1bits.SSP1IF = 0; // Clear the serial port interrupt flag
    PIR2bits.BCL1IF = 0; // Clear the bus collision interrupt flag
    PIE2bits.BCL1IE = 1; // Enable bus collision interrupts
    PIE1bits.SSP1IE = 1; // Enable serial port interrupts
    INTCONbits.PEIE = 1; // Enable peripheral interrupts
    PWM_Init(); //start pwm
    PWM_set_duty(0); //put duty of pwm to 0
    IOCBP = 0b11101101; //configure interrupt on rising edge for sensors
    INTCON = 0b01001000; //enables interrupts
    T1CON = 0b00110100; //configure timer 1 to run at 1 MHz, interrupt time is 65.536 ms
    PIE1bits.TMR1IE = 1; //enable timer 1 interrupt
    T1CONbits.TMR1ON = 1; //start timer 1
    __delay_ms(50);
    LATAbits.LATA6 = 1; //make sound at start
    __delay_ms(450);
    LATAbits.LATA6 = 0;
    INTCONbits.GIE = 1; //run interrupts

    while (1) {

        if (I2C_buffer.data.ENABLE == 0 && blockStop == 1) {
            ignoreCount = 1;
            __delay_ms(2); //give some time to the MCU to apply changes to hardware
            PWM_set_duty(0); //put duty of pwm to 0
        } else {
            PWM_set_duty(512); //put duty of pwm to 512
            __delay_ms(2);
            ignoreCount = 0;
        }

        if (I2C_buffer.data.RESET == 1) { //Reset the device
            asm("RESET");
        }

        if (I2C_buffer.data.SAVE == 1) { //Save non-volatile data into the EEPROM
            eeprom_write(0, I2C_buffer.data.ADDRESS);
            eeprom_write(1, I2C_buffer.data.ENABLE);
            eeprom_write(2, I2C_buffer.data.BUZZER);
            eeprom_write(3, I2C_buffer.byte[0x06]);
            eeprom_write(4, I2C_buffer.byte[0x07]);
            __delay_ms(10);
            I2C_buffer.data.SAVE = 0;
        }

        if (triggerUP == 1) { //validates it is was a person(adult or kid) and wait until it pass the barries before re-enabling detection, if not discards the event and re-enables detection
            __delay_ms(500);
            while (PORTBbits.RB0 == 1 || PORTBbits.RB2 == 1 || PORTBbits.RB3 == 1 || PORTBbits.RB5 == 1 || PORTBbits.RB6 == 1 || PORTBbits.RB7 == 1) {
                __delay_ms(10);
            }
            __delay_ms(100);
            if (middleSen == 1 && topSen == 1) {
                I2C_buffer.data.UPA++;
                I2C_buffer.data.NWA = I2C_buffer.data.UPA - I2C_buffer.data.DWA;
            } else {
                if (middleSen == 1 && topSen == 0) {
                    I2C_buffer.data.UPK++;
                    I2C_buffer.data.NWK = I2C_buffer.data.UPK - I2C_buffer.data.DWK;
                }
            }
            while (PORTBbits.RB0 == 1 || PORTBbits.RB2 == 1 || PORTBbits.RB3 == 1 || PORTBbits.RB5 == 1 || PORTBbits.RB6 == 1 || PORTBbits.RB7 == 1) {
                __delay_ms(10);
            }
            triggerUP = 0;
            middleSen = 0;
            topSen = 0;
            IOCBF = 0; //clear interrupt on change flags
        }

        if (triggerDW == 1) { //validates it is was a person(adult or kid) and wait until it pass the barries before re-enabling detection, if not discards the event and re-enables detection
            __delay_ms(500);
            while (PORTBbits.RB0 == 1 || PORTBbits.RB2 == 1 || PORTBbits.RB3 == 1 || PORTBbits.RB5 == 1 || PORTBbits.RB6 == 1 || PORTBbits.RB7 == 1) {
                __delay_ms(10);
            }
            __delay_ms(100);
            if (middleSen == 1 && topSen == 1) {
                I2C_buffer.data.DWA++;
                I2C_buffer.data.NWA = I2C_buffer.data.UPA - I2C_buffer.data.DWA;
            } else {
                if (middleSen == 1 && topSen == 0) {
                    I2C_buffer.data.DWK++;
                    I2C_buffer.data.NWK = I2C_buffer.data.UPK - I2C_buffer.data.DWK;
                }
            }
            while (PORTBbits.RB0 == 1 || PORTBbits.RB2 == 1 || PORTBbits.RB3 == 1 || PORTBbits.RB5 == 1 || PORTBbits.RB6 == 1 || PORTBbits.RB7 == 1) {
                __delay_ms(10);
            }
            triggerDW = 0;
            middleSen = 0;
            topSen = 0;
            IOCBF = 0; //clear interrupt on change flags
        }

    }
    return;
}


