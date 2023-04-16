#ifndef BSE_FSW_TEMPLATE_ATMEL_STUFF_H
#define BSE_FSW_TEMPLATE_ATMEL_STUFF_H

// If we're getting garbage, maybe reverse these?
#define INPUT GPIO_DIRECTION_IN
#define OUTPUT GPIO_DIRECTION_OUT

#define HIGH  1
#define LOW   0

#define digitalWrite(pin, val) gpio_set_pin_level(pin, val)
#define digitalRead(pin) gpio_get_pin_level(pin)
#define pinMode(pin, val) gpio_set_pin_direction(pin, val)
char spiSendReceive(char send);

#endif //BSE_FSW_TEMPLATE_ATMEL_STUFF_H