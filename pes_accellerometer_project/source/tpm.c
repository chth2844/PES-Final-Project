/*
 * tpm.c: Contains TPM Timer Functions
 *
 * Created on: 29-Nov-2020
 *
 * Author: Chirayu Thakur
 *
 */
#include "tpm.h"
#include "state_machine.h"
#define MOD_VALUE 749 //Defines how high the CNT register counts

/*
 * Initializes PWM peripheral
 *
 * Parameters:None
 *
 * Returns: none
 *
 */
void init_led_tpm(void)
{

	// Enable Clock to Port B and Port D
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK|SIM_SCGC5_PORTD_MASK;
	SIM->SCGC5|=SIM_SCGC5_PORTD_MASK;
	//Enable clock to TPM0
	SIM->SCGC6|=SIM_SCGC6_TPM0_MASK;
	//Enable clock to TPM2
	SIM->SCGC6|=SIM_SCGC6_TPM2_MASK;
	//Set PIN to FTM
	PORTD->PCR[BLUE_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTD->PCR[BLUE_LED_SHIFT] |= PORT_PCR_MUX(4);
	PORTB->PCR[RED_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[RED_LED_SHIFT] |= PORT_PCR_MUX(3);
	PORTB->PCR[GREEN_LED_SHIFT] &= ~PORT_PCR_MUX_MASK;
	PORTB->PCR[GREEN_LED_SHIFT] |= PORT_PCR_MUX(3);
	//Select clock source for TPM 48Mhz
	SIM->SOPT2|=SIM_SOPT2_TPMSRC(1)|SIM_SOPT2_PLLFLLSEL_MASK;
	//Load the Counter and MOD
	TPM0->MOD=MOD_VALUE;
	TPM2->MOD=MOD_VALUE;
	//Set Count to Up direction with divide by 128 prescaler
	TPM0->SC=TPM_SC_PS(128);
	//Continue operation in debug mode
	//TPM0->CONF
	//Set Channel 1 to edge aligned low true TPM
	TPM0->CONTROLS[1].CnSC=TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK;
	//Set Initial Duty Cycle
	TPM0->CONTROLS[1].CnV=0;
	//Set Channel 2 to edge aligned low true TPM
	TPM2->CONTROLS[0].CnSC=TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK;
	//Set Initial Duty Cycle
	TPM2->CONTROLS[0].CnV=0;
	TPM2->CONTROLS[1].CnSC=TPM_CnSC_MSB_MASK|TPM_CnSC_ELSA_MASK;
	//Set Initial Duty Cycle
	TPM2->CONTROLS[1].CnV=0;
	//Start TPM
	TPM0->SC|=TPM_SC_CMOD(1);
	TPM2->SC|=TPM_SC_CMOD(1);



}

/*
 * Sets Colour using PWM timer
 *
 * Parameters:RGB colour values
 *
 * Returns: none
 *
 */
void set_colour(RGB_Value_t colour_val)
{
	TPM2->CONTROLS[0].CnV=colour_val.RED_Value; //Compute value of colour for RED LED
    TPM2->CONTROLS[1].CnV=colour_val.GREEN_Value; //Compute value of colour for GREEN LED
	TPM0->CONTROLS[1].CnV=colour_val.BLUE_Value; //Compute value of colour for BLUE LED

}

