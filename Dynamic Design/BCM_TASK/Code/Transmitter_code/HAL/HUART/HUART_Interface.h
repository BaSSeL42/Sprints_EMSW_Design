/*
 * HUART_Interface.h
 *
 *  Created on: Apr 25, 2023
 *      Author: hp
 */

/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											Start huart_Interface
 ********************************************************************************************************/
/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											File Gurad
 ********************************************************************************************************/
#ifndef HAL_HUART_HUART_INTERFACE_H_
#define HAL_HUART_HUART_INTERFACE_H_


/********************************************************************************************************
 * 											User Defined Data type
 ********************************************************************************************************/
typedef enum
{
	HUART_OK = 0,
	HUART_NOK

}en_huartErrStat_t;


typedef enum
{
	HUART_TX = 0,
	HUART_RX,
	HUART_TX_RX,
	HUART_STATE_INVALID

}en_huart_tx_rx_sel_t;




/********************************************************************************************************
 * 											Function prototype
 ********************************************************************************************************/
/*
 *
 */
en_huartErrStat_t HUART_enInit(Uint32_t copy_u32BaudRateH, en_huart_tx_rx_sel_t en_huart_tx_rx_sel);

/*
 *
 */
en_huartErrStat_t HUART_enDeInit(en_huart_tx_rx_sel_t en_huart_tx_rx_sel);
/*
 *
 */
en_huartErrStat_t HUART_enSyncSendData(Uint8_t Copy_u8DataH);

/*
 *
 */
en_huartErrStat_t HUART_enAsyncSendData(Uint8_t Copy_u8DataH);

/*
 *
 */
en_huartErrStat_t HUART_enRecieveData(Uint8_t* Ref_u8DataH);

/*
 *
 */
void HUART_sendSyncString(Uint8_t * Hstr, Uint8_t u8_arr_size);


/**/
void HUART_sendAsyncString(Uint8_t * Hstr, Uint16_t u16_arr_size);
/**/
void HUART_receiveAsyncString(Uint16_t u16_arr_size);

/*
 *
 */
void HUART_receiveSTRING(Uint8_t * Hstr ,Uint8_t Hsize );

/**/
en_huartErrStat_t HUART_enEnableInterrupt(en_huart_tx_rx_sel_t en_huart_tx_rx_sel);


#endif /* HAL_HUART_HUART_INTERFACE_H_ */
/********************************************************************************************************
 ********************************************************************************************************/
/********************************************************************************************************
 * 											End huart_Interface
 ********************************************************************************************************/
/********************************************************************************************************
 ********************************************************************************************************/
