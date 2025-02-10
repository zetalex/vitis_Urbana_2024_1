/******************************************************************************
* Copyright (C) 2025 Antonio Sanchez
******************************************************************************/

#ifndef __CDMA_H_
#define __CDMA_H_

#include "xil_types.h"

#ifndef SDT
#include "platform_config.h"
#endif

/**
 * @brief Starts a CDMA transfer.
 * 
 * This function initializes the CDMA device and starts a transfer from the source buffer to the destination buffer.
 * It uses polling mode and retries the transfer a specified number of times if it fails initially.
 * 
 * @param SrcBuffer Pointer to the source buffer.
 * @param DestBuffer Pointer to the destination buffer.
 * @param Length Length of the data to be transferred.
 * 
 * @return Status of the CDMA transfer.
 * @retval XST_SUCCESS if the transfer was successful.
 * @retval XST_FAILURE if the transfer failed.
 */
int XAxiCdma_Transfer_Start(u8 *SrcBuffer, u8 *DestBuffer, int Length);

/**
 * @brief Checks the status of the CDMA transfer.
 * 
 * This function checks if the CDMA transfer is complete or if it has timed out.
 * It also checks for any hardware errors and resets the CDMA device if necessary.
 * 
 * @return Status of the CDMA transfer.
 * @retval XST_SUCCESS if the transfer was successful.
 * @retval XST_FAILURE if the transfer failed.
 * @retval XST_TIMEOUT if the transfer timed out.
 * @retval XST_DEVICE_BUSY if the device is still busy.
 */
int XAxiCdma_Transfer_End_Check();

#endif