/**
  ******************************************************************************
  * @file           : modules.h
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/14
  ******************************************************************************
  */

#ifndef MODULES_H
#define MODULES_H

#include "main.h"
#include "bsp.h"

// WS2812
#define WS2812_SPI_UNIT     hspi6
extern SPI_HandleTypeDef WS2812_SPI_UNIT;
void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b);

// dynamixel motor
void dynamixel_init();
void dynamixel_loop();

// buzzer
#include "buzzer.h"

// modules
void modules_init();

#endif //MODULES_H
