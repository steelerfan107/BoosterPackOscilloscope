////////////////////////////////////////////////////
//  Eric Taylor
//  Rose-Hulman - ECE 462 Senior Design
//  TI BoosterPack Oscilliscope Software
//
//  This program uses an EPI interface to read in 20 bit samples (First 10 bits - Channel 1, Second 10 bits - Channel 2)
//  and uses a timer interupt to have a constant time between samples. These samples are then shown on a LCD screen using
//  a SPI controller. 
//
//  The following code was mide by me with exception of the SPI driver for the LCD included in "Screen_ILI9340_Library.h"
//
//  TODO: Setup uDMA so sample can be read in faster allowing faster signals to be measured.


/////////////////////////////////////////
//  Header Files
////////////////////////////////////////

#include "Energia.h"


// Include application, user and local libraries
#include "SPI.h"
#include "driverlib/epi.h"
#include "driverlib/udma.h"
#include "driverlib/gpio.h"
// Screen selection
#define ILI9340 // HX8353E K35 HI32 W32 ILI9225B HY28A ST7735 PicasoSPE PicasoSGC

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "inc/hw_ints.h"
#include "driverlib/interrupt.h"

#if defined(ILI9340)
#include "Screen_ILI9340_Library.h"
Screen_ILI9340 myScreen;
#endif

////////////////////////////////////////////////////
//  Global Variable Declarations
////////////////////////////////////////////////////

uint8_t largeOrientation = 0;
uint32_t data_location = 0;
int flag = 0, test = 0;
unsigned long * data_addr = (unsigned long *)0x60000000;
volatile int temp = 1;
volatile int tempB = 1;

signed long buffer_a[320],old_buffer_a[320];
signed long buffer_b[320],old_buffer_b[320];
unsigned long sample;
unsigned long int_sample;

/*
void * PP_BUFFER_A = 0;
void * PP_BUFFER_B = 0; 

///////////////////////////////////////////////////
// DMA Setup (Not Used at the Moment)
//////////////////////////////////////////////////

tDMAControlTable g_dmaCtrl[64] __attribute__ ((aligned(1024)));
void setupDMA(){
    PP_BUFFER_A = (void *)malloc(32*1024);
    PP_BUFFER_B = (void *)malloc(32*1024);
    
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
    uDMAEnable();
    uDMAControlBaseSet(g_dmaCtrl);
    
    uDMAChannelAssign(UDMA_CH20_EPI0RX);
    
    uDMAChannelSelectSecondary(UDMA_DEF_TMR1A_SEC_EPI0RX);
    
    uDMAChannelAttributeEnable(UDMA_SEC_CHANNEL_EPI0RX, UDMA_ATTR_HIGH_PRIORITY);
    uDMAChannelAttributeDisable(UDMA_SEC_CHANNEL_EPI0RX, UDMA_ATTR_USEBURST | UDMA_ATTR_REQMASK | UDMA_ATTR_ALTSELECT);
    
    uDMAChannelControlSet(UDMA_CH20_EPI0RX | UDMA_PRI_SELECT,UDMA_SIZE_32 | UDMA_SRC_INC_NONE | UDMA_DST_INC_32 |UDMA_ARB_8);
    uDMAChannelTransferSet(UDMA_CH20_EPI0RX | UDMA_PRI_SELECT,UDMA_MODE_PINGPONG,data_addr,PP_BUFFER_A,1023);
    
    uDMAChannelControlSet(UDMA_CH20_EPI0RX | UDMA_ALT_SELECT,UDMA_SIZE_32 | UDMA_SRC_INC_NONE | UDMA_DST_INC_32 |UDMA_ARB_8);
    uDMAChannelTransferSet(UDMA_CH20_EPI0RX | UDMA_ALT_SELECT,UDMA_MODE_PINGPONG,data_addr,PP_BUFFER_B,1024);
    
    //uDMAIntRegister(UDMA_INT_SW, DMA_HandleInterrupt);
    
    uDMAChannelEnable(UDMA_CH20_EPI0RX);  
    //uDMAChannelRequest(UDMA_SEC_CHANNEL_EPI0TX);
}
*/

