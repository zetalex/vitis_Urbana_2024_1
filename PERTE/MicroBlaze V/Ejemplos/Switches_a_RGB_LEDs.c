/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
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