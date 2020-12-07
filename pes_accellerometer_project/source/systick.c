/*
 * systick.c: Contains functions to initialize SysTick  timer
 *
 * Created on: 05-Dec-2020
 *
 * Author: Chirayu Thakur
 */

#include "systick.h"

volatile uint32_t count_value;
volatile uint32_t reset_value;
volatile uint16_t overflow;
#define MAX 4294967294 //Maximum uint32 value
#define CLK 48000000L  //Internal clock Frequency
#define DIV 100        //Frequency corresponding to 10ms

/*
 * Initializes SysTick Timer
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
void Init_SysTick(void)
{
	SysTick->LOAD=(CLK)/DIV;//Set Reload to get 10ms interrupts
	NVIC_SetPriority(SysTick_IRQn,3);//Set Interrupt Priority
	SysTick->VAL=0;//Force load of reload value
	SysTick->CTRL=SysTick_CTRL_TICKINT_Msk|SysTick_CTRL_ENABLE_Msk|SysTick_CTRL_CLKSOURCE_Msk ; //Enable Interrupt,
	                                                                                            //Internal Clock,Enable Systic Timer

}

/*
 * Returns Time in SysTick counts since start of program
 *
 * Parameters:None
 *
 * Returns: Time in SysTick counts
 *
 */
ticktime_t now(void)
{
	if(count_value>=MAX)
	{
		return 0;
	}

	else {

		return count_value;
	}
}

/*
 * Resets Systic timer
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
void reset_timer(void)
{
	reset_value=0;


}

/*
 * Return the time since the last call to reset_timer()
 *
 * Parameters:None
 *
 * Returns:
 *
 *      uint32_t reset_value: Time in Systick counts since last call to reset_timer()
 *
 */
ticktime_t get_timer(void)
{
	return reset_value;
}



/*
 * Interrupt Request handler for SysTick counter
 *
 * Parameters:None
 *
 * Returns: None
 *
 */
void SysTick_Handler()
{

	if((SysTick->CTRL)&SysTick_CTRL_COUNTFLAG_Msk)
	{
		uint32_t masking_state = __get_PRIMASK();
		__disable_irq();
		reset_value++;
		count_value++;
		__set_PRIMASK(masking_state);
	 }
}

