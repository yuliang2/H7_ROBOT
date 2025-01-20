#include "port_handler.h"
#include "modules.h"
#include "stdlib.h"
#include "string.h"
#include <stdio.h>
#include <stdarg.h>
#include "usbd_cdc_if.h"

#define LATENCY_TIMER  16

typedef struct
{
    int     socket_fd;
    int     baudrate;
    char    port_name[100];

    double  packet_start_time;
    double  packet_timeout;
    double  tx_time_per_byte;
}PortData;

static PortData *portData;
static USARTInstance *dxl_usart_instance; // dxl通信串口实例

void dynamixel_log(const char *format,...)
{
    uart_printf(format);
}


int portHandler(const char *port_name) {
    int port_num;

    if (portData == NULL)
    {
        port_num = 0;
        g_used_port_num = 1;
        portData = (PortData *)calloc(1, sizeof(PortData));
        g_is_using = (uint8_t*)calloc(1, sizeof(uint8_t));
    }
    else
    {
        for (port_num = 0; port_num < g_used_port_num; port_num++)
        {
            if (!strcmp(portData[port_num].port_name, port_name))
                break;
        }

        if (port_num == g_used_port_num)
        {
            for (port_num = 0; port_num < g_used_port_num; port_num++)
            {
                if (portData[port_num].socket_fd != -1)
                    break;
            }

            if (port_num == g_used_port_num)
            {
                g_used_port_num++;
                portData = (PortData*)realloc(portData, g_used_port_num * sizeof(PortData));
                g_is_using = (uint8_t*)realloc(g_is_using, g_used_port_num * sizeof(uint8_t));
            }
        }
        else
        {
            dynamixel_log("[PortHandler setup] The port number %d has same device name... reinitialize port number %d!!\n", port_num, port_num);
        }
    }

    portData[port_num].socket_fd = -1;
    portData[port_num].baudrate = DEFAULT_BAUDRATE;
    portData[port_num].packet_start_time = 0.0;
    portData[port_num].packet_timeout = 0.0;
    portData[port_num].tx_time_per_byte = 0.0;

    g_is_using[port_num] = False;

    setPortName(port_num, port_name);

    return port_num;
}

// void dxlRxCallback() {
//     dxl_usart_instance->recv_buff;
// }

uint8_t openPort(int port_num) {
    // int baudrate = portData[port_num].baudrate;
    // dynamixel_log("port:%d baudrate:%d\n", port_num, baudrate);
    USART_Init_Config_s conf;
    conf.module_callback = NULL;
    conf.usart_handle = &huart2;
    conf.recv_buff_size = 255u; // mx 255(u8)
    dxl_usart_instance = USARTRegister(&conf);

    return true;
}

void closePort(int port_num) {
    // Useless
}

void clearPort(int port_num) {
    uint8_t uselessData;
    while (HAL_UART_Receive(&huart2,&uselessData,1,0) == HAL_OK);
}

void setPortName(int port_num, const char *port_name) {
    strcpy(portData[port_num].port_name, port_name);
}

char *getPortName(int port_num) {
    return portData[port_num].port_name;
}

uint8_t setBaudRate(int port_num, const int baudrate) {
    // Useless
    return true;
}

int getBaudRate(int port_num) {
    return portData[port_num].baudrate;
}

int getBytesAvailable(int port_num) {
    // May be useless?
    return 0;
}

int readPort(int port_num, uint8_t *packet, int length) {
    int received_length = dxl_usart_instance->received_count;
    if (received_length) {
        memcpy(packet, dxl_usart_instance->recv_buff, received_length);
        memset(dxl_usart_instance->recv_buff, 0, received_length); // 接收结束后清空buffer,对于变长数据是必要的
        dxl_usart_instance->received_count = 0;
    }
    return received_length;
    // return readPortLinux(port_num, packet, length);
}

int writePort(int port_num, uint8_t *packet, int length) {
    USARTSend(dxl_usart_instance, packet, length, USART_TRANSFER_DMA);
    return length;
    // return writePortLinux(port_num, packet, length);
}

void setPacketTimeout(int port_num, uint16_t packet_length) {
    portData[port_num].packet_start_time = HAL_GetTick();
    portData[port_num].packet_timeout = (portData[port_num].tx_time_per_byte * (double)packet_length) + (LATENCY_TIMER * 2.0) + 2.0;
}

void setPacketTimeoutMSec(int port_num, double msec) {
    portData[port_num].packet_start_time = HAL_GetTick();
    portData[port_num].packet_timeout = msec;
}

double getTimeSinceStart(int port_num)
{
    double time_since_start;

    time_since_start = HAL_GetTick() - portData[port_num].packet_start_time;
    if (time_since_start < 0.0)
        portData[port_num].packet_start_time = HAL_GetTick();

    return time_since_start;
}

uint8_t isPacketTimeout(int port_num) {
    if (getTimeSinceStart(port_num) > portData[port_num].packet_timeout)
    {
        portData[port_num].packet_timeout = 0;
        return True;
    }
    return False;
}
