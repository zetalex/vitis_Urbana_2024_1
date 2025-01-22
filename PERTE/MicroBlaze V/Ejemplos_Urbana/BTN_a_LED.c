/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * BTN_a_LED.c: 
 * Esta aplicacion de ejemplo lee los 4 botones (BTN0-BTN3) de la placa RealDigital AMD Urbana
 * y escribe el estado de los botones en los 4 primeros LEDs (LED0-LED3) de la misma placa.
 * Para tanto lectura como escritura se utiliza la libreria que controla el AXI GPIO instalado en la FPGA.
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xparameters.h"

XGpio Gpio; /* The Instance of the GPIO Driver */

#define SW_LED_CHANNEL 1
#define BTN_CHANNEL 2
#define LED_DELAY     1000000

int main()
{
    int Status;
    u32 BTN_read;
    u32 LED_write;
    init_platform();

    print("Hello World\n\r");
    print("Successfully ran Hello World application");
    
    	/* Initialize the GPIO driver */
    #ifndef SDT
        Status = XGpio_Initialize(&Gpio, GPIO_EXAMPLE_DEVICE_ID);
    #else
        Status = XGpio_Initialize(&Gpio, XPAR_AXI_GPIO_0_BASEADDR);
    #endif
        if (Status != XST_SUCCESS) {
            xil_printf("Gpio Initialization Failed\r\n");
            return XST_FAILURE;
        }
    
    XGpio_SetDataDirection(&Gpio,SW_LED_CHANNEL,
			    0x0000FFFF);

    XGpio_SetDataDirection(&Gpio,BTN_CHANNEL,
			    0xF);

    while (1) {
        /* Read Buttons */
        BTN_read = XGpio_DiscreteRead(&Gpio, BTN_CHANNEL);
        LED_write = BTN_read << 16
        /* Set the corresponding LEDs to the same level as its button is indicating */
        XGpio_DiscreteWrite(&Gpio, SW_LED_CHANNEL, LED_write);
	}
    cleanup_platform();
    return 0;
}
