#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"

//#include "inc/hw_timer.h"
//#include "utils/uartstdio.h"

//*****************************************************************************
//
//! \addtogroup example_list
//! <h1>Timer (timers)</h1>
//!
//! This example application demonstrates the use of the timers to generate
//! periodic interrupts.  One timer is set up to interrupt once per second and
//! the other to interrupt twice per second; each interrupt handler will toggle
//! its own indicator on the display.
//!
//! UART0, connected to the Virtual Serial Port and running at 115,200, 8-N-1,
//! is used to display messages from this application.
//
//*****************************************************************************

//*****************************************************************************
//
// Flags that contain the current value of the interrupt indicator as displayed
// on the UART.
//
//*****************************************************************************
uint32_t g_ui32Flags;

//*****************************************************************************
//
// The error routine that is called if the driver library encounters an error.
//
//*****************************************************************************
#ifdef DEBUG
void
__error__(char *pcFilename, uint32_t ui32Line)
{
}
#endif

//*****************************************************************************
//
// The interrupt handler for the first timer interrupt.
//
//*****************************************************************************
void
Timer0_Handler(void)
{
    static uint16_t x = 0;
    //TimerIntClear(TIMER0_BASE, TIMER_BOTH);
    TimerIntClear(TIMER0_BASE, TIMER_A );
//    GPIOPinWrite(GPIO_PORTF_BASE ,  GPIO_PIN_1 , GPIO_PIN_1 );
//     GPIOPinWrite(GPIO_PORTF_BASE ,  GPIO_PIN_2 , 1<<2 );
//     GPIOPinWrite(GPIO_PORTF_BASE ,  GPIO_PIN_3 , 1<<3 );
    switch(x)
    {

    case 0:
        GPIOPinWrite(GPIO_PORTF_BASE ,  GPIO_PIN_1 , 0 );
//        GPIOPinWrite(GPIO_PORTF_BASE ,  GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 ,  GPIO_PIN_1);
        break;
    case 1:
        GPIOPinWrite(GPIO_PORTF_BASE ,  GPIO_PIN_1 , GPIO_PIN_1 );
//        GPIOPinWrite(GPIO_PORTF_BASE ,  GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 ,  GPIO_PIN_2);
        break;
    case 2:
        GPIOPinWrite(GPIO_PORTF_BASE ,  GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 ,  GPIO_PIN_3);
       break;
    }
    x++;
    if(x == 2){x = 0;}
}



int
main(void)
{
   unsigned long ulPeriod = 0;
//    //
//    // Enable lazy stacking for interrupt handlers.  This allows floating-point
//    // instructions to be used within interrupt handlers, but at the expense of
//    // extra stack usage.
//    //
//    ROM_FPULazyStackingEnable();

    //
    // Set the clocking to run directly from the crystal.
    //
    // Set Clock to 40MHz
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN  );


    //
    // Enable the GPIO port that is used for the on-board LED.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //
    // Enable the GPIO pins for the LED (PF1 & PF2).
    //
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3 | GPIO_PIN_2 | GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_5);
    //
    // Enable the peripherals used by this example.
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    //// Wait for the Timer0 module to be ready.
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER0));

    //
    // Configure the two 32-bit periodic timers.
    //
    TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
    //
    // Set CallBack for Timer0 Interrupt
    //
    TimerIntRegister(TIMER0_BASE , TIMER_A , Timer0_Handler);
    //
    // Enable Interrupt for Full Width Timer0 (32-Bit)
    //
    IntEnable(INT_TIMER0A );
    //
    // Enable Interrupt When TimeOut occurs
    //
    TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT );
    //
    // Enable NVIC Interrupt
    //
    IntMasterEnable();

    ulPeriod = SysCtlClockGet();
    //
    // Achieve 1sec Timer Interval
    //
    TimerLoadSet(TIMER0_BASE, TIMER_A   , ulPeriod-1  );

    //
    // Enable Timer0 and Start Count
    //
   // TimerEnable(TIMER0_BASE, TIMER_A);

    /* Enable Timer1 */

      //
      // Enable the peripherals used by this example.
      //

      //SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
      //// Wait for the Timer0 module to be ready.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_TIMER1));
    TimerConfigure(TIMER1_BASE, TIMER_CFG_ONE_SHOT);
    ulPeriod = SysCtlClockGet();
    TimerLoadSet(TIMER1_BASE, TIMER_A   , ulPeriod-1  );
    //TimerEnable(TIMER1_BASE, TIMER_A);



  //  uint32_t value = 1;
    TimerEnable(TIMER0_BASE, TIMER_A);
    //
    // Loop forever while the timers run.
    //
    while(1)
    {
        GPIOPinWrite(GPIO_PORTB_BASE ,  GPIO_PIN_5 ,  GPIO_PIN_5);
     //   TimerLoadSet(TIMER1_BASE, TIMER_A   , ulPeriod-1  );
     //   TimerEnable(TIMER1_BASE, TIMER_A);
      //  while ( ( (TIMER0_BASE+TIMER_O_RIS) & 0x1) == 0) ;
        SysCtlDelay( (ulPeriod-1)/3 );
        GPIOPinWrite(GPIO_PORTB_BASE ,  GPIO_PIN_5 ,  ~(1<<5) );
        SysCtlDelay( (ulPeriod-1)/3 );
       // while (( (TIMER0_BASE+TIMER_O_RIS) & 0x1) == 0) ;
    }

}
