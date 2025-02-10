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

#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xtimer_config.h"
#include "timer.h"
#include "cdma.h"
#include "xstatus.h"
#include <string.h>
#include <stdlib.h>

#define BYTES_TO_TRANSFER 1000

u8 source[BYTES_TO_TRANSFER] __attribute__ ((aligned (64)));;
u8 destination[BYTES_TO_TRANSFER] __attribute__ ((aligned (64)));;

static u32 parallel_task_operations = 0;

static void buffer_check();

static void buffer_random_fill();

static void buffer_copy_without_dma();

static int buffer_copy_with_dma();

static void parallel_task();

int main()
{
    int Status;
    u32 time_initial,  time_final, time_elapsed;
    char time_elapsed_char[100];

    init_platform();

    Status = timer_init();
    if(XST_SUCCESS != Status)
    {
        print("Timer Init Error\n\r");
    }

    print("Hello HW/SW Codesigner!!\n\r");

    buffer_random_fill();

    time_initial = time_get();

    buffer_copy_without_dma();

    time_final = time_get();

    time_elapsed = time_final- time_initial;

    sprintf(time_elapsed_char,"Time elapsed NO DMA: %lu\n\r",time_elapsed);
    print(time_elapsed_char);
    buffer_check();


    buffer_random_fill();

    time_initial = time_get();

    buffer_copy_with_dma();

    time_final = time_get();
    
    time_elapsed = time_final- time_initial;
    
    sprintf(time_elapsed_char,"Time elapsed DMA: %lu\n\r",time_elapsed);
    print(time_elapsed_char);
    sprintf(time_elapsed_char,"Parallel task iterations executed: %lu\n\r",parallel_task_operations);
    print(time_elapsed_char);
    buffer_check();
    
    cleanup_platform();
    return 0;
}


/**
 * @brief Fills the source buffer with random data.
 */
static void buffer_random_fill()
{
    srand((unsigned int)time_get());
    for(u32 pos = 0; pos < BYTES_TO_TRANSFER; pos++)
    {
        source[pos]=(u8)rand();
    }
}

/**
 * @brief Copies data from the source buffer to the destination buffer without using DMA.
 */
static void buffer_copy_without_dma()
{
    for(u32 pos = 0; pos < BYTES_TO_TRANSFER; pos++)
    {
        destination[pos]=source[pos];
    }
}

/**
 * @brief Copies data from the source buffer to the destination buffer using DMA.
 * 
 * @return Status of the DMA transfer.
 * @retval XST_SUCCESS if the transfer was successful.
 * @retval XST_FAILURE if the transfer failed.
 */
static int buffer_copy_with_dma()
{
    int Status;

    if (Status = XAxiCdma_Transfer_Start(source, destination, BYTES_TO_TRANSFER), XST_SUCCESS != Status)
    {
        return Status;
    }

    do {
        Status = XAxiCdma_Transfer_End_Check();
        parallel_task();

    }while (XST_DEVICE_BUSY == Status);

    return Status;
}

/**
 * @brief Checks if the data in the source and destination buffers are the same.
 */
static void buffer_check()
{
    u32 pos;
    for(pos = 0; pos < BYTES_TO_TRANSFER; pos++)
    {
        if(destination[pos]!=source[pos])
        {
            break;
        }
    }

    if(BYTES_TO_TRANSFER != pos)
    {
        print ("ERROR: Buffers are not the same\n\r");
    }
    else {
        print ("SUCCESS: Buffers are the same\n\r");
    }
}

/**
 * @brief Performs a parallel task operation.
 */
static void parallel_task()
{
    ++parallel_task_operations;
}