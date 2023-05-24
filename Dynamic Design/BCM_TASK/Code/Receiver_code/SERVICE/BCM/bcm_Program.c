/*
 * bcm_Program.c

 *
 *  Created on: May 22, 2023
 *      Author: hp
 */
/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											Start bcm_Program.c
 ********************************************************************************************************/
/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											Includes
 ********************************************************************************************************/
#include <util/delay.h>
#include "../../MCAL/UART/MUART_Interface.h"
#include "../../HAL/LED/led.h"
#include "../../HAL/HUART/HUART_Interface.h"
#include "bcm_Interface.h"
#include "bcm_Config.h"

/********************************************************************************************************
 * 											Global and Static Variables
 ********************************************************************************************************/
static Uint32_t flag = 0;

/********************************************************************************************************
 * 											Extern Variable
 ********************************************************************************************************/
extern volatile Uint8_t gvu8_txComplete;
extern volatile Uint8_t gvu8_rxComplete;
extern volatile Uint8_t g_p_u8_rx_string_buffer[30];
extern str_bcm_instance_t app_str_bcm_instance ;
extern Uint16_t u16_RX_BufferSize ;


/********************************************************************************************************
 * 											Functions Implmentation
 ********************************************************************************************************/

/*
 * Author			: Bassel Yasser
 * Function			: bcm_init
 * Description		: - Choosing type of communication protocol
 * 					  - choosing operation states {transmitter or receiver}
 * Parameter [1]	: ptr_str_instance_t --> address of structer instance
 * return			: enu_system_status_t
 */

enu_system_status_t bcm_init(str_bcm_instance_t* ptr_str_instance_t)
{
	enu_system_status_t enu_system_status = BCM_OK;

	if (ptr_str_instance_t->enu_communication_sel == BCM_UART)
	{
		enu_system_status = HUART_enInit(BCM_UART_BAUDRATE, ptr_str_instance_t->enu_tx_rx_state);
		if (ptr_str_instance_t->enu_tx_rx_state == BCM_RX || ptr_str_instance_t->enu_tx_rx_state == BCM_TX_RX)
		{
			HUART_receiveAsyncString(u16_RX_BufferSize);
		}
		(void) HUART_enEnableInterrupt(ptr_str_instance_t->enu_tx_rx_state);
	}
	else if (ptr_str_instance_t->enu_communication_sel == BCM_SPI)
	{

	}
	else if(ptr_str_instance_t->enu_communication_sel == BCM_I2C)
	{

	}
	else
	{
		/* Do nothing */
	}

	return enu_system_status;
}

/********************************************************************************************************
 ********************************************************************************************************/
/*
 * Author			: Bassel Yasser
 * Function			: bcm_deinit
 * Description		: De initialize selected communication protocol
 * Parameter [1]	: ptr_str_instance_t --> address of structer instance
 * return			: enu_system_status_t
 */

enu_system_status_t bcm_deinit(str_bcm_instance_t* ptr_str_instance_t)
{
	enu_system_status_t enu_system_status = BCM_OK;

	if (ptr_str_instance_t->enu_communication_sel == BCM_UART)
	{
		enu_system_status = HUART_enDeInit(ptr_str_instance_t->enu_tx_rx_state);

	}
	else if (ptr_str_instance_t->enu_communication_sel == BCM_SPI)
	{

	}
	else if(ptr_str_instance_t->enu_communication_sel == BCM_I2C)
	{

	}
	else
	{
		/* Do nothing */
	}

	return enu_system_status;
}
/********************************************************************************************************
 ********************************************************************************************************/

/*
 * Author			: Bassel Yasser
 * Function			: bcm_send
 * Description		: Send Data via selected communication protocol
 * Parameter [1]	: ptr_str_instance_t --> address of structer instance
 * Parameter [2] 	: u8_one_byte_data 	 --> variable of uint8
 * return			: enu_system_status_t
 */
enu_system_status_t bcm_send(str_bcm_instance_t* ptr_str_instance_t, Uint8_t u8_one_byte_data)
{
	enu_system_status_t bcm_error_status = BCM_OK;

	if (ptr_str_instance_t->enu_communication_sel ==BCM_UART )
	{
		bcm_error_status = HUART_enAsyncSendData(u8_one_byte_data);
	}
	else if (ptr_str_instance_t->enu_communication_sel == BCM_SPI)
	{

	}
	else if(ptr_str_instance_t->enu_communication_sel == BCM_I2C)
	{

	}
	else
	{
		/* Do nothing */
	}

	return bcm_error_status;
}
/********************************************************************************************************
 ********************************************************************************************************/

/*
 * Author			: Bassel Yasser
 * Function			: bcm_send_n
 * Description		: Send Group of Data via selected communication protocol
 * Parameter [1]	: ptr_str_instance_t 	 --> address of structer instance
 * Parameter [2] 	: ptr_u8_n_byte_data 	 --> Address of array or string to be sent
 * Parameter [3] 	: u16_byte_length	 	 --> Buffer size of uint16
 * return			: enu_system_status_t
 */
enu_system_status_t bcm_send_n(str_bcm_instance_t* ptr_str_instance_t, Uint8_t* ptr_u8_n_byte_data, Uint16_t u16_byte_length)
{
	enu_system_status_t bcm_error_status = BCM_OK;

	if (ptr_str_instance_t->enu_communication_sel ==BCM_UART )
	{
		HUART_sendAsyncString(ptr_u8_n_byte_data, u16_byte_length);
	}
	else if (ptr_str_instance_t->enu_communication_sel == BCM_SPI)
	{

	}
	else if(ptr_str_instance_t->enu_communication_sel == BCM_I2C)
	{

	}
	else
	{
		/* Do nothing */
	}


	return bcm_error_status;
}
/********************************************************************************************************
 ********************************************************************************************************/

/*
 * Author			: Bassel Yasser
 * Function			: bcm_dispatcher
 * Description		: is periodic function and notifies the user with neede event
 * Parameter [1]	: ptr_str_instance_t --> address of structer instance
 * return			: enu_system_status_t
 */
enu_system_status_t bcm_dispatcher(str_bcm_instance_t* ptr_str_instance_t)
{
	enu_system_status_t bcm_error_status = BCM_OK;

	if (ptr_str_instance_t->enu_communication_sel == BCM_UART )
	{
		if (gvu8_txComplete == 1)
		{
			gvu8_txComplete = 0;
			HLed_toggle(DIO_PINA_0);
		}
		else if(gvu8_rxComplete == 1)
		{
			gvu8_rxComplete = 0;
			_delay_ms(500);
			HLed_toggle(DIO_PINA_1);
			(void) bcm_send_n(&app_str_bcm_instance, "yasser", 9);
		}
		else
		{

		}

	}
	else if (ptr_str_instance_t->enu_communication_sel == BCM_SPI)
	{

	}
	else if(ptr_str_instance_t->enu_communication_sel == BCM_I2C)
	{

	}
	else
	{
		/* Do nothing */
	}
	return bcm_error_status;
}
/********************************************************************************************************
 * 											END bcm_Program.c
 ********************************************************************************************************/
