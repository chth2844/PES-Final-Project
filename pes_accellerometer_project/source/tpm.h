/*
 * tpm.h: Contains TPM Timer Functions
 *
 * Created on: 29-Nov-2020
 *
 * Author: Chirayu Thakur
 *
 */
#ifndef TPM_H_
#define TPM_H_

#include "MKL25Z4.h"
#include "state_machine.h"

#define RED_LED_SHIFT (18)
#define GREEN_LED_SHIFT (19)
#define BLUE_LED_SHIFT (1)
#define MASK(x) (1UL << (x))

typedef struct RGB_Value
{
	int RED_Value;
	int GREEN_Value;
	int BLUE_Value;
} RGB_Value_t;

//function prototypes
void set_colour( RGB_Value_t colour_val);
void init_led_tpm(void);
void Delay_ms(int ctr);


#endif /* TPM_H_ */

