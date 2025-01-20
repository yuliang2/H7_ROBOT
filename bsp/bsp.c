#include "bsp.h"

/**
 * @brief bsp层初始化统一入口,这里仅初始化必须的bsp组件,其他组件的初始化在各自的模块中进行
 *        需在实时系统启动前调用,目前由RobotoInit()调用
 *
 * @note 其他实例型的外设如CAN和串口会在注册实例的时候自动初始化,不注册不初始化
  */
//
void bspInit()
{
    HAL_GPIO_WritePin(POWER_5V_GPIO_Port, POWER_5V_Pin, 1);
    HAL_GPIO_WritePin(POWER_OUT1_GPIO_Port, POWER_OUT1_Pin, 1);
}

