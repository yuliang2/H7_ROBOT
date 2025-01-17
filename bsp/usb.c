/**
  ******************************************************************************
  * @file           : usb.c
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/14
  ******************************************************************************
  */

#include "bsp.h"

#include <stdio.h>
#include <stdarg.h>
#include "usbd_cdc_if.h"

/**
 * @brief 通过单片机的USB接口作为虚拟串口，打印数据
 */
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
