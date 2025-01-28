/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * Switches_a_LED.c: 
 * Esta aplicacion de ejemplo lee los 16 switches (SW0-SW15) de la placa RealDigital AMD Urbana
 * y escribe el estado de los botones en los 16 LEDs verdes (LED0-LED15) de la misma placa.
 * Para tanto lectura como escritura se utiliza la libreria que controla el AXI GPIO instalado en la FPGA.
 */
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xparameters.h"
#include "sleep.h"

XGpio Gpio0; /* The Instance of the GPIO Driver */
XGpio Gpio1; /* The Instance of the GPIO Driver */

#define SW_CHANNEL 1
#define LED_CHANNEL 2
#define BTN_CHANNEL 1
#define LED_DELAY     1000000


int main()
{
    int Status0, Status1;
    u32 SW_read;
    u32 LED_write;
    init_platform();

    xil_printf("GPIO: Switch to LED\n\r");
    
    	/* Initialize the GPIO driver */
    #ifndef SDT
        Status0 = XGpio_Initialize(&Gpio0, GPIO_EXAMPLE_DEVICE_ID);
        Status1 = XGpio_Initialize(&Gpio1, GPIO_EXAMPLE_DEVICE_ID);
    #else
        Status0 = XGpio_Initialize(&Gpio0, XPAR_AXI_GPIO_0_BASEADDR);
        Status1 = XGpio_Initialize(&Gpio1, XPAR_AXI_GPIO_1_BASEADDR);
    #endif
        if (Status0 != XST_SUCCESS || Status1 != XST_SUCCESS ) {
            xil_printf("Gpio Initialization Failed\r\n");
            return XST_FAILURE;
        }
    
    XGpio_SetDataDirection(&Gpio0,SW_CHANNEL,
			    0xFFFFFFFF);

    XGpio_SetDataDirection(&Gpio0,LED_CHANNEL,
			    0x0);

    XGpio_SetDataDirection(&Gpio1,BTN_CHANNEL,
			    0xFFFFFFFF);

    while (1) {
        /* Read Switches */
        SW_read = XGpio_DiscreteRead(&Gpio0, SW_CHANNEL);
        LED_write = SW_read;
        /* Set the corresponding LEDs to the same level as its switch is indicating */
        //XGpio_WriteReg(Gpio0.BaseAddress,((LED_CHANNEL - 1) * XGPIO_CHAN_OFFSET) + XGPIO_DATA_OFFSET,LED_write);
        XGpio_DiscreteWrite(&Gpio0, LED_CHANNEL, LED_write);
	}
    cleanup_platform();
    return 0;
}
