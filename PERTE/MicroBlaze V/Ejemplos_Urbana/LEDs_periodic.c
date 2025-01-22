/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * LEDs_periodic.c: 
 * Esta aplicacion enciende los 16 LEDs de la placa Urbana y, transcurrido un tiempo, marcado por LED_DELAY los apaga.
 * Este comportamiento se repite cíclicamente. En este ejemplo, el delay no es preciso, no puede darse en unidades de tiempo
 * Esto se debe a que la implementacion consiste en un bucle for que se ejecuta un numero determinado de veces, por lo que depende de la velocidad de la CPU.
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
    //volatile int Delay;
    u32 SW_read;
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
		/* Set the LED to High */
		XGpio_DiscreteWrite(&Gpio, SW_LED_CHANNEL, 0xFFFF0000);

		/* Wait a small amount of time so the LED is visible */
		for (Delay = 0; Delay < LED_DELAY; Delay++);

		/* Clear the LED bit */
		XGpio_DiscreteWrite(&Gpio, SW_LED_CHANNEL, 0x0);

		/* Wait a small amount of time so the LED is visible */
		for (Delay = 0; Delay < LED_DELAY; Delay++);
	}
    cleanup_platform();
    return 0;
}
