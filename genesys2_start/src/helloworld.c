/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * Switches_a_LED.c: 
 * Esta aplicacion de ejemplo lee los 8 switches (SW0-SW7) de la placa Genesys2 Digilent
 * y escribe el estado de los botones en los 8 LEDs verdes (LED0-LED7) de la misma placa.
 * Para tanto lectura como escritura se utiliza la libreria que controla el AXI GPIO instalado en la FPGA.
 */


//#define EXERCISE_1
//#define EXERCISE_2
#define EXERCISE_3

/***************************** Include Files *********************************/
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xparameters.h"

//includes for exercise 2
#include "sleep.h"
#include "xiltimer.h"

//includes for exercise 3
#include "xil_exception.h"
#include "xinterrupt_wrap.h"

/************************** Constant Definitions *****************************/




XGpio Gpio_sw_led; /* The Instance of the GPIO Driver */
XGpio Gpio_btn;     /*The instance of the GPIO Driver for key buttons */

#define SW_CHANNEL   1
#define LED_CHANNEL      2
#define LED_DELAY   100000


#ifdef EXERCISE_2
#define NUMBER_OF_SW 8
#endif

#ifdef EXERCISE_3
#define BUTTON_INTERRUPT XGPIO_IR_CH1_MASK  /* Channel 1 Interrupt Mask is for BTN and SW */
/*
 * The following constant determines which buttons must be pressed at the same
 * time to cause interrupt processing to stop and start
 */
#define INTERRUPT_CONTROL_VALUE 0x7
#define INTR_DELAY	0x00FFFFFF
#define	XGPIO_AXI_BASEADDRESS	XPAR_XGPIO_1_BASEADDR
#define BTN_CHANNEL		1

/************************** Function Prototypes ******************************/
void GpioHandler(void *CallBackRef);

int GpioIntrExample(XGpio *InstancePtr,
		    UINTPTR BaseAddress,
		    u16 IntrMask, u32 *DataRead);

static u16 GlobalIntrMask; /* GPIO channel mask that is needed by the Interrupt Handler */
static volatile u32 IntrFlag; /* Interrupt Handler Flag */

#endif





int main()
{
    int Status;
    u32 i;
    u32 SW_read;
    u32 BTN_read;


    i = 0;

    init_platform();

    print("GPIO: Switch to LED\n\r");
    
   	/* Initialize the GPIO driver */
    Status = XGpio_Initialize(&Gpio_sw_led, XPAR_AXI_GPIO_0_BASEADDR);
    if (Status != XST_SUCCESS) {
        xil_printf("Gpio Initialization Failed\r\n");
        return XST_FAILURE;
    }

    /* SW are inputs */
    XGpio_SetDataDirection(&Gpio_sw_led,SW_CHANNEL,
			    0x0000FFFF);
    /* LED are outputs */
    XGpio_SetDataDirection(&Gpio_sw_led,LED_CHANNEL,
			    0x0);

    while (1) {
        /* Read Switches */
        SW_read = XGpio_DiscreteRead(&Gpio_sw_led, SW_CHANNEL);
        

        #ifdef EXERCISE_1
        LED_write = SW_read;
        /* Set the corresponding LEDs to the same level as its switch is indicating */
        XGpio_DiscreteWrite(&Gpio_sw_led, LED_CHANNEL, LED_write);
        #endif
        
        #ifdef EXERCISE_2
        /* Playing with leds...
            
            if any SW is ON one led start lighthing from led0 to led7 with a LED_DELAY
            if all SW are OFF there is a light game on leds lighting odds or evens with a LED_DELAY
            
            */

        LED_write = 0x00000001;
        if (SW_read!=0){
            if (i<NUMBER_OF_SW-1){
                i = i+1;
            }
            else{
                i = 0;
            }
            
            LED_write = LED_write<<i;
            XGpio_DiscreteWrite(&Gpio_sw_led, LED_CHANNEL, LED_write);
            sleep(1);
        //    usleep(250000);

        }
        else{
              LED_write = 0x000000AA;
              XGpio_DiscreteWrite(&Gpio_sw_led, LED_CHANNEL, LED_write);
              for (Delay = 0; Delay < LED_DELAY; Delay++);
              LED_write = 0x00000055;
              XGpio_DiscreteWrite(&Gpio_sw_led, LED_CHANNEL, LED_write);
              for (Delay = 0; Delay < LED_DELAY; Delay++);
        }
    
        #endif

        #ifdef EXERCISE_3
        Status = GpioIntrExample(&Gpio_btn,XGPIO_AXI_BASEADDRESS,BTN_CHANNEL, &BTN_read);

        if (Status == 0 ) {
            if (BTN_read == 0) {
                print("No button pressed. \r\n");
            } else {
			    print("Successfully ran Gpio Interrupt Tapp Example\r\n");
		    }
        } else {
		    print("Gpio Interrupt Tapp Example Failed.\r\n");
		    return XST_FAILURE;
	    }

	    return XST_SUCCESS;
        #endif

	}

    cleanup_platform();
    return 0;
}

