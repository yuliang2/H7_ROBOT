//
// Created by XuJiang on 2025/1/14.
//

#ifndef BSP_H
#define BSP_H

#include "stdint.h"
#include "main.h"
#include "usart.h"

// UART
#include "usart/bsp_usart.h"

// USB
void usbvcom_printf(const char *format,...);

// BSP
void bspInit();

// PWM
#include "bsp_pwm.h"

#endif //BSP_H
