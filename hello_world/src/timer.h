/******************************************************************************
* Copyright (C) 2025 Antonio Sanchez
******************************************************************************/

#ifndef __TIMER_H_
#define __TIMER_H_

#include "xil_types.h"

#ifndef SDT
#include "platform_config.h"
#endif

/**
 * @brief Initializes the timer.
 * 
 * This function initializes the timer counter so that it's ready to use.
 * It performs a self-test to ensure that the hardware was built correctly,
 * enables the Autoreload mode of the timer counters, and starts the timer counter.
 * 
 * @return Status of the timer initialization.
 * @retval XST_SUCCESS if the initialization was successful.
 * @retval XST_FAILURE if the initialization failed.
 */
int timer_init();

/**
 * @brief Gets the current timer value.
 * 
 * This function returns the current value of the timer counter.
 * 
 * @return The current timer value.
 */
u32 time_get();

#endif


