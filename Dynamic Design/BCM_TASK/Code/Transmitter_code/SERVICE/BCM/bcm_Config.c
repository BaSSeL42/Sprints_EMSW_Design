/*
 * bcm_Config.c
 *
 *  Created on: May 23, 2023
 *      Author: hp
 */


#include "bcm_Interface.h"
#include "bcm_Config.h"

str_bcm_instance_t app_str_bcm_instance = {
		.enu_communication_sel = BCM_UART,
		.enu_tx_rx_state = BCM_TX_RX
};


Uint16_t u16_RX_BufferSize = 6;
