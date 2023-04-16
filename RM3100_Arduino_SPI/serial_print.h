#include "driver_init.h"
#include "utils.h"
#include <hal_io.h>
#include "serial_read.h"

void s_print(char* buf);

// void s_print_init();

char* itoa(int num, char* str, int base);