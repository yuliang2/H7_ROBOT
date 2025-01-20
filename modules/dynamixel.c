#include "modules.h"
#include "dynamixel_sdk/dynamixel_sdk.h"
#include "stdlib.h"

// Control table address
#define ADDR_PRO_TORQUE_ENABLE          64                 // Control table address is different in Dynamixel model
#define ADDR_PRO_GOAL_POSITION          116
#define ADDR_PRO_PRESENT_POSITION       132

#define PROTOCOL_VERSION                2.0

#define BAUDRATE                        57600
#define TORQUE_ENABLE                   1
#define TORQUE_DISABLE                  0

int port_num;
int dxl_comm_result = COMM_TX_FAIL;             // Communication result

uint8_t dxl_error = 0;                          // Dynamixel error
int32_t dxl_present_position = 0;               // Present position

PacketData *packetData;
int     g_used_port_num;
uint8_t    *g_is_using;

MotorInstance_s motorInstance[8];

/**
 * @brief 所有舵机的初始化函数
 * @note 在app的startMotorTask中调用
 */
void motor_init() {
    port_num = portHandler("2");
    openPort(2);
    packetHandler();

    motor_single_init(&motorInstance[0], 11, 2166, DXL_DEFAULT_MIN_POSITION_VALUE, DXL_DEFAULT_MAX_POSITION_VALUE);
    motor_single_init(&motorInstance[1], 12, 1724, DXL_DEFAULT_MIN_POSITION_VALUE, DXL_DEFAULT_MAX_POSITION_VALUE);
    motor_single_init(&motorInstance[2], 13, 1824, DXL_DEFAULT_MIN_POSITION_VALUE, DXL_DEFAULT_MAX_POSITION_VALUE);
    motor_single_init(&motorInstance[3], 14, 1519, DXL_DEFAULT_MIN_POSITION_VALUE, DXL_DEFAULT_MAX_POSITION_VALUE);
    motor_single_init(&motorInstance[4], 15, 2178, DXL_DEFAULT_MIN_POSITION_VALUE, DXL_DEFAULT_MAX_POSITION_VALUE);
    motor_single_init(&motorInstance[5], 16, 1227, DXL_DEFAULT_MIN_POSITION_VALUE, DXL_DEFAULT_MAX_POSITION_VALUE);
    motor_single_init(&motorInstance[6], 17, 430, DXL_DEFAULT_MIN_POSITION_VALUE, DXL_DEFAULT_MAX_POSITION_VALUE);
    motor_single_init(&motorInstance[7], 18, 1973, DXL_DEFAULT_MIN_POSITION_VALUE, DXL_DEFAULT_MAX_POSITION_VALUE);

}

/**
 * @brief 单个电机初始化函数
 * @note 在motor_init中调用
 */
void motor_single_init(MotorInstance_s *motor_instance, int id, int target_position, int minPositionValue, int maxPositionValue){

    motor_instance->id = id;
    motor_instance->minPositionValue = minPositionValue;
    motor_instance->maxPositionValue = maxPositionValue;
    motor_instance->target_position = target_position;

    // Enable Dynamixel Torque
    write1ByteTxRx(port_num, PROTOCOL_VERSION, motor_instance->id, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE);
    if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS)
    {
        dynamixel_log("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
    }
    else if ((dxl_error = getLastRxPacketError(port_num, PROTOCOL_VERSION)) != 0)
    {
        dynamixel_log("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
    }
    else
    {
        dynamixel_log("Dynamixel [ID:%03d] has been successfully enabled \n", motor_instance->id);
    }
}

/**
 * @brief dynamixel电机的位置控制函数
 * @note 在motor task中使用
 */
uint8_t dynamixel_write_goal_position(MotorInstance_s *motor_instance, const int goal_position) {
    // Write goal position
    write4ByteTxRx(port_num, PROTOCOL_VERSION, motor_instance->id, ADDR_PRO_GOAL_POSITION, goal_position);
    if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS)
    {
        dynamixel_log("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
        return false;
    }
    if ((dxl_error = getLastRxPacketError(port_num, PROTOCOL_VERSION)) != 0)
    {
        dynamixel_log("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
        return false;
    }
    return true;
}

/**
 * @brief dynamixel电机的位置读取函数
 * @note 在motor task中使用
 */
uint8_t dynamixel_read_present_position(MotorInstance_s *motor_instance) {
    // Read present position
    uint32_t present_position = read4ByteTxRx(port_num, PROTOCOL_VERSION, motor_instance->id, ADDR_PRO_PRESENT_POSITION);
    if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS)
    {
        dynamixel_log("ID:%d %s\n", motor_instance->id, getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
        return false;
    }
    else if ((dxl_error = getLastRxPacketError(port_num, PROTOCOL_VERSION)) != 0)
    {
        dynamixel_log("ID:%d %s\n", motor_instance->id, getRxPacketError(PROTOCOL_VERSION, dxl_error));
        return false;
    }
    motor_instance->present_position = present_position;
    return true;
}

/**
 * @brief dynamixel电机的检查在线函数
 * @note 备用
 */
int dynamixel_check_motor_online(MotorInstance_s *motor_instance) {
    // Try to ping the Dynamixel
    // Get Dynamixel model number
    int dxl_model_number = pingGetModelNum(port_num, PROTOCOL_VERSION, motor_instance->id);
    if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS)
    {
        dynamixel_log("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
    }
    else if ((dxl_error = getLastRxPacketError(port_num, PROTOCOL_VERSION)) != 0)
    {
        dynamixel_log("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
    }

    printf("[ID:%03d] ping Succeeded. Dynamixel model number : %d\n", motor_instance->id, dxl_model_number);
    return dxl_model_number;
}
