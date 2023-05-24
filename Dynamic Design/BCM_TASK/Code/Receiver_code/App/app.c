#include <util/delay.h>
#include "../Common/std_types.h"
#include "../SERVICE/BCM/bcm_Interface.h"
#include "../HAL/LED/led.h"

#include "app.h"

extern str_bcm_instance_t app_str_bcm_instance ;

void APP_vidInit(void)
{

	HLed_Init(DIO_PINA_0);
	HLed_Init(DIO_PINA_1);

	(void) bcm_init(&app_str_bcm_instance);

}


void APP_vidStart(void)
{
	_delay_ms(1000);
	(void) bcm_dispatcher(&app_str_bcm_instance);
}
