/*
 * bcm_Interface.h
 *
 *  Created on: May 22, 2023
 *      Author: hp
 */


/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											Start bcm_Interface.h
 ********************************************************************************************************/
/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											File Guard
 ********************************************************************************************************/
#ifndef SERVICE_BCM_BCM_INTERFACE_H_
#define SERVICE_BCM_BCM_INTERFACE_H_
/********************************************************************************************************
 * 											Includes
 ********************************************************************************************************/
#include "../../Common/std_types.h"

/********************************************************************************************************
 * 											User defined Data type
 ********************************************************************************************************/

/********************************  enum error for return  ***********************************************/
typedef enum
{
	BCM_NOK = 0,
	BCM_OK

}enu_system_status_t;

/********************************  Communication protocol type  *****************************************/
typedef enum
{
	BCM_UART = 0,
	BCM_SPI,
	BCM_I2C,

	BCM_COMM_INVALID

}enu_communication_sel_t;

/********************************  operation state  ***********************************************/
typedef enum
{
	BCM_TX = 0,
	BCM_RX,
	BCM_TX_RX,

	BCM_STATE_INVALID

}enu_tx_rx_state_t;

/****************************  BCM structer to be configured by user  ******************************/
typedef struct
{
	enu_communication_sel_t enu_communication_sel;
	enu_tx_rx_state_t		enu_tx_rx_state;

}str_bcm_instance_t;


/********************************************************************************************************
 * 											Function Prototypes
 ********************************************************************************************************/
/*
 * Author			: Bassel Yasser
 * Function			: bcm_init
 * Description		: - Choosing type of communication protocol
 * 					  - choosing operation states {transmitter or receiver}
 * Parameter [1]	: ptr_str_instance_t --> address of structer instance
 * return			: enu_system_status_t
 */
enu_system_status_t bcm_init(str_bcm_instance_t* ptr_str_instance_t);

/*
 * Author			: Bassel Yasser
 * Function			: bcm_deinit
 * Description		: De initialize selected communication protocol
 * Parameter [1]	: ptr_str_instance_t --> address of structer instance
 * return			: enu_system_status_t
 */
enu_system_status_t bcm_deinit(str_bcm_instance_t* ptr_str_instance_t);



/*
 * Author			: Bassel Yasser
 * Function			: bcm_send
 * Description		: Send Data via selected communication protocol
 * Parameter [1]	: ptr_str_instance_t --> address of structer instance
 * Parameter [2] 	: u8_one_byte_data 	 --> variable of uint8
 * return			: enu_system_status_t
 */
enu_system_status_t bcm_send(str_bcm_instance_t* ptr_str_instance_t, Uint8_t u8_one_byte_data);

/*
 * Author			: Bassel Yasser
 * Function			: bcm_send_n
 * Description		: Send Group of Data via selected communication protocol
 * Parameter [1]	: ptr_str_instance_t 	 --> address of structer instance
 * Parameter [2] 	: ptr_u8_n_byte_data 	 --> Address of array or string to be sent
 * Parameter [3] 	: u16_byte_length	 	 --> Buffer size of uint16
 * return			: enu_system_status_t
 */
enu_system_status_t bcm_send_n(str_bcm_instance_t* ptr_str_instance_t, Uint8_t* ptr_u8_n_byte_data, Uint16_t u16_byte_length);



/*
 * Author			: Bassel Yasser
 * Function			: bcm_dispatcher
 * Description		: is periodic function and notifies the user with neede event
 * Parameter [1]	: ptr_str_instance_t --> address of structer instance
 * return			: enu_system_status_t
 */
enu_system_status_t bcm_dispatcher(str_bcm_instance_t* ptr_str_instance_t);



#endif /* SERVICE_BCM_BCM_INTERFACE_H_ */
/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											End bcm_Interface.h
 ********************************************************************************************************/
/********************************************************************************************************
 ********************************************************************************************************/
