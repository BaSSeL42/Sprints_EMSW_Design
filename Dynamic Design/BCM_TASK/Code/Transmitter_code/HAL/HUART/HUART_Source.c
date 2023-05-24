/*
 * HUART_Source.c
 *
 *  Created on: Apr 25, 2023
 *      Author: hp
 */
/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											Start huart_Source.c
 ********************************************************************************************************/
/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											Includes
 ********************************************************************************************************/
#include "../../Common/std_types.h"
#include "../../Common/Bit_Math.h"
#include "../../MCAL/UART/MUART_Interface.h"
#include "../../MCAL/DIO/DIO_Interface.h"
#include "HUART_Interface.h"

/********************************************************************************************************
 * 											Global & Static Variables
 ********************************************************************************************************/

/********************************************************************************************************
 * 											Function Implmentation
 ********************************************************************************************************/
en_huartErrStat_t HUART_enInit(Uint32_t copy_u32BaudRateH,en_huart_tx_rx_sel_t en_huart_tx_rx_sel)
{

	en_huartErrStat_t local_enErrorStatus = HUART_OK;

	local_enErrorStatus = MUART_enInit(copy_u32BaudRateH);

	if (en_huart_tx_rx_sel < HUART_STATE_INVALID)
	{
		switch(en_huart_tx_rx_sel)
		{
			case HUART_TX 			:	MUART_en_TX_Enable(); 		break;
			case HUART_RX			:	MUART_en_RX_Enable(); 		break;
			case HUART_TX_RX		:	MUART_en_TX_RX_Enable(); 	break;
			default					: break;
		}
	}
	else
	{
		local_enErrorStatus = HUART_NOK;
	}


	return local_enErrorStatus;
}


en_huartErrStat_t HUART_enEnableInterrupt(en_huart_tx_rx_sel_t en_huart_tx_rx_sel)
{
	en_huartErrStat_t local_enErrorStatus = HUART_OK;

	if (en_huart_tx_rx_sel < HUART_STATE_INVALID)
	{
		switch(en_huart_tx_rx_sel)
		{
			case HUART_TX 			:	MUART_enEnableInterrupt(MUART_TX_INTERRUPT);		break;
			case HUART_RX			:	MUART_enEnableInterrupt(MUART_RX_INTERRUPT); 		break;
			case HUART_TX_RX		:	MUART_enEnableInterrupt(MUART_TX_INTERRUPT); MUART_enEnableInterrupt(MUART_RX_INTERRUPT);	break;
			default					: break;
		}
		GLI();
	}
	else
	{
		local_enErrorStatus = HUART_NOK;
	}


	return local_enErrorStatus;

}



en_huartErrStat_t HUART_enDeInit(en_huart_tx_rx_sel_t en_huart_tx_rx_sel)
{
	en_huartErrStat_t local_enErrorStatus = HUART_OK;

	if (en_huart_tx_rx_sel < HUART_STATE_INVALID)
	{
		switch(en_huart_tx_rx_sel)
		{
			case HUART_TX 			:	MUART_en_TX_Disable(); 		break;
			case HUART_RX			:	MUART_en_RX_Disable(); 		break;
			case HUART_TX_RX		:	MUART_en_TX_RX_Disable(); 	break;
			default					: break;
		}
	}
	else
	{
		local_enErrorStatus = HUART_NOK;
	}

	return local_enErrorStatus;
}



en_huartErrStat_t HUART_enSyncSendData(Uint8_t Copy_u8DataH)
{
	en_huartErrStat_t local_enErrorStatus = HUART_OK;

	(void) MUART_enSyncSendData(Copy_u8DataH);

	return local_enErrorStatus;
}



en_huartErrStat_t HUART_enAsyncSendData(Uint8_t Copy_u8DataH)
{
	en_huartErrStat_t errorState = HUART_OK;

	(void) MUART_enAsyncSendData(Copy_u8DataH);

	return errorState;
}



en_huartErrStat_t HUART_enRecieveData(Uint8_t* Ref_u8DataH)
{
	en_huartErrStat_t local_enErrorStatus = HUART_OK;

	local_enErrorStatus = MUART_enRecieveData(Ref_u8DataH);

	return local_enErrorStatus;
}


void HUART_sendSyncString(Uint8_t * Hstr, Uint8_t u8_arr_size)
{
	MUART_sendSyncString(Hstr, u8_arr_size);
}

void HUART_sendAsyncString(Uint8_t * Hstr, Uint16_t u16_arr_size)
{
	MUART_sendAsyncString(Hstr, u16_arr_size);

	(void) MUART_enEnableInterrupt(MUART_TX_INTERRUPT);
}


void HUART_receiveAsyncString(Uint16_t u16_arr_size)
{
//	g_s8_rx_char_size = s8_arr_size;

	MUART_receiveAsyncString(u16_arr_size);

}

void HUART_receiveSTRING(Uint8_t * Hstr ,Uint8_t Hsize )
{
	MUART_receiveSTRING(Hstr ,Hsize );
}


/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											End huart_Source.c
 ********************************************************************************************************/
/********************************************************************************************************
 ********************************************************************************************************/



