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
#include <stdbool.h>

#include "main.h"
#include "bsp.h"

// WS2812
#define WS2812_SPI_UNIT     hspi6
extern SPI_HandleTypeDef WS2812_SPI_UNIT;
void WS2812_Ctrl(uint8_t r, uint8_t g, uint8_t b);

// dynamixel motor
#define DXL_DEFAULT_MIN_POSITION_VALUE      0             // Dynamixel will rotate between this value
#define DXL_DEFAULT_MAX_POSITION_VALUE      4095          // and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
#define DXL_MOVING_STATUS_THRESHOLD     10            // Dynamixel moving status threshold
// 电机实例结构体,每个电机对应一个实例.
typedef struct
{
  int id;
  int minPositionValue;
  int maxPositionValue;
  int target_position;
  int present_position;
} MotorInstance_s;
extern MotorInstance_s motorInstance[8];
void motor_init();
void motor_single_init(MotorInstance_s *motor_instance, int id, int target_position, int minPositionValue, int maxPositionValue);
uint8_t dynamixel_write_goal_position(MotorInstance_s *motor_instance, const int goal_position);
uint8_t dynamixel_read_present_position(MotorInstance_s *motor_instance);



// buzzer
#include "buzzer.h"

// modules
void modules_init();

//talkWithJetson
#define MAX_DIGITS 10
#define MAX_KEYS 8
typedef struct {
  int values[MAX_KEYS];
  char keys[MAX_KEYS]; // A to H
} CommandResults;
extern bool needToSendToOrin;
extern CommandResults* decoded_results;
CommandResults* command_decode(const char* s);
char* command_encode(const CommandResults* command);
void talk_with_jetson_init();
void talk_with_jetson_send(char *send_buf);

#endif //MODULES_H
