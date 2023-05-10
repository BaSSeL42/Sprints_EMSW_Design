/*
 * MUART_Source.c
 *
 *  Created on: Apr 24, 2023
 *      Author: hp
 */
/********************************************************************************************************/
/*									Standard Types LIB													*/
/********************************************************************************************************/
#include "../../Common/std_types.h"
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
/*									Function Implementation			     								*/
/********************************************************************************************************/
en_uartErrStat_t MUART_enInit(Uint32_t copy_u32BaudRate)
{
	en_uartErrStat_t local_enErrorStatus = MUART_OK;
	Uint16_t local_u16UBRRReg ;

	if(copy_u32BaudRate >= 2400 && copy_u32BaudRate <= 250000)
	{
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

#if MUART_TX_RX == MUART_TX_ENABLE

		SetBit(UCSRB_REG, TXEN);

#elif MUART_TX_RX == MUART_RX_ENABLE

		SetBit(UCSRB_REG, RXEN);

#elif MUART_TX_RX == MUART_TX_RX_ENABLE

		SetBit(UCSRB_REG, TXEN);
		SetBit(UCSRB_REG, RXEN);

#else
#error "MUART_TX_RX_INVALID"
#endif


#if MUART_PARITY_TYPE == MUART_NO_PARITY

		UCSRC_REG &= 0b11001111;

#elif MUART_PARITY_TYPE == MUART_EVEN_PARITY

		UCSRC_REG &= 0b11001111;
		UCSRC_REG |= (1 << URSEL) | (2 << 4);

//		ClrBit(UCSRB_REG, RUPM0);
//		SetBit(UCSRC_REG, RUPM1);

#elif MUART_PARITY_TYPE == MUART_ODD_PARITY

		UCSRC_REG &= 0b11001111;
		UCSRC_REG |= (1 << URSEL) | (3 << 4);

//		SetBit(UCSRB_REG, RUPM0);
//		SetBit(UCSRC_REG, RUPM1);

#endif



#if MUART_DATA_LENGTH == MUART_5_BIT_DATA

		UCSRC_REG |= (1 << URSEL) ;
		UCSRC_REG &= 0b11111001;
		UCSRB_REG &= ~(1 << UCSZ2);


#elif MUART_DATA_LENGTH == MUART_6_BIT_DATA

		UCSRC_REG &= 0b11111001;
		UCSRC_REG |= (1 << URSEL) | (1 << UCSZ0) ;
		UCSRB_REG &= ~(1 << UCSZ2);

#elif MUART_DATA_LENGTH == MUART_7_BIT_DATA

		UCSRC_REG &= 0b11111001;
		UCSRC_REG |= (1 << URSEL) | (1 << UCSZ1) ;
		UCSRB_REG &= ~(1 << UCSZ2);

#elif MUART_DATA_LENGTH == MUART_8_BIT_DATA

		UCSRC_REG &= 0b11111001;
		UCSRC_REG |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1) ;
		UCSRB_REG &= ~(1 << UCSZ2);


#elif MUART_DATA_LENGTH == MUART_9_BIT_DATA

		UCSRC_REG &= 0b11111001;
		UCSRC_REG |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1) ;
		UCSRB_REG |= (1 << UCSZ2);

#endif

#if MUART_STOP_BIT == MUART_1_STOP_BIT

		ClrBit(UCSRC_REG, RUSBS);

#elif MUART_STOP_BIT == MUART_2_STOP_BIT

		SetBit(UCSRC_REG, RUSBS);
#endif

		/* Asynchronous mode */
		ClrBit(UCSRC_REG, UMSEL);

		/* Set 8-bit data */
//		SetBit(UCSRC_REG, URSEL);
//		SetBit(UCSRC_REG, UCSZ0);
//		SetBit(UCSRC_REG, UCSZ1);
//		ClrBit(UCSRB_REG, UCSZ2);

		/* Set 8-bit data, Asynchronous mode and One Stop bit */
//		UCSRC_REG = (1 << URSEL) | (3 << UCSZ0);

		/* One stop bit */
//		ClrBit(UCSRC_REG, USBS);
	}else
	{
		local_enErrorStatus = MUART_NOK;
	}
	return local_enErrorStatus;
}



en_uartErrStat_t MUART_enSendData(Uint8_t Copy_u8Data)
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


en_uartErrStat_t MUART_enRecieveData(Uint8_t* Ref_u8Data)
{
	en_uartErrStat_t local_enErrorStatus = MUART_OK;

	if (Ref_u8Data != NULL)
	{
		while( !(GetBit(UCSRA_REG, RXC)) );
		*Ref_u8Data= UDR_REG;

	}else
	{
		local_enErrorStatus = MUART_NOK;
	}
	return local_enErrorStatus;
}


void MUART_sendSTRING(Uint8_t * str)
{
	Uint8_t COUNTER = 0;
	while(str[COUNTER] != NULL)
	{
		(void)MUART_enSendData(str[COUNTER]);
		COUNTER++;
	}
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
	str[COUNTER] = NULL;
}

/********************************************************************************************************/
/*													END		     										*/
/********************************************************************************************************/
