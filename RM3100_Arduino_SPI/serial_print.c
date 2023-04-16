#include "serial_print.h"

// struct io_descriptor *io;

// void s_print_init(){
// 	usart_sync_get_io_descriptor(&USART_0, &io);
// 	usart_sync_enable(&USART_0);
// }

void s_print(char* buf)
{
    char* s;
    for(s=buf; *s; ++s);
    unsigned int size = (unsigned int)(s - buf);

	io_write(io, (uint8_t *)buf, size);
}

void reverse(char str[], int length) 
{ 
    int start = 0;
    char str_b[length];
    for (int i = length-1; i >= 0; i--){
        str_b[start] = str[i];
        start++;
    }

    for (int i = 0; i < length; i++){
        str[i] = str_b[i];
        start++;
    }

} 

char* itoa(int num, char* str, int base) 
{ 
    int i = 0; 
    bool isNegative = false; 
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0) 
    { 
        str[i++] = '0'; 
        str[i] = '\0'; 
        return str; 
    } 
  
    // In standard itoa(), negative numbers are handled only with  
    // base 10. Otherwise numbers are considered unsigned. 
    if (num < 0 && base == 10) 
    { 
        isNegative = true; 
        num = -num; 
    } 
  
    // Process individual digits 
    while (num != 0) 
    { 
        int rem = num % base; 
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0'; 
        num = num/base; 
    } 
  
    // If number is negative, append '-' 
    if (isNegative) 
        str[i++] = '-'; 
  
    str[i] = '\0'; // Append string terminator 
  
    // Reverse the string 
    reverse(str, i); 

    return str;
} 