//
// Created by XuJiang on 2025/1/13.
//
#include "user.h"

uint8_t r = 0;
uint8_t g = 1;
uint8_t b = 0;

void robot_init(){
    HAL_Delay(1000);
    usbvcom_printf("Program Started!\r\n");
    uart_printf("Program Started!\r\n");
    dynamixel_init();
    usbvcom_printf("Init OK!\r\n");
    uart_printf("Init OK!\r\n");
    // robot_uart_init();
}

void robot_loop(){
    // if (u1RxFlag == 1)
    // {
    //     HAL_UART_Transmit(&huart1, "U2 send info to U3!\n", sizeof("U2 send info to U3!\n")-1, 0xFFFF);
    //     usbvcom_printf("U2 send info to U3!\n");
    //     HAL_UART_Transmit(&huart2, "U2TOU3", 6, 0xFFFF);
    //     u1RxFlag = 0;
    // }
    // if (u2RxFlag == 1)
    // {
    //     HAL_UART_Transmit(&huart1, "U2 get info!\n", sizeof("U2 get info!\n")-1, 0xFFFF);
    //     HAL_UART_Transmit(&huart1, "RS485 test pass!\n", sizeof("RS485 test pass!\n")-1, 0xFFFF);
    //     usbvcom_printf("U2 get info!\n");
    //     usbvcom_printf("RS485 test pass!\n");
    //     u2RxFlag = 0;
    // }
    // if (u3RxFlag == 1)
    // {
    //     HAL_UART_Transmit(&huart1, "U3 get info!\n", sizeof("U3 get info!\n")-1, 0xFFFF);
    //     HAL_UART_Transmit(&huart1, "U3 send info to U2!\n", sizeof("U3 send info to U2!\n")-1, 0xFFFF);
    //     usbvcom_printf("U3 get info!\n");
    //     usbvcom_printf("U3 send info to U2!\n");
    //     HAL_UART_Transmit(&huart3, "U3TOU2", 6, 0xFFFF);
    //     u3RxFlag = 0;
    // }
    dynamixel_loop();
    WS2812_Ctrl(r, g, b);
    g = 30-g;
    HAL_Delay(100);
}