///////////////////////////////////////////////////
// Setup Gain
// The pins in order control - D0, D1, D2, D3, D4, D5, D6 of the PGA. This sets up these pins to be output enabled.
///////////////////////////////////////////////////
void setupGain(){
    pinMode(51, OUTPUT); 
    pinMode(53, OUTPUT); 
    pinMode(58, OUTPUT); 
    pinMode(74, OUTPUT); 
    pinMode(73, OUTPUT); 
    pinMode(72, OUTPUT); 
    pinMode(71, OUTPUT); 
}

//////////////////////////////////////////////////
// Setup the External Periphrial Intrerface (EPI Interface Reads in Data From the ADC)
/////////////////////////////////////////////////
void setupEPI()
{
  // Define what pins in each of the ports are used for the EPI
  #define EPI_PORTA_PINS (GPIO_PIN_6 | GPIO_PIN_7)
  #define EPI_PORTB_PINS (GPIO_PIN_3 | GPIO_PIN_2)
  #define EPI_PORTC_PINS (GPIO_PIN_7 | GPIO_PIN_6 | GPIO_PIN_5 | GPIO_PIN_4 )
  #define EPI_PORTG_PINS (GPIO_PIN_1 | GPIO_PIN_0 )
  #define EPI_PORTK_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5)
  #define EPI_PORTL_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5)
  #define EPI_PORTM_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
  #define EPI_PORTN_PINS (GPIO_PIN_2 | GPIO_PIN_4 | GPIO_PIN_5)
  #define EPI_PORTQ_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
  
  // Configuration options for the EPI
   unsigned long ulEPIConfig = EPI_GPMODE_CLKPIN   
       | EPI_GPMODE_ASIZE_4   // address bus size of 4
       | EPI_GPMODE_DSIZE_24   //
       | EPI_GPMODE_READ2CYCLE  // The RD2CYC bit in the EPIGPCFG register must be set at all times in General-Purpose
       | EPI_GPMODE_FRAMEPIN;  // frame signal    
  
  
   // Enable EPI0
   SysCtlPeripheralEnable(SYSCTL_PERIPH_EPI0);
   
   // SET GPIO to connect to Hardware
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
   SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
   
   //Configure Pins to drive 8mA
   GPIOPinConfigure(GPIO_PK0_EPI0S0);
   GPIOPinConfigure(GPIO_PK1_EPI0S1);
   GPIOPinConfigure(GPIO_PK2_EPI0S2);
   GPIOPinConfigure(GPIO_PK3_EPI0S3);
   GPIOPinConfigure(GPIO_PC7_EPI0S4);
   GPIOPinConfigure(GPIO_PC6_EPI0S5);
   GPIOPinConfigure(GPIO_PC5_EPI0S6);
   GPIOPinConfigure(GPIO_PC4_EPI0S7);
   GPIOPinConfigure(GPIO_PA6_EPI0S8);
   GPIOPinConfigure(GPIO_PA7_EPI0S9);
   GPIOPinConfigure(GPIO_PG1_EPI0S10);
   GPIOPinConfigure(GPIO_PG0_EPI0S11);
   GPIOPinConfigure(GPIO_PM3_EPI0S12);
   GPIOPinConfigure(GPIO_PM2_EPI0S13);
   GPIOPinConfigure(GPIO_PM1_EPI0S14);
   GPIOPinConfigure(GPIO_PM0_EPI0S15);
   GPIOPinConfigure(GPIO_PL0_EPI0S16);
   GPIOPinConfigure(GPIO_PL1_EPI0S17);
   GPIOPinConfigure(GPIO_PL2_EPI0S18);
   GPIOPinConfigure(GPIO_PL3_EPI0S19);
   GPIOPinConfigure(GPIO_PQ0_EPI0S20);
   GPIOPinConfigure(GPIO_PQ1_EPI0S21);
   GPIOPinConfigure(GPIO_PQ2_EPI0S22);
   GPIOPinConfigure(GPIO_PQ3_EPI0S23);
   // Commented out lines represent GPIO used elsewhere
   //GPIOPinConfigure(GPIO_PK7_EPI0S24);
   //GPIOPinConfigure(GPIO_PK6_EPI0S25);
   GPIOPinConfigure(GPIO_PL4_EPI0S26);
   GPIOPinConfigure(GPIO_PB2_EPI0S27);
   GPIOPinConfigure(GPIO_PB3_EPI0S28);
   GPIOPinConfigure(GPIO_PN2_EPI0S29);
   //GPIOPinConfigure(GPIO_PN3_EPI0S30);
   GPIOPinConfigure(GPIO_PK5_EPI0S31);
   GPIOPinConfigure(GPIO_PK4_EPI0S32);
   GPIOPinConfigure(GPIO_PL5_EPI0S33);
   GPIOPinConfigure(GPIO_PN4_EPI0S34);
   GPIOPinConfigure(GPIO_PN5_EPI0S35);
   
   // Set GPIO to EPI
   GPIOPinTypeEPI(GPIO_PORTA_BASE, EPI_PORTA_PINS);
   GPIOPinTypeEPI(GPIO_PORTB_BASE, EPI_PORTB_PINS);
   GPIOPinTypeEPI(GPIO_PORTC_BASE, EPI_PORTC_PINS);
   GPIOPinTypeEPI(GPIO_PORTG_BASE, EPI_PORTG_PINS);
   GPIOPinTypeEPI(GPIO_PORTK_BASE, EPI_PORTK_PINS);
   GPIOPinTypeEPI(GPIO_PORTL_BASE, EPI_PORTL_PINS);
   GPIOPinTypeEPI(GPIO_PORTM_BASE, EPI_PORTM_PINS);
   GPIOPinTypeEPI(GPIO_PORTN_BASE, EPI_PORTN_PINS);
   GPIOPinTypeEPI(GPIO_PORTQ_BASE, EPI_PORTQ_PINS);
  
  //
  // Set the EPI divider.
  //
  EPIDividerSet(EPI0_BASE, 1);
  
  //
  // Select GPIO
  //
  EPIModeSet(EPI0_BASE, EPI_MODE_GENERAL);
  
  //
  // Activate External CLK on EPI0s31, Set data to 24 bits EPI0s0 to EPI0S23
  //
  EPIConfigGPModeSet(EPI0_BASE,ulEPIConfig,0,0);

  //
  // Set the address map.
  //
  EPIAddressMapSet(EPI0_BASE, EPI_ADDR_RAM_SIZE_256MB | EPI_ADDR_RAM_BASE_6);
  
  
  //
  // Start the EPI Interface
  //
  EPINonBlockingReadConfigure(EPI0_BASE, 0, EPI_NBCONFIG_SIZE_32, 0);
  EPINonBlockingReadStart(EPI0_BASE,0,1);
}



