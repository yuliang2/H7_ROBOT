#include "user.h"

#include <stdio.h>
#include <stdarg.h>
#include "usbd_cdc_if.h"

void usbvcom_printf(const char *format,...)
{
    uint8_t usbtemp[64];
    uint16_t     len;
    va_list     args;

    va_start(args, format);
    len = vsnprintf((char *)usbtemp, sizeof(usbtemp)+1, (char *)format, args);
    va_end(args);

    CDC_Transmit_HS(usbtemp, len);
}