#ifdef EXERCISE_3


/******************************************************************************/
/**
*
* This is the entry function from the TestAppGen tool generated application
* which tests the interrupts when enabled in the GPIO
*
* @param	IntcInstancePtr is a reference to the Interrupt Controller
*		driver Instance
* @param	InstancePtr is a reference to the GPIO driver Instance
* @param	DeviceId is the XPAR_<GPIO_instance>_DEVICE_ID value from
*		xparameters.h
* @param	IntrId is XPAR_<INTC_instance>_<GPIO_instance>_IP2INTC_IRPT_INTR
*		value from xparameters.h
* @param	IntrMask is the GPIO channel mask
* @param	DataRead is the pointer where the data read from GPIO Input is
*		returned
*
* @return
*		- XST_SUCCESS if the Test is successful
*		- XST_FAILURE if the test is not successful
*
* @note		None.
*
******************************************************************************/
#ifndef SDT
int GpioIntrExample(INTC *IntcInstancePtr, XGpio *InstancePtr, u16 DeviceId,
		    u16 IntrId, u16 IntrMask, u32 *DataRead)
#else
int GpioIntrExample(XGpio *InstancePtr, UINTPTR BaseAddress,
		    u16 IntrMask, u32 *DataRead)
#endif
{
	int Status;
	u32 delay;
#ifdef SDT
	XGpio_Config *ConfigPtr;

	ConfigPtr = XGpio_LookupConfig(BaseAddress);
#endif
	/* Initialize the GPIO driver. If an error occurs then exit */
#ifndef SDT
	Status = XGpio_Initialize(InstancePtr, DeviceId);
#else
	Status = XGpio_Initialize(InstancePtr, BaseAddress);
#endif
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

#ifndef SDT
	Status = GpioSetupIntrSystem(IntcInstancePtr, InstancePtr, DeviceId,
				     IntrId, IntrMask);
#else
    GlobalIntrMask = IntrMask;
	Status = XSetupInterruptSystem(InstancePtr, &GpioHandler,
				       ConfigPtr->IntrId,
				       ConfigPtr->IntrParent,
				       XINTERRUPT_DEFAULT_PRIORITY);

    u32 Register;
    // Instead of calling the driver functions, I call directly the needed reads and writes in the registers
    Register = XGpio_ReadReg(InstancePtr->BaseAddress, XGPIO_IER_OFFSET);
	XGpio_WriteReg(InstancePtr->BaseAddress, XGPIO_IER_OFFSET,Register | IntrMask);
	//XGpio_InterruptEnable(InstancePtr, IntrMask);
    XGpio_WriteReg(InstancePtr->BaseAddress, XGPIO_GIE_OFFSET,XGPIO_GIE_GINTR_ENABLE_MASK);
	//XGpio_InterruptGlobalEnable(InstancePtr);
#endif
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	IntrFlag = 0;
	delay = 0;

	while (!IntrFlag && (delay < INTR_DELAY)) {
		delay++;
	}

#ifndef SDT
	GpioDisableIntr(IntcInstancePtr, InstancePtr, IntrId, IntrMask);
#else
	XDisconnectInterruptCntrl(ConfigPtr->IntrId, ConfigPtr->IntrParent);
#endif

	*DataRead = IntrFlag;

	return Status;
}

