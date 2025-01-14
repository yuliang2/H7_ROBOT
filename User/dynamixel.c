#include "user.h"
#include "dynamixel_sdk.h"
#include "stdlib.h"

// Control table address
#define ADDR_PRO_TORQUE_ENABLE          64                 // Control table address is different in Dynamixel model
#define ADDR_PRO_GOAL_POSITION          116
#define ADDR_PRO_PRESENT_POSITION       132

#define PROTOCOL_VERSION                2.0

#define DXL_ID                          1
#define BAUDRATE                        57600
#define TORQUE_ENABLE                   1
#define TORQUE_DISABLE                  0
#define DXL_MINIMUM_POSITION_VALUE      0             // Dynamixel will rotate between this value
#define DXL_MAXIMUM_POSITION_VALUE      4095          // and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
#define DXL_MOVING_STATUS_THRESHOLD     10            // Dynamixel moving status threshold

int port_num;

int dynamixel_index = 0;
int dxl_comm_result = COMM_TX_FAIL;             // Communication result
int dxl_goal_position[8] = { 0, DXL_MAXIMUM_POSITION_VALUE/4, DXL_MAXIMUM_POSITION_VALUE/2, DXL_MAXIMUM_POSITION_VALUE/4*3, DXL_MAXIMUM_POSITION_VALUE, DXL_MAXIMUM_POSITION_VALUE/4*3, DXL_MAXIMUM_POSITION_VALUE/2, DXL_MAXIMUM_POSITION_VALUE/4 };         // Goal position

uint8_t dxl_error = 0;                          // Dynamixel error
int32_t dxl_present_position = 0;               // Present position

PacketData *packetData;
int     g_used_port_num;
uint8_t    *g_is_using;

void dynamixel_init(){
    port_num = portHandler("2");
    packetHandler();

    // Enable Dynamixel Torque
    write1ByteTxRx(port_num, PROTOCOL_VERSION, DXL_ID, ADDR_PRO_TORQUE_ENABLE, TORQUE_ENABLE);
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
        dynamixel_log("Dynamixel has been successfully connected \n");
    }
}

uint8_t dynamixel_write_goal_position(int port_num, int id, const int goal_position) {
    // Write goal position
    write4ByteTxRx(port_num, PROTOCOL_VERSION, DXL_ID, ADDR_PRO_GOAL_POSITION, goal_position);
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

uint8_t dynamixel_read_present_position(int port_num, int id, int32_t *present_position) {
    // Read present position
    *present_position = read4ByteTxRx(port_num, PROTOCOL_VERSION, DXL_ID, ADDR_PRO_PRESENT_POSITION);
    if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS)
    {
        dynamixel_log("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
        return false;
    }
    else if ((dxl_error = getLastRxPacketError(port_num, PROTOCOL_VERSION)) != 0)
    {
        dynamixel_log("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
        return false;
    }
    return true;
}

void dynamixel_loop() {
    static  bool finishGoal = true;
    static uint32_t startTime;
    if (finishGoal) {
        dynamixel_write_goal_position(port_num,DXL_ID,dxl_goal_position[dynamixel_index]);
        startTime = HAL_GetTick();
        finishGoal = false;
    }
    dynamixel_read_present_position(port_num,DXL_ID,&dxl_present_position);
    dynamixel_log("ID %3d:%3d,%3d\r\n",DXL_ID, dxl_goal_position[dynamixel_index], dxl_present_position);
    if (abs(dxl_goal_position[dynamixel_index] - dxl_present_position) <= DXL_MOVING_STATUS_THRESHOLD && HAL_GetTick() > startTime + 2000) {
        finishGoal = true;
        dynamixel_index = (dynamixel_index + 1) % 8;
    }

}