/////////////////////////////////////////////////
//  Draw the GUI on the Screen
//////////////////////////////////////////////

void GUI()
{
    myScreen.setOrientation(1);
    myScreen.setFontSize(0);
    myScreen.gText(4, 4, "V/D " + String(1) + ", Time/D: " + String(1)+ ",R/F: " + String(1) , colour_white);

    for(int x = 1; x < 320; x++){
        if(x%20 == 0){
            if(x == 20*8){
                //Long Vertical Axis
                for(int i = -120; i < 120; i++){
                myScreen.point(x, 120 + i, colour_white);
                //myScreen.point(x, 240 + i, colour_white);
                
                if(i%20 == 0 && i != -120){
                // Lines on Vertical Axis
                  for(int y = -6; y < 6; y++){
                    myScreen.point(x+y, 120 + i, colour_white);
                    //myScreen.point(x+y, 240 + i, colour_white);
                  }
                }
                
              }
            }else{
               // Ticks on Horizontal Axis
              for(int i = -5; i < 5; i++){
                myScreen.point(x, 120 + i, colour_white);
                //myScreen.point(x, 240 + i, colour_white);
              }
            }
        }
        myScreen.point(x, 120, colour_white);
        //myScreen.point(x, 240, colour_white);
        
    }
}

//////////////////////////////////////////
//  GUI UPdate
//   - Updates the GUI based on the input
////////////////////////////////////////
void GUI_update(uint8_t v_div,uint8_t t_div, uint8_t risefall )
{
    myScreen.setFontSize(0);
    myScreen.gText(4, 4, "V/D " + String(v_div) + ", Time/D: " + String(t_div)+ ",R/F: " + String(risefall) , colour_white);
    delay(100);
}

