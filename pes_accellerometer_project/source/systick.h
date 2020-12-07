/*
 * systick.h: Contains functions to initialize SysTick  timer
 *
 * Created on: 05-Dec-2020
 *
 * Author: Chirayu Thakur
 */


#ifndef SYSTICK_H_
#define SYSTICK_H_

#include "MKL25Z4.h"

//typedef
typedef uint32_t ticktime_t;

//Systic Timer Interval
#define INTERVAL_10_MS 1
#define INTERVAL_20_MS 2
#define INTERVAL_500_MS   50
#define INTERVAL_2000_MS  200

//function prototypes
void Init_SysTick(void);
ticktime_t now(void);
void reset_timer(void);
ticktime_t get_timer(void);

#endif /* SYSTICK_H_ */
