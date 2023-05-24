/*
 * MUART_Source.c
 *
 *  Created on: Apr 24, 2023
 *      Author: hp
 */
/********************************************************************************************************/
/*									Standard Types LIB													*/
/********************************************************************************************************/
#include <avr/interrupt.h>
#include <string.h>
#include <stdlib.h>
//#include "../../Common/std_types.h"
#include "../../Common/Bit_Math.h"
/********************************************************************************************************/
/*									Peripheral Files 													*/
/********************************************************************************************************/
#include "MUART_Private.h"
#include "MUART_Interface.h"
#include "MUART_Config.h"
/********************************************************************************************************/
/*										Macros		     												*/
/********************************************************************************************************/
#define F_CPU		8000000UL

/********************************************************************************************************/
/*										Global Variables and extern		     							*/
/********************************************************************************************************/
volatile Uint16_t g_u16_char_size = 0;
volatile Uint16_t g_u16_rx_char_size = 0;
Uint8_t g_p_u8_string[30];
volatile Uint8_t g_p_u8_rx_string_buffer[30];
Uint8_t g_u8_stringCounter = 0;
volatile Uint8_t gvu8_txComplete = 0, gvu8_rxComplete = 0;

extern st_muart_t uartInfo;

/********************************************************************************************************/
/*									Function Implementation			     								*/
/********************************************************************************************************/
en_uartErrStat_t MUART_enInit(Uint32_t copy_u32BaudRate)
{
	Uint8_t local_u8BitMask;
	en_uartErrStat_t local_enErrorStatus = MUART_OK;
	Uint16_t local_u16UBRRReg ;

	if(copy_u32BaudRate >= 2400 && copy_u32BaudRate <= 250000)
	{

		/******************************** Set UART speed and setting Baudrate **************************/

#if MUART_SPEED_TYPE == MUART_SINGLE_SPEED

		ClrBit(UCSRA_REG, U2X);
		local_u16UBRRReg = ((F_CPU / (16*copy_u32BaudRate)) - 1);
		UBRRL_REG = (Uint8_t)local_u16UBRRReg;
		UBRRH_REG = (Uint8_t)((0 << URSEL) | (local_u16UBRRReg >> 8) );

#elif MUART_SPEED_TYPE == MUART_DOUBLE_SPEED

		SetBit(UCSRA_REG, U2X);
		local_u16UBRRReg = (F_CPU / (8*copy_u32BaudRate)) - 1;
		UBRRL_REG = (Uint8_t)local_u16UBRRReg;
		UBRRH_REG = (Uint8_t)((0 << URSEL) | (local_u16UBRRReg >> 8) );

#else

#error "Please put correct baudRate Speed"

#endif

		/******************************** Enabling TX and RX **************************/

//#if MUART_TX_RX == MUART_TX_ENABLE
//
//		SetBit(UCSRB_REG, TXEN);
//
//#elif MUART_TX_RX == MUART_RX_ENABLE
//
//		SetBit(UCSRB_REG, RXEN);
//
//#elif MUART_TX_RX == MUART_TX_RX_ENABLE
//
//		SetBit(UCSRB_REG, TXEN);
//		SetBit(UCSRB_REG, RXEN);
//
//#else
//#error "MUART_TX_RX_INVALID"
//#endif

		/******************************** Selecting Parity type **************************/
		UCSRC_REG &= 0b11001111;
		UCSRC_REG |= (1 << URSEL) | (uartInfo.enParityType << 4);


		/********************** Selecting Data Length ************************************/
		UCSRC_REG &= 0b11111001;
		local_u8BitMask = (0b00000011 | uartInfo.enDataLength);
		UCSRC_REG |= (1 << URSEL) | (1 << local_u8BitMask);
		local_u8BitMask = ( (0b00000100 | uartInfo.enDataLength) >> 2);
		UCSRB_REG &= 0b11111011;
		UCSRB_REG |= (local_u8BitMask << 2);

		/********************** Selecting Stop bit mode ************************************/
#if MUART_STOP_BIT == MUART_1_STOP_BIT

		ClrBit(UCSRC_REG, RUSBS);

#elif MUART_STOP_BIT == MUART_2_STOP_BIT

		SetBit(UCSRC_REG, RUSBS);
#endif

		/********************** Asynchronous mode  ************************************/
		ClrBit(UCSRC_REG, UMSEL);


	}else
	{
		local_enErrorStatus = MUART_NOK;
	}
	return local_enErrorStatus;
}