///////////////////////////////////////////////////
// sSets Gain
// The pins in order control - D0, D1, D2, D3, D4, D5, D6 of the PGA. This sets up these pins to the inputed state.
///////////////////////////////////////////////////
void setGainData(int d6, int d5, int d4, int d3, int d2, int d1, int d0){
      if(d0 == 1){
          digitalWrite(51, HIGH);
      }else{
          digitalWrite(51, LOW);
      }
      
      if(d1 == 1){
          digitalWrite(53, HIGH);
      }else{
          digitalWrite(53, LOW);
      }
      
      if(d2 == 1){
          digitalWrite(58, HIGH);
      }else{
          digitalWrite(58, LOW);
      }
      
      if(d3 == 1){
          digitalWrite(74, HIGH);
      }else{
          digitalWrite(74, LOW);
      }
      
      if(d4 == 1){
          digitalWrite(73, HIGH);
      }else{
          digitalWrite(73, LOW);
      }
      
      if(d5 == 1){
          digitalWrite(72, HIGH);
      }else{
          digitalWrite(72, LOW);
      }
      
      if(d6 == 1){
          digitalWrite(71, HIGH);
      }else{
          digitalWrite(71, LOW);
      }
}


uint16_t size = myScreen.screenSizeX();
int i = 0,x = 0;
uint32_t data = 0;
uint32_t y = 0,z = 0,fill = 0;

unsigned long int timer_value;
volatile int buffer_count = 0;
int triggered = 0;
int temp_test = 0;
double gain = 4;
double old_gain = 4;
int    wait = 100;
signed int ya,yb,ya_o,yb_o;

const int TimePin = PUSH1;
const int VoltagePin = PUSH2;
const int TrigPin = 12;

int volState = 1;
int timeState = 1;


///////////////////////////////////////////////
// Initialization Code
/////////////////////////////////////////////

void setup()
{   
    // Setup PGA Control
    setupGain();
    setGainData(1,1,1,1,1,1,1);
    
    pinMode(VoltagePin, INPUT_PULLUP); 
    pinMode(TimePin, INPUT_PULLUP); 
    
    attachInterrupt(VoltagePin, voldiv, RISING);
    attachInterrupt(TimePin, timediv, RISING);
 
    
    pinMode(12, INPUT);
    
    setupEPI();
    // Start LCD Screen
    myScreen.begin();
    
    myScreen.setFontSize(myScreen.fontMax());
    //while(1){
    // Draw GUI
    GUI();
    //}
    
    // Fill the old and new buffers
    for(x = 0; x <= 319; x+=1){
        buffer_a[x] = 0;
        old_buffer_a[x] = 0;
        buffer_b[x] = 0;
        old_buffer_b[x] = 0;
    }   
    
    
    i = 0;
    buffer_count = 0;
    TimerSetup();
    attachInterrupt(TrigPin, trigger, RISING);
}


