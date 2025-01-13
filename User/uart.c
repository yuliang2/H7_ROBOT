//
// Created by XuJiang on 2025/1/13.
//
#include "user.h"
#include "string.h"
#include "usart.h"



uint8_t u1Rx[U1_RX_LEN];
uint8_t u2Rx[U2_RX_LEN];
uint8_t u3Rx[U3_RX_LEN];
uint8_t u1RxFlag = 0;
uint8_t u2RxFlag = 0;
uint8_t u3RxFlag = 0;

void robot_uart_init(){
    HAL_UART_Receive_IT(&huart1, u1Rx, U1_RX_LEN);
    HAL_UART_Receive_IT(&huart2, u2Rx, U2_RX_LEN);
    HAL_UART_Receive_IT(&huart3, u3Rx, U3_RX_LEN);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart1)
    {
        if (memcmp(u1Rx, "test\n", U1_RX_LEN) == 0)
        {
            u1RxFlag = 1;
        }
        HAL_UART_Receive_IT(&huart1, u1Rx, U1_RX_LEN);
    }
    if (huart == &huart2)
    {
        if (memcmp(u2Rx, "U3TOU2", U2_RX_LEN) == 0)
        {
            u2RxFlag = 1;
        }
        HAL_UART_Receive_IT(&huart2, u2Rx, U2_RX_LEN);
    }
    else if (huart == &huart3)
    {
        if (memcmp(u3Rx, "U2TOU3", U3_RX_LEN) == 0)
        {
            u3RxFlag = 1;
        }
        HAL_UART_Receive_IT(&huart3, u3Rx, U3_RX_LEN);
    }
}

