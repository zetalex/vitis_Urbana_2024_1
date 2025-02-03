/******************************************************************************
* Copyright (C) 2025 Antonio Sanchez
******************************************************************************/

#include "xparameters.h"
#include "xtmrctr.h"
#include <xil_types.h>

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are only defined here such that a user can easily
 * change all the needed parameters in one place.
 */
#ifndef SDT
#define TMRCTR_DEVICE_ID	XPAR_TMRCTR_0_DEVICE_ID
#else
#define XTMRCTR_BASEADDRESS	XPAR_XTMRCTR_0_BASEADDR
#endif


/*
 * This example only uses the 1st of the 2 timer counters contained in a
 * single timer counter hardware device
 */
#define TIMER_COUNTER_0	 0

XTmrCtr TimerCounter; /* The instance of the Tmrctr Device */
u8 TmrCtrNumber;


int timer_init()
{
    int Status;
	XTmrCtr *TmrCtrInstancePtr = &TimerCounter;
    TmrCtrNumber = TIMER_COUNTER_0;
	/*
	 * Initialize the timer counter so that it's ready to use,
	 * specify the device ID that is generated in xparameters.h
	 */


#ifndef SDT
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, TMRCTR_DEVICE_ID);
#else
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, XTMRCTR_BASEADDRESS);
#endif

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Perform a self-test to ensure that the hardware was built
	 * correctly, use the 1st timer in the device (0)
	 */
	Status = XTmrCtr_SelfTest(TmrCtrInstancePtr, TmrCtrNumber);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	/*
	 * Enable the Autoreload mode of the timer counters.
	 */
	XTmrCtr_SetOptions(TmrCtrInstancePtr, TmrCtrNumber,
			   XTC_AUTO_RELOAD_OPTION);

	/*
	 * Start the timer counter such that it's incrementing by default
	 */
	XTmrCtr_Start(TmrCtrInstancePtr, TmrCtrNumber);      

    return XST_SUCCESS;        
}

u32 time_get()
{
    return XTmrCtr_GetValue(&TimerCounter, TmrCtrNumber);
}