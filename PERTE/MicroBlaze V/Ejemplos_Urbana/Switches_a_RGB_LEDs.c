/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * Switches_a_RGB_LEDs.c: 
 * Esta aplicacion de ejemplo lee los 6 primeros switches (SW0-SW5) de la placa RealDigital AMD Urbana
 * y escribe el estado de los mismos en los 2 LEDs RGBs (RGB0 y RGB1) de la misma placa.
 * Para tanto lectura como escritura se utiliza la libreria que controla el AXI GPIO instalado en la FPGA.
 * El control de los colores es el siguiente:
 * - RGB0: SW0 Rojo, SW1 verde, SW2 azul
 * - RGB1: SW3 Rojo, SW4 verde, Sw5 azul
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

int main()
{
    int Status;
    u32 SW_read;
    u32 RGB_LED_write;
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
        /* Read Switches */
        SW_read = XGpio_DiscreteRead(&Gpio, SW_LED_CHANNEL);

        /* AND mask to only take SW0-SW5 and displace bits for writing them in the second GPIO channel, in the RGB LEDs zone*/
        RGB_LED_write = (SW_read & 0x3F) << 4;

        /* Turn on the RGB LEDs in the Urbana depending on the switches SW0-SW5 */
        XGpio_DiscreteWrite(&Gpio, RGB_BTN_CHANNEL, RGB_LED_write);
	}
    cleanup_platform();
    return 0;
}