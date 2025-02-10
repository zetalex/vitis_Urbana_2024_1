/******************************************************************************
* Copyright (C) 2025 Antonio Sanchez
******************************************************************************/

#include "xaxicdma.h"
#include "xdebug.h"
#include "xil_cache.h"
#include "xparameters.h"
#include "xil_util.h"
#include <xstatus.h>

#ifdef __aarch64__
#include "xil_mmu.h"
#endif

#if (!defined(DEBUG))
extern void xil_printf(const char *format, ...);
#endif

/******************** Constant Definitions **********************************/

/*
 * The following constants map to the XPAR parameters created in the
 * xparameters.h file. They are defined here such that a user can easily
 * change all the needed parameters in one place.
 */

#ifndef SDT
#ifndef TESTAPP_GEN
#define DMA_CTRL_DEVICE_ID	XPAR_AXICDMA_0_DEVICE_ID
#endif
#else
#define AXICDMA_BASE_ADDR       XPAR_XAXICDMA_0_BASEADDR
#endif

#define BUFFER_BYTESIZE		64	/* Length of the buffers for DMA
					 * transfer
					 */

#define NUMBER_OF_TRANSFERS	4	/* Number of transfers to do in this
					 * test
					 */

#define RESET_LOOP_COUNT	10	/* Number of times to check reset is
					 * done
					 */
#define POLL_TIMEOUT_COUNTER    1000000U

/**************************** Type Definitions *******************************/


/***************** Macros (Inline Functions) Definitions *********************/


/************************** Function Prototypes ******************************/


/************************** Variable Definitions *****************************/

static XAxiCdma AxiCdmaInstance;	/* Instance of the XAxiCdma */

static u32 PollTimeoutCounter;      /* Variable to control CDMA transfer tiemout */

int XAxiCdma_Transfer_Start(u8  *SrcBuffer, u8  *DestBuffer, int Length)
{
	int Status;

	XAxiCdma_Config *CfgPtr;
	int Retries = NUMBER_OF_TRANSFERS;

	/* Initialize the XAxiCdma device.
	 */
#ifndef SDT
    u16 DeviceId = DMA_CTRL_DEVICE_ID;
	CfgPtr = XAxiCdma_LookupConfig(DeviceId);
#else
    XAxiCdma *InstancePtr = &AxiCdmaInstance;
    UINTPTR BaseAddress = AXICDMA_BASE_ADDR;
	CfgPtr = XAxiCdma_LookupConfig(BaseAddress);
#endif
	if (!CfgPtr) {
		return XST_FAILURE;
	}

	Status = XAxiCdma_CfgInitialize(&AxiCdmaInstance, CfgPtr,
					CfgPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/* Disable interrupts, we use polling mode
	 */
	XAxiCdma_IntrDisable(&AxiCdmaInstance, XAXICDMA_XR_IRQ_ALL_MASK);

	/* Flush the SrcBuffer before the DMA transfer, in case the Data Cache
	 * is enabled
	 */
	Xil_DCacheFlushRange((UINTPTR)&SrcBuffer, Length);
	Xil_DCacheFlushRange((UINTPTR)&DestBuffer, Length);

	/* Try to start the DMA transfer
	 */
	while (Retries) {
		Retries -= 1;

		Status = XAxiCdma_SimpleTransfer(InstancePtr, (UINTPTR)SrcBuffer,
						 (UINTPTR)DestBuffer, Length, NULL, NULL);
		if (Status == XST_SUCCESS) {
			break;
		}
	}

	/* Return failure if failed to submit the transfer
	 */
	if (!Retries) {
		xdbg_printf(XDBG_DEBUG_ERROR,
			    "Failed to submit the transfer with %d\r\n", Status);
		return XST_FAILURE;
	}


    PollTimeoutCounter = POLL_TIMEOUT_COUNTER;

	/* DMA set finishes successfully
	 */
	return XST_SUCCESS;
}



int XAxiCdma_Transfer_End_Check()
{
    XAxiCdma *InstancePtr = &AxiCdmaInstance;
    u32 reg_value;
    u32 Status;

	/* Check DMA transfer is done or timeout
	 */
    reg_value = Xil_In32(InstancePtr->BaseAddr + XAXICDMA_SR_OFFSET);

    if ((reg_value &  (XAXICDMA_SR_IDLE_MASK)) == 0)
    {
        if(0==(--PollTimeoutCounter))
        {
            return XST_TIMEOUT;
        }
        else 
        {
            return XST_DEVICE_BUSY;
        }
    }


	/* If the hardware has errors, this example fails
	 * This is a poll example, no interrupt handler is involved.
	 * Therefore, error conditions are not cleared by the driver.
	 */
	Status = XAxiCdma_GetError(InstancePtr);
	if (Status != 0x0) {
		int TimeOut = RESET_LOOP_COUNT;

		/* Need to reset the hardware to restore to the correct state
		 */
		XAxiCdma_Reset(InstancePtr);

		while (TimeOut) {
			if (XAxiCdma_ResetIsDone(InstancePtr)) {
				break;
			}
			TimeOut -= 1;
		}

		/* Reset has failed, print a message to notify the user
		 */
		xdbg_printf(XDBG_DEBUG_ERROR, "Reset done failed\r\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
