
/******************************************************************************
* Copyright (C) 2025 Antonio Sanchez
******************************************************************************/

#ifndef __TIMER_H_
#define __TIMER_H_

#include "xil_types.h"

#ifndef SDT
#include "platform_config.h"
#endif

int timer_init();
u32 time_get();
#endif