/*
 * Author		: Bassel Yasser Mahmoud
 * Function		: MUART_en_TX_Enable
 * Description	: Transmitter is enabled
 * in[1]		: void
 * Return		: void
 */
void MUART_en_TX_Enable(void)
{
	SetBit(UCSRB_REG, TXEN);
}

/*
 * Author		: Bassel Yasser Mahmoud
 * Function		: MUART_en_RX_Enable
 * Description	: Receiver is enabled
 * in[1]		: void
 * Return		: void
 */
void MUART_en_RX_Enable(void)
{
	SetBit(UCSRB_REG, RXEN);
}

/*
 * Author		: Bassel Yasser Mahmoud
 * Function		: MUART_en_TX_Disable
 * Description	: transmitter is disabled
 * in[1]		: void
 * Return		: void
 */
void MUART_en_TX_Disable(void)
{
	ClrBit(UCSRB_REG, TXEN);
}

/*
 * Author		: Bassel Yasser Mahmoud
 * Function		: MUART_en_RX_Disable
 * Description	: Receiver is disabled
 * in[1]		: void
 * Return		: void
 */
void MUART_en_RX_Disable(void)
{
	ClrBit(UCSRB_REG, RXEN);
}


/*
 * Author		: Bassel Yasser Mahmoud
 * Function		: Receiver and transmitter are enabled
 * Description	: UART Initialization
 * in[1]		: void
 * Return		: void
 */
void MUART_en_TX_RX_Enable(void)
{
	SetBit(UCSRB_REG, TXEN);
	SetBit(UCSRB_REG, RXEN);
}

/*
 * Author		: Bassel Yasser Mahmoud
 * Function		: MUART_en_TX_RX_Disable
 * Description	: Receiver and transmitter are disabled
 * in[1]		: void
 * Return		: void
 */
void MUART_en_TX_RX_Disable(void)
{
	ClrBit(UCSRB_REG, TXEN);
	ClrBit(UCSRB_REG, RXEN);
}


en_uartErrStat_t MUART_enSyncSendData(Uint8_t Copy_u8Data)
{
	en_uartErrStat_t local_enErrorStatus = MUART_OK;

//	if (Copy_u8Data != 0)
//	{
		while( !(GetBit(UCSRA_REG, UDRE)) );
		UDR_REG = Copy_u8Data;
		while( !(GetBit(UCSRA_REG, TXC)) );

//	}else
//	{
//		local_enErrorStatus = MUART_NOK;
//	}
	return local_enErrorStatus;
}



/*
 * Author		: Bassel Yasser Mahmoud
 * Function		: MUART_enAsyncSendData
 * Description	: UART transmit data to receiver (Asynchronous function)
 * in[1]		: Copy_u8Data (Data to be sent)
 * Return		: en_uartErrStat_t {MUART_OK, MUART_NOK}
 */
en_uartErrStat_t MUART_enAsyncSendData(Uint8_t Copy_u8Data)
{
	en_uartErrStat_t local_enErrorStatus = MUART_OK;
	g_u16_char_size = 0;
	UDR_REG = Copy_u8Data;

	return local_enErrorStatus;
}

/*
 * Author		: Bassel Yasser Mahmoud
 * Function		: MUART_enAsyncSendData
 * Description	: UART transmit data to receiver (Asynchronous function)
 * in[1]		: Copy_u8Data (Data to be sent)
 * Return		: en_uartErrStat_t {MUART_OK, MUART_NOK}
 */
