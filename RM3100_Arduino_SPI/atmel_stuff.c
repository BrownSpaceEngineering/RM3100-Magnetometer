#include <atmel_start.h>

#define digitalWrite(pin, val) gpio_set_pin_level(pin, val)
#define digitalRead(pin) gpio_get_pin_level(pin)
#define pinMode(pin, val) gpio_set_pin_function(pin, val)

char spiSendReceive(char send) {
    /* APPROACH 1 */
//    struct io_descriptor *io;
//    spi_m_sync_get_io_descriptor(&SPI_0, &io);
//    spi_m_sync_enable(&SPI_0);
//    // The Arducam code does this manually - see CS_LOW and CS_HIGH
//    // In the future, do we want to do it here instead?
//    /* Control the slave select (SS) pin */
////    gpio_set_pin_level(SPI_0_SS, false);
////    unsigned char send_str[2] = {send, '\0'};  // Arducam uses chars, but Atmel uses strings
//    io_write(io, (unsigned char *) &send, 1);
//
//    char data;
//    if (io_read(io, (uint8_t *)&data, 1) == 1) {
//        /* read OK, handle data. */;
//        return data;
//    } else {
//        // Error - in real life, actually handle this, but for now...
//        assert(0, "atmel_stuff.c", 25);
//
//        return 0x00;
//    }
    /* Control the slave select (SS) pin */
//    gpio_set_pin_level(SPI_0_SS, true);
// END APPROACH 1



// APPROACH 2 (would still need chip-select stuff)
    unsigned char res;
    struct spi_xfer xfer;
    xfer.size = 1;
    xfer.txbuf = (unsigned char*) &send;
    xfer.rxbuf = &res;
    spi_m_sync_enable(&SPI_0);  // if you forget this line, this function returns -20
    int32_t bytes_read = spi_m_sync_transfer(&SPI_0, &xfer);
    if (bytes_read == 1) {
        return *(xfer.rxbuf);
    } else if (bytes_read == ERR_BUSY) {
        assert(0, "atmel_stuff.c", 42);
        return 0x00;
    } else {
        assert(0, "atmel_stuff.c", bytes_read);
        return 0x00;
    }
}