/******************************************************************************
* Copyright (C) 2023 Advanced Micro Devices, Inc. All Rights Reserved.
* SPDX-License-Identifier: MIT
******************************************************************************/
/*
 * UART_hello_world.c: 
 * Esta aplicacion de ejemplo saca a través de la UART de la placa RealDigital Urbana una serie de caracteres.
 * Internamente, esto está implementado utilizando el tipico print que redirige su parametro de entrada a stdout (standard out).
 * Para que este ejemplo funcione correcamente es necesario tener configurado en el componente software de Vitis que incluye este codigo fuente
 * que stdout y stdin se envíen a la axi_uartlite
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
    init_platform();

    /* Print to stdout some characters*/
    print("Hello World\n\r");
    print("Successfully ran Hello World application");

    cleanup_platform();
    return 0;
}
