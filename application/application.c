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
osThreadId orinTaskHandle;

/**
 * @brief freeRTOS的任务初始化函数
 * @note 在main函数的初始化中使用
 */
void OSTaskInit(void){
  osThreadDef(buzzerTask, startBuzzerTask, osPriorityNormal, 0, 128);
  buzzerTaskHandle = osThreadCreate(osThread(buzzerTask), NULL);

  osThreadDef(motortask, startMotorTask, osPriorityNormal, 0, 256);
  motorTaskHandle = osThreadCreate(osThread(motortask), NULL);

  osThreadDef(ledtask, startLEDTask, osPriorityNormal, 0, 128);
  ledTaskHandle = osThreadCreate(osThread(ledtask), NULL);

  osThreadDef(orintask, startOrinTask, osPriorityNormal, 0, 128);
  orinTaskHandle = osThreadCreate(osThread(orintask), NULL);

}


/**
 * @brief 电机任务
 */
__attribute__((noreturn)) void startMotorTask(void const *argument) {
  static  bool finishGoal = true;
  static uint32_t startTime;

  uart_printf("[freeRTOS] MOTOR Task Start\r\n");
  osDelay(500);
  motor_init();
  uart_printf("[FreeRTOS] MOTOR Task Running\r\n");
  // dynamixel_write_goal_position(&motorInstance[decoded_results->keys[3]], decoded_results->values[i]);
  for (;;)
  {
    for (int i = 0; i < MAX_KEYS; i++) {
      int motorNum = i;
      int motorTarget = motorInstance[i].target_position;

      if (dynamixel_read_present_position(&motorInstance[motorNum])) {
        osDelay(10);
        int motorPresentPosition = motorInstance[motorNum].present_position;
        if (abs(motorTarget - motorPresentPosition) > DXL_MOVING_STATUS_THRESHOLD) {
          dynamixel_write_goal_position(&motorInstance[motorNum], motorInstance[i].target_position);
        }
      }
      osDelay(20);
    }
    osDelay(100);
  }
}

/**
 * @brief 蜂鸣器任务
 */
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

/**
 * @brief LED任务
 */
__attribute__((noreturn)) void startLEDTask(void const *argument) {
  static int green;

  for (;;) {
    green = 100-green;
    WS2812_Ctrl(0, green, 0);
    osDelay(1000);
  }
}

/**
 * @brief 和Orin NX上位机通信的任务
 */
__attribute__((noreturn)) void startOrinTask(void const *argument) {
  CommandResults commandToSend;
  talk_with_jetson_init();
  for (;;) {
    // if (decoded_results) {
    //   uart_printf("Decoded Results:\n");
    //   for (int i = 0; i < MAX_KEYS; i++) {
    //     uart_printf("%c: %d\t", decoded_results->keys[i], decoded_results->values[i]);
    //     uart_printf("\r\n");
    //   }
    // }
    if (needToSendToOrin) {
      needToSendToOrin = false;

      for (int i = 0; i < MAX_KEYS; i++) {
        commandToSend.keys[i] = 'A'+i;
        commandToSend.values[i] = motorInstance[i].present_position;
      }
      char* encoded_string = command_encode(&commandToSend);
      talk_with_jetson_send(encoded_string);
    }

    uart_printf("Servos:");
    for (int i = 0; i < MAX_KEYS; i++) {
      int target_position = motorInstance[i].target_position;
      int present = motorInstance[i].present_position;
      uart_printf("%d,%d,", target_position,present);
    }
    uart_printf("\r\n");
    osDelay(50);
  }
}

/**
 * @brief 测试函数
 */
void app_loop_for_test(){

}
