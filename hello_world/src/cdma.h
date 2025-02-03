/******************************************************************************
* Copyright (C) 2025 Antonio Sanchez
******************************************************************************/

#ifndef __CDMA_H_
#define __CDMA_H_

#include "xil_types.h"

#ifndef SDT
#include "platform_config.h"
#endif

int XAxiCdma_Transfer_Start(u8  *SrcBuffer, u8  *DestBuffer, int Length);
int XAxiCdma_Transfer_End_Check();
#endif