// CPEG222 - Project 1 template
// Inputs:  One on-board button: Btn1
// Outputs: One PmodSSD
// Function Description:  The SSD will display 9 on the left digit initially.
//                        Pressing btn1 will switch the display to the other digit.

#include <p32xxxx.h>
#include <plib.h>
// Configuration Bit settings (Don't touch them if not necessary)
// SYSCLK = 80 MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 40 MHz
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
#define SYS_FREQ     80000000L

#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF//not changed
#pragma config POSCMOD = HS, FNOSC = PRIPLL,   FPBDIV = DIV_2// set PBCLK as 40MHZ




// The speed control
#define DELAY 100
// The refresh rate of the segments
#define REFRESH_RATE 2000   // ~1/100 secs

/****************************************************/
/* Port Definition                           */
/*    You should add your port definition if necessary*/
/****************************************************/

// Buttons
#define Btn1	PORTGbits.RG6

// 7 Segment Display pmod for using the bottom row JC & JD jumpers
// Segments
#define SegA 	PORTBbits.RB15
#define SegB	PORTDbits.RD5
#define SegC 	PORTDbits.RD4
#define SegD 	PORTBbits.RB14
#define SegE	PORTDbits.RD1
#define SegF	PORTDbits.RD2
#define SegG	PORTDbits.RD3
// Display selection. 0 = right, 1 = left (Cathode)
#define DispSel PORTDbits.RD12

/*    Definition of Modes    */
/*    Init/Left : Left digit display    */
/*   Right:      Right digit display   */
enum Mode {Left,Right};

//Subroutine, add yours for the project
void displayDigit(unsigned char, unsigned char); // Display one digit among two digits
void slowDownDisplay(unsigned char, unsigned char); // Frequency control function

// look-up table for the numbers
unsigned char number[]={
    0b0111111,   //0
    0b0000000,   //define 1
    0b0000000,   //define 2
    0b0000000,   //define 3
    0b0000000,   //define 4
    0b0000000,   //define 5
    0b0000000,   //define 6
    0b0000000,   //define 7
    0b0000000,   //define 8
    0b1101111,   //9
    0b0000000   //clear
};

//variable definition
unsigned int display_value =9; // The initial displayed value
int Buttonlock=0;
enum Mode mode=Left; //Initial mode is left

main(){
    //initialization
    TRISB = 0x0000;      // set PORTB as output
    TRISD = 0x0000;      // set PORTE as output
    TRISG = 0x40;      // set PORTG as output
    PORTD = 0;      // initialize PORTE to 0
    PORTB = 0;      // initialize PORTG to 0
    
    while(1){
        // output logic
        // display logic
        // this is a Moore machine, output display only depend on current mode
        //displayDigit(mode==Left, number[display_value]); //display digit
       slowDownDisplay(mode==Left, number[display_value]);
       slowDownDisplay(mode==Right, number[display_value]);
       
       //slowDownDisplay(mode==Right, number[display_value]);// debouncing & display digit
        
        // Next state logic
        if (Btn1&&!Buttonlock)
        {
            slowDownDisplay(mode==Left, number[display_value]);   // debouncing & display digit
            Buttonlock=1;
            
            if(mode==Left)
            {
                mode=Right;display_value++;
            }
            else
            {
                mode=Left;display_value--;
            }
        }
        else if(!Btn1 && Buttonlock)  //unlock
        {
           slowDownDisplay(mode==Left, number[display_value]); 
           slowDownDisplay(mode==Right, number[display_value]);// debouncing & display digit
       
            Buttonlock=0;
        }
    }
}

//display digit
void displayDigit (unsigned char display_sel, unsigned char value){
    DispSel = display_sel;
    SegA    = value & 1;
    SegB    = (value >> 1) & 1;
    SegC    = (value >> 2) & 1;
    SegD    = (value >> 3) & 1;
    SegE    = (value >> 4) & 1;
    SegF    = (value >> 5) & 1;
    SegG    = (value >> 6) & 1;
}

// debouncing keys & display
void slowDownDisplay(unsigned char display_sel, unsigned char value){
    unsigned int slow = 0;
    while(slow < REFRESH_RATE){
        displayDigit(display_sel,value);
        slow++;
    }
}
//#define SegA 	LATBbits.LATB15
//#define SegB	LATDbits.LATD5
//#define SegC 	LATDbits.LATD4
//#define SegD 	LATBbits.LATB14
//#define SegE	LATDbits.LATD1
//#define SegF	LATDbits.LATD2
//#define SegG	LATDbits.LATD3
// Display selection. 0 = right, 1 = left (Cathode)
//#define DispSel LATDbits.LATD12
