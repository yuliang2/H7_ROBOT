//
// Created by XuJiang on 2025/1/13.
//
#include "string.h"
#include "usart.h"
#include <stdio.h>
#include <stdarg.h>
#include "bsp.h"

uint8_t u1Rx[U1_RX_LEN];
uint8_t u2Rx[U2_RX_LEN];
uint8_t u3Rx[U3_RX_LEN];
uint8_t u1RxFlag = 0;
uint8_t u2RxFlag = 0;
uint8_t u3RxFlag = 0;

void robot_uart_init(){
    // now: nothing to do
}


void uart_printf(const char *format,...)
{
    uint8_t usbtemp[64];
    uint16_t     len;
    va_list     args;

    va_start(args, format);
    len = vsnprintf((char *)usbtemp, sizeof(usbtemp)+1, (char *)format, args);
    va_end(args);

    HAL_UART_Transmit(&huart1, (uint8_t *)usbtemp, len, 100);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        u1RxFlag = 1;
        HAL_UART_Receive_IT(&huart1, u1Rx, U1_RX_LEN);
    }
    if (huart == &huart2)
    {
        u2RxFlag = 1;
        HAL_UART_Receive_IT(&huart2, u2Rx, U2_RX_LEN);
    }
    else if (huart == &huart3)
    {
        u3RxFlag = 1;
        HAL_UART_Receive_IT(&huart3, u3Rx, U3_RX_LEN);
    }
}

