#include <atmel_start.h>
#include "hal_spi_m_os.h"

#define digitalWrite(pin, val) gpio_set_pin_level(pin, val)
#define digitalRead(pin) gpio_get_pin_level(pin)
#define pinMode(pin, val) gpio_set_pin_function(pin, val)

char spiSendReceive(char send)
{
    unsigned char res;
    uint8_t const *txbuf = (unsigned char *)&send;
    uint8_t const *rxbuf = &res;
    spi_m_os_enable(&SPI_0); // if you forget this line, this function returns -20
    int32_t bytes_read = spi_m_os_transfer(&SPI_0, &txbuf, &rxbuf, 1);
    if (bytes_read == 1)
    {
        return *(xfer.rxbuf);
    }
    else if (bytes_read == ERR_BUSY)
    {
        assert(0, "atmel_stuff.c", 42);
        return 0x00;
    }
    else
    {
        assert(0, "atmel_stuff.c", bytes_read);
        return 0x00;
    }
}