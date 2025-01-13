//
// Created by XuJiang on 2025/1/13.
//

#ifndef USER_H
#define USER_H

#include "main.h"
#include "usart.h"

#define U1_RX_LEN   5
#define U2_RX_LEN   6
#define U3_RX_LEN   6

extern uint8_t u1Rx[], u2Rx[], u3Rx[];
extern uint8_t u1RxFlag, u2RxFlag, u3RxFlag;
void robot_init();
void robot_loop();
void usbvcom_printf(const char *format,...);

#define WS2812_SPI_UNIT     hspi6
extern SPI_HandleTypeDef WS2812_SPI_UNIT;

void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b);

#endif //USER_H
