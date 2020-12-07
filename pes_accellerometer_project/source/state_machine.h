/*
 * state_machine.h: Contains Functions that implement state machines
 *
 * Created on: 02-Dec-2020
 *
 * Author: Chirayu Thakur
 *
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include "mma8451.h"
#include "i2c.h"
#include "tpm.h"


//function prototypes
void fsm(void);
void detect_motion(void);
void detect_freefall(void);
void detect_tap(void);

#endif /* STATE_MACHINE_H_ */