en_uartErrStat_t MUART_enEnableInterrupt(en_muart_interrupt_t en_muart_interrupt)
{
	en_uartErrStat_t errorStat = MUART_OK;
	if (en_muart_interrupt < MUART_INTERRUPT_INVALID)
	{
		SetBit(UCSRB_REG, en_muart_interrupt);
//		SetBit(UCSRA_REG, en_muart_interrupt);
	}
	else
	{
		errorStat = MUART_NOK;
	}

	return errorStat;
}

/*
 * Author		: Bassel Yasser Mahmoud
 * Function		: MUART_enAsyncSendData
 * Description	: UART transmit data to receiver (Asynchronous function)
 * in[1]		: Copy_u8Data (Data to be sent)
 * Return		: en_uartErrStat_t {MUART_OK, MUART_NOK}
 */
en_uartErrStat_t MUART_enDisableInterrupt(en_muart_interrupt_t en_muart_interrupt)
{
	en_uartErrStat_t errorStat = MUART_OK;
	if (en_muart_interrupt < MUART_INTERRUPT_INVALID)
	{
		ClrBit(UCSRB_REG, en_muart_interrupt);
	}
	else
	{
		errorStat = MUART_NOK;
	}

	return errorStat;
}



en_uartErrStat_t MUART_enRecieveData(Uint8_t* Ref_u8Data)
{
	en_uartErrStat_t local_enErrorStatus = MUART_OK;

	if (Ref_u8Data != NULL)
	{
		while( !(GetBit(UCSRA_REG, RXC)) );
		*Ref_u8Data= UDR_REG;

	}
	else
	{
		local_enErrorStatus = MUART_NOK;
	}
	return local_enErrorStatus;
}


void MUART_sendSyncString(Uint8_t * str, Uint8_t u8_arr_size)
{
//	Uint8_t COUNTER = 0;

	for (Uint8_t count = 0; count < u8_arr_size; count++)
	{
		(void)MUART_enSyncSendData(str[count]);
	}

//	while(str[COUNTER] != '\0')
//	{
//		(void)MUART_enSyncSendData(str[COUNTER]);
//		COUNTER++;
//	}
}




void MUART_sendAsyncString(Uint8_t * str , Uint16_t u16_arr_size)
{
	g_u16_char_size = u16_arr_size;
	strcpy(g_p_u8_string, str);
	UDR_REG = g_p_u8_string[g_u8_stringCounter];
}


void MUART_receiveAsyncString(Uint16_t u16_arr_size)
{
	g_u16_rx_char_size = u16_arr_size;

//	g_u8_rx_char_size = 8;

}

void MUART_receiveSTRING(Uint8_t * str ,Uint8_t size )
{
	Uint8_t COUNTER = 0;
	Uint8_t Data;
	while(COUNTER < size-1)
	{
		(void)MUART_enRecieveData(&Data);

		str[COUNTER]= Data;
		if(str[COUNTER] == ENTER_KEY)break;
//		(void)MUART_enSendData(str[COUNTER]);
		COUNTER++;
	}
	str[COUNTER] = '\0';
}




/********************************************************************************************************/
/*													ISR	     										*/
/********************************************************************************************************/

ISR(USART_TXC_vect)
{
	static Uint16_t u16_size_counter = 1;


	if(u16_size_counter <= g_u16_char_size  && g_p_u8_string[u16_size_counter] != '\0' )
	{
		UDR_REG = g_p_u8_string[u16_size_counter];
		u16_size_counter++;
	}
	else
	{
		gvu8_txComplete = 1;
//		ClrBit(UCSRB_REG, 6);
		u16_size_counter = 1 ;
//		g_u8_stringCounter = 0;

	}
}


ISR(USART_RXC_vect)
{
	static Uint8_t counter = 0;
//	static Uint16_t g_u16_rx_char_size = 2;
	if(counter < g_u16_rx_char_size-1)
	{
		g_p_u8_rx_string_buffer[counter] = UDR_REG;
		counter++;
	}
	else
	{
		g_p_u8_rx_string_buffer[counter] = '\0' ;
		counter = 0;
		gvu8_rxComplete = 1;
	}

}


/********************************************************************************************************/
/*													END		     										*/
/********************************************************************************************************/
