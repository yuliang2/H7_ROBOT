/**
  ******************************************************************************
  * @file           : application.c
  * @author         : XuJiang
  * @brief          : None
  * @attention      : None
  * @date           : 2025/1/14
  ******************************************************************************
  */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "stdbool.h"

#include "application.h"

osThreadId motorTaskHandle;
osThreadId buzzerTaskHandle;
osThreadId ledTaskHandle;



void OSTaskInit(void){
  osThreadDef(buzzerTask, startBuzzerTask, osPriorityNormal, 0, 128);
  buzzerTaskHandle = osThreadCreate(osThread(buzzerTask), NULL);

  osThreadDef(motortask, startMotorTask, osPriorityNormal, 0, 256);
  motorTaskHandle = osThreadCreate(osThread(motortask), NULL);

  osThreadDef(ledtask, startLEDTask, osPriorityNormal, 0, 128);
  ledTaskHandle = osThreadCreate(osThread(ledtask), NULL);

}


__attribute__((noreturn)) void startMotorTask(void const *argument) {
  static  bool finishGoal = true;
  static uint32_t startTime;
  dynamixel_init();

  uart_printf("[freeRTOS] MOTOR Task Start");
  for (;;)
  {
    dynamixel_loop();
    osDelay(10);
  }
}

__attribute__((noreturn)) void startBuzzerTask(void const *argument) {
  Buzzer_config_s buzzer_config ={
    .alarm_level = ALARM_LEVEL_HIGH, //设置警报等级 同一状态下 高等级的响应
    .loudness=  0.5, //设置响度
    .octave=  OCTAVE_4, // 设置音阶
};
  BuzzzerInstance *buzzer_alarm;
  buzzer_alarm = BuzzerRegister(&buzzer_config);

  bool needAlarm = ALARM_OFF;

  for (;;) {
    // needAlarm = !needAlarm;
    // AlarmSetStatus(buzzer_alarm, needAlarm);
    BuzzerTask();
    osDelay(1000);
  }
}

__attribute__((noreturn)) void startLEDTask(void const *argument) {
  static int green;

  for (;;) {
    green = 100-green;
    WS2812_Ctrl(0, green, 0);
    osDelay(1000);
  }
}

void app_loop_for_test(){

}