// Loop Code
void loop()
{   
    //myScreen.gText(40, 40, String(int_sample&0x0000001F));
    
    //
    // Software Trigger
    //   - Waits until a value is read on EPI, once it finds the correct value it starts the timer interrupt to sample the EPI
    
    //if(data_addr[0]  != 0 && triggered == 0){
    //  triggered = 1;
    //  TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
    //}
    
    //
    // If the Scope was triggered and the buffer of samples was filled
    //
   
    if(triggered == 1){
      interrupts();

      
      if( buffer_count == 320 ){   
         
        // Disable the Time Interrupt
        TimerIntDisable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
        noInterrupts();
        
        // DRAW SIGNAL
        for(x = 0; x <= 320; x+=1){
          ya = ((buffer_a[x]-512+115)/gain+120);
          yb = ((buffer_b[x]-512+115)/gain+120);
          
          if(buffer_a[x] != 10 && x % 20 != 0 && ya % 20 != 0){
            myScreen.point(x, ya, colour_red);
          }
          if(buffer_b[x] != 10 && x % 20 != 0 && yb % 20 != 0){
            myScreen.point(x, yb, colour_blue);
          }
        }
         // ERASE OLD SIGNAL
        for( x = 0; x < 320; x +=1){
          ya_o = ((old_buffer_a[x]-512+115)/old_gain+120);
          yb_o = ((old_buffer_b[x]-512+115)/old_gain+120);
        
          if(x % 20 != 0 && ya_o % 20 != 0 && old_buffer_a[x] != 10 && buffer_a[x] != old_buffer_a[x]){
            myScreen.point(x, ya_o, colour_black);
          }
          
          if(x % 20 != 0 && yb_o % 20 != 0 && old_buffer_b[x] != 10 && buffer_b[x] != old_buffer_b[x]){
            myScreen.point(x, yb_o, colour_black);
          }
        
          old_buffer_a[x] = buffer_a[x];
          old_buffer_b[x] = buffer_b[x];
        
        }
        
        //GUI();
        myScreen.gText(20, 20, "A: " + String(buffer_a[x]-100));
        myScreen.gText(70, 20, "B: " + String(buffer_b[x]-100));
        delay(10);
        buffer_count = 0;
        triggered = 0;
        old_gain = gain;
        // Start Intrrupts again
        interrupts();
        TimerLoadSet(TIMER1_BASE,TIMER_A, wait);
      }
      
    }
}

// When the voltage per deviation button is pressed this function changes the gain to make displayed signal larger or smaller
void voldiv()
{
    volState++;
    if(volState == 3){
      volState = 0;
    }
    
    if(volState == 0){
      gain = 8;
    }
    if(volState == 1){
      gain = 4;
    }
    if(volState == 2){
      gain = 2;
    }
    
    GUI_update(volState,timeState, 1);
  
}

// When the time per deviation button is pressed this function changes the time beween samples to change how much of a signal is measured
void timediv()
{
    timeState++;
    if(timeState == 3){
      timeState = 0;
    }
    
    if(timeState == 0){
      wait = 150;
    }
    if(timeState == 1){
      wait = 300;
    }
    if(timeState == 2){
      wait = 600;
    }
    GUI_update(volState,timeState, 1);
}

// When trigger is seen this function is called and starts collecting samples
void trigger()
{
      triggered = 1;
      interrupts();
      TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
}

//
// Timer Interrupt Initalization
//
void TimerSetup(){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
  TimerIntRegister(TIMER1_BASE, TIMER_A, Timer1IntHandler);
  TimerEnable(TIMER1_BASE, TIMER_A);
  TimerLoadSet(TIMER1_BASE,TIMER_A,150);
  IntEnable(INT_TIMER1A); 
}

//
// Timer Interrupt Handler
//
void Timer1IntHandler(void) {
        
	// Clear the timer interrupt
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

        // If the buffer is not filled and the scope is triggered
        if( buffer_count < 320 && triggered == 1){
          // Get sample from EPI
          sample = data_addr[0];
          // Store First Sample
          buffer_a[buffer_count] = sample&0x00000003FF;
          // Store Second Sample
          buffer_b[buffer_count] = (sample>>10)&0x000003FF;
          // Count Number of Samples
          buffer_count++;
        }
}

