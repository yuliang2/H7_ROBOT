#include "user.h"
#include "dynamixel_sdk.h"
#include "stdlib.h"

// Control table address and other constants
#define ADDR_PRO_TORQUE_ENABLE          64            // Control table address is different in Dynamixel model
#define ADDR_PRO_GOAL_POSITION          116
#define ADDR_PRO_PRESENT_POSITION       132

#define PROTOCOL_VERSION                2.0

#define DXL_ID                          1
#define BAUDRATE                        57600
#define TORQUE_ENABLE                   1
#define TORQUE_DISABLE                  0
#define DXL_MINIMUM_POSITION_VALUE      0             // Dynamixel will rotate between this value
#define DXL_MAXIMUM_POSITION_VALUE      4095          // and this value (note that the Dynamixel would not move when the position value is out of movable range. Check e-manual about the range of the Dynamixel you use.)
#define DXL_MOVING_STATUS_THRESHOLD     20            // Dynamixel moving status threshold

int port_num = 2;

int dynamixel_index = 0;
int dxl_comm_result = COMM_TX_FAIL;             // Communication result
int dxl_goal_position[2] = { DXL_MINIMUM_POSITION_VALUE, DXL_MAXIMUM_POSITION_VALUE };         // Goal position

uint8_t dxl_error = 0;                          // Dynamixel error
int32_t dxl_present_position = 0;               // Present position

void dynamixel_init(){
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

void dynamixel_loop() {
    // Write goal position
    write4ByteTxRx(port_num, PROTOCOL_VERSION, DXL_ID, ADDR_PRO_GOAL_POSITION, dxl_goal_position[dynamixel_index]);
    if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS)
    {
        dynamixel_log("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
    }
    else if ((dxl_error = getLastRxPacketError(port_num, PROTOCOL_VERSION)) != 0)
    {
        dynamixel_log("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
    }

    do
    {
        // Read present position
        dxl_present_position = read4ByteTxRx(port_num, PROTOCOL_VERSION, DXL_ID, ADDR_PRO_PRESENT_POSITION);
        if ((dxl_comm_result = getLastTxRxResult(port_num, PROTOCOL_VERSION)) != COMM_SUCCESS)
        {
            dynamixel_log("%s\n", getTxRxResult(PROTOCOL_VERSION, dxl_comm_result));
        }
        else if ((dxl_error = getLastRxPacketError(port_num, PROTOCOL_VERSION)) != 0)
        {
            dynamixel_log("%s\n", getRxPacketError(PROTOCOL_VERSION, dxl_error));
        }

        dynamixel_log("[ID:%03d] GoalPos:%03d  PresPos:%03d\n", DXL_ID, dxl_goal_position[dynamixel_index], dxl_present_position);

    } while ((abs(dxl_goal_position[dynamixel_index] - dxl_present_position) > DXL_MOVING_STATUS_THRESHOLD));

    // Change goal position
    if (dynamixel_index == 0)
    {
        dynamixel_index = 1;
    }
    else
    {
        dynamixel_index = 0;
    }
}
