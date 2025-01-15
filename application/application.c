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
extern PWMInstance *buzzer;

BuzzzerInstance *buzzer_alarm;

void app_init(void){
  Buzzer_config_s buzzer_config ={
    .alarm_level = ALARM_LEVEL_HIGH, //设置警报等级 同一状态下 高等级的响应
    .loudness=  0.5, //设置响度
    .octave=  OCTAVE_4, // 设置音阶
};
  buzzer_alarm = BuzzerRegister(&buzzer_config);
  AlarmSetStatus(buzzer_alarm, ALARM_ON);
  BuzzerTask();
}

void app_loop_for_test(){
  static int g;
  dynamixel_loop();


  if (HAL_GetTick()>1000) {
    AlarmSetStatus(buzzer_alarm, ALARM_OFF);
    BuzzerTask();
  }

  WS2812_Ctrl(0, g, 0);
  g = (g+1)%100;
}
