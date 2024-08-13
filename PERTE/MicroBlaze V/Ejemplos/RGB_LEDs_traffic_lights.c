/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * LEDs_periodic.c: 
 * Esta aplicacion enciende los 2 LEDs RGB que tiene la placa RealDigital AMD Urbana imitando el comportamiento de un semáforo:
 * - Se enciende RGB1 en rojo
 * - Se encienden ambos RGB1 y RGB0 en amarillo (rojo + verde)
 * - Se enciende RGB0 en verde
 * 
 * Este comportamiento se repite cíclicamente.
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xgpio.h"
#include "xparameters.h"

XGpio Gpio; /* The Instance of the GPIO Driver */

#define SW_LED_CHANNEL 1
#define RGB_BTN_CHANNEL 2
#define LED_DELAY     1000000
#define RED 1
#define GREEN 2
#define YELLOW 3
#define RGB0_offset 4
#define RGB1_offset 7

int main()
{
    int Status;
    u32 Delay;
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

    XGpio_SetDataDirection(&Gpio,RGB_BTN_CHANNEL,
			    0xF);

    while (1) {

        /* Turn on the RGB LEDs for RED light */
        XGpio_DiscreteWrite(&Gpio, RGB_BTN_CHANNEL, (RED << RGB1_offset));

        /* Wait a small amount of time so the LED is visible */
		for (Delay = 0; Delay < LED_DELAY; Delay++);

        /* Turn on the RGB LEDs for yellow lights */
        XGpio_DiscreteWrite(&Gpio, RGB_BTN_CHANNEL, (YELLOW << RGB1_offset) | (YELLOW << RGB0_offset));

        /* Wait a small amount of time so the LED is visible */
		for (Delay = 0; Delay < LED_DELAY; Delay++);

        /* Turn on the RGB LEDs for green lights */
        XGpio_DiscreteWrite(&Gpio, RGB_BTN_CHANNEL, (GREEN << RGB0_offset));

        /* Wait a small amount of time so the LED is visible */
		for (Delay = 0; Delay < LED_DELAY; Delay++);
	}
    cleanup_platform();
    return 0;
}