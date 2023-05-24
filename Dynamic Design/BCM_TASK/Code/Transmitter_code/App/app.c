#include <util/delay.h>
#include "../Common/std_types.h"
#include "../SERVICE/BCM/bcm_Interface.h"
#include "../HAL/LED/led.h"

#include "app.h"

extern str_bcm_instance_t app_str_bcm_instance ;

Uint8_t var = 0;

void APP_vidInit(void)
{
	HLed_Init(DIO_PIND_6);
	HLed_Init(DIO_PIND_7);


	(void) bcm_init(&app_str_bcm_instance);
	(void) bcm_send_n(&app_str_bcm_instance, "Bassel", 9);

}


void APP_vidStart(void)
{
	_delay_ms(1000);
	(void) bcm_dispatcher(&app_str_bcm_instance);


}
