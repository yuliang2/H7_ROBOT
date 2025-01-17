/**
  ******************************************************************************
  * @file           : application.h
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/14
  ******************************************************************************
  */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "modules.h"
#include "bsp.h"

#include "buzzer.h"

void OSTaskInit(void);
void app_loop_for_test();

__attribute__((noreturn)) void startMotorTask(void const *argument);
__attribute__((noreturn)) void startBuzzerTask(void const *argument);
__attribute__((noreturn)) void startLEDTask(void const *argument);
__attribute__((noreturn)) void startOrinTask(void const *argument);

#endif //APPLICATION_H
