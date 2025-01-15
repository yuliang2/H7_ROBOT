//
// Created by XuJiang on 2025/1/14.
//

#ifndef BSP_H
#define BSP_H

#include "stdint.h"
#include "main.h"
#include "usart.h"

// UART
#define U1_RX_LEN   5
#define U2_RX_LEN   6
#define U3_RX_LEN   6
extern uint8_t u1Rx[], u2Rx[], u3Rx[];
extern uint8_t u1RxFlag, u2RxFlag, u3RxFlag;
void robot_uart_init();
void uart_printf(const char *format,...);

// USB
void usbvcom_printf(const char *format,...);

// BSP
void bspInit();

// PWM
#include "bsp_pwm.h"

#endif //BSP_H
