
#define F_CPU 1000000UL  // 1 MHz

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/wdt.h>

// Wake up by WDT interrupt.

EMPTY_INTERRUPT(WDT_vect)

/*
  Delay in powerdown mode. Wake up by watchdog interrupt.
 */

void delay_power_down_wdt(uint8_t wdto) {
    
    wdt_reset();
    wdt_enable(wdto);
    WDTCSR |= (1<<WDIE);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_enable();
    // Make sure interrups are enabled and the I flag is restored
    NONATOMIC_BLOCK(NONATOMIC_RESTORESTATE)
    {
	sleep_cpu();
	wdt_disable();
    }
    sleep_disable();
}

int main(void)
{

	DDRB = 1<<1;		

    sei();

    while(1)
    {
	// Short flash
	
	PORTB |= 1<<1;		//Pin PB1
	_delay_ms(15);		
	PORTB &= ~(1<<1);	//Pin PB1
	
	delay_power_down_wdt(WDTO_8S);		// (WDTO_500MS)(WDTO_1S)(WDTO_2S)(WDTO_4S)(WDTO_8S)
    }
}
 //----------End------