#ifndef SDT
/******************************************************************************/
/**
*
* This function performs the GPIO set up for Interrupts
*
* @param	IntcInstancePtr is a reference to the Interrupt Controller
*		driver Instance
* @param	InstancePtr is a reference to the GPIO driver Instance
* @param	DeviceId is the XPAR_<GPIO_instance>_DEVICE_ID value from
*		xparameters.h
* @param	IntrId is XPAR_<INTC_instance>_<GPIO_instance>_IP2INTC_IRPT_INTR
*		value from xparameters.h
* @param	IntrMask is the GPIO channel mask
*
* @return	XST_SUCCESS if the Test is successful, otherwise XST_FAILURE
*
* @note		None.
*
******************************************************************************/
int GpioSetupIntrSystem(INTC *IntcInstancePtr, XGpio *InstancePtr,
			u16 DeviceId, u16 IntrId, u16 IntrMask)
{
	int Result;

	GlobalIntrMask = IntrMask;

#ifdef XPAR_INTC_0_DEVICE_ID

#ifndef TESTAPP_GEN
	/*
	 * Initialize the interrupt controller driver so that it's ready to use.
	 * specify the device ID that was generated in xparameters.h
	 */
	Result = XIntc_Initialize(IntcInstancePtr, INTC_DEVICE_ID);
	if (Result != XST_SUCCESS) {
		return Result;
	}
#endif /* TESTAPP_GEN */

	/* Hook up interrupt service routine */
	XIntc_Connect(IntcInstancePtr, IntrId,
		      (Xil_ExceptionHandler)GpioHandler, InstancePtr);

	/* Enable the interrupt vector at the interrupt controller */
	XIntc_Enable(IntcInstancePtr, IntrId);

#ifndef TESTAPP_GEN
	/*
	 * Start the interrupt controller such that interrupts are recognized
	 * and handled by the processor
	 */
	Result = XIntc_Start(IntcInstancePtr, XIN_REAL_MODE);
	if (Result != XST_SUCCESS) {
		return Result;
	}
#endif /* TESTAPP_GEN */

#else /* !XPAR_INTC_0_DEVICE_ID */

#ifndef TESTAPP_GEN
	XScuGic_Config *IntcConfig;

	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}

	Result = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
				       IntcConfig->CpuBaseAddress);
	if (Result != XST_SUCCESS) {
		return XST_FAILURE;
	}
#endif /* TESTAPP_GEN */

	XScuGic_SetPriorityTriggerType(IntcInstancePtr, IntrId,
				       0xA0, 0x3);

	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
	Result = XScuGic_Connect(IntcInstancePtr, IntrId,
				 (Xil_ExceptionHandler)GpioHandler, InstancePtr);
	if (Result != XST_SUCCESS) {
		return Result;
	}

	/* Enable the interrupt for the GPIO device.*/
	XScuGic_Enable(IntcInstancePtr, IntrId);
#endif /* XPAR_INTC_0_DEVICE_ID */

	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_InterruptEnable(InstancePtr, IntrMask);
	XGpio_InterruptGlobalEnable(InstancePtr);

	/*
	 * Initialize the exception table and register the interrupt
	 * controller handler with the exception table
	 */
	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
				     (Xil_ExceptionHandler)INTC_HANDLER, IntcInstancePtr);

	/* Enable non-critical exceptions */
	Xil_ExceptionEnable();

	return XST_SUCCESS;
}
#endif

/******************************************************************************/
/**
*
* This is the interrupt handler routine for the GPIO for this example.
*
* @param	CallbackRef is the Callback reference for the handler.
*
* @return	None.
*
* @note		None.
*
******************************************************************************/
void GpioHandler(void *CallbackRef)
{
	XGpio *GpioPtr = (XGpio *)CallbackRef;

	IntrFlag = 1;

	/* Clear the Interrupt */
    u32 Register;
    Register = XGpio_ReadReg(GpioPtr->BaseAddress, XGPIO_ISR_OFFSET);
	XGpio_WriteReg(GpioPtr->BaseAddress, XGPIO_ISR_OFFSET,Register & GlobalIntrMask);
    Register = XGpio_ReadReg(GpioPtr->BaseAddress, XGPIO_ISR_OFFSET);
	//XGpio_InterruptClear(GpioPtr, GlobalIntrMask);
    return;

}

#ifndef SDT
/******************************************************************************/
/**
*
* This function disables the interrupts for the GPIO
*
* @param	IntcInstancePtr is a pointer to the Interrupt Controller
*		driver Instance
* @param	InstancePtr is a pointer to the GPIO driver Instance
* @param	IntrId is XPAR_<INTC_instance>_<GPIO_instance>_VEC
*		value from xparameters.h
* @param	IntrMask is the GPIO channel mask
*
* @return	None
*
* @note		None.
*
******************************************************************************/
void GpioDisableIntr(INTC *IntcInstancePtr, XGpio *InstancePtr,
		     u16 IntrId, u16 IntrMask)
{
	XGpio_InterruptDisable(InstancePtr, IntrMask);
#ifdef XPAR_INTC_0_DEVICE_ID
	XIntc_Disable(IntcInstancePtr, IntrId);
#else
	/* Disconnect the interrupt */
	XScuGic_Disable(IntcInstancePtr, IntrId);
	XScuGic_Disconnect(IntcInstancePtr, IntrId);
#endif
	return;
}
#endif

#endif /*EXERCISE_3*/