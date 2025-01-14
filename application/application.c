/**
  ******************************************************************************
  * @file           : application.c
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/14
  ******************************************************************************
  */
#include "application.h"

void app_init(void){

}

void app_loop_for_test(){
  static int g;
  dynamixel_loop();
  WS2812_Ctrl(0, g, 0);
  g = (g+1)%100;
}
