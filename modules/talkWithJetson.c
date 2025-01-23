/**
  ******************************************************************************
  * @file           : talkWithJetson.c
  * @author         : XuJiang
  * @brief          : 单片机和上位机通信的模块
  * @attention      : 通信格式< 12A 34B 56C 78D 90E 12F 34G 56H CRC>\r\n 其中A-H表示八个舵机
  * @date           : 2025/1/16
  ******************************************************************************
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "modules.h"

static USARTInstance *orin_usart_instance; // orin系统串口实例
CommandResults* decoded_results;

bool needToSendToOrin = false;


/**
 * @brief 计算CRC8校验值
 * @note 参数：字符串data，需要计算的长度length
 */
uint8_t calculate_crc(const char* data, size_t length) {
    uint8_t crc = 0;
    for (size_t i = 0; i < length; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x07; // 最后的实现可以依据具体的 CRC-8 多项式做调整
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

/**
 * @brief 和Orin NX的通信数据解码
 * @note 传入串口接收到的一串字符串，得到一个存储有Keys和Values的CommandResults结构体
 */
CommandResults* command_decode(const char* s) {
    static CommandResults results;

    // Initialize to make sure If there are no matches, we return zeros.
    for (int i = 0; i < MAX_KEYS; i++) {
        results.values[i] = 0;
        results.keys[i] = 'A' + i; // 初始化字符A到H
    }

    // Local variables
    char temp[MAX_DIGITS] = {0};
    const char* ptr = s;

    // Check if the string starts with '<'
    if (*ptr != '<') {
        return NULL; // Invalid format
    }
    ptr++; // Move past '<'

    for (int i = 0; i < MAX_KEYS; i++) {
        // Skip whitespace
        while (isspace(*ptr)) {
            ptr++;
        }

        // Read number
        int sign = 1;
        if (*ptr == '-') { // Check for negative number
            sign = -1;
            ptr++;
        } else if (*ptr == '+') { // Check for positive explicitly
            ptr++;
        }

        // Read digits
        int idx = 0;
        while (isdigit(*ptr) && idx < MAX_DIGITS - 1) {
            temp[idx++] = *ptr++;
        }
        temp[idx] = '\0'; // Null-terminate the string

        // Check for the expected character 'A', 'B', ..., 'H'
        if (*ptr != results.keys[i]) {
            return NULL; // Invalid format
        }
        ptr++; // Move past the character

        // Convert the string to an integer and store in results
        results.values[i] = sign * atoi(temp);
    }

    // Skip whitespace before CRC
    while (isspace(*ptr)) {
        ptr++;
    }
    // 读取接受到的CRC
    uint8_t crc_received = (uint8_t)strtol(ptr, NULL, 16); // 读取接收到的CRC
    // 计算CRC，长度不包括 CRC 和 '>'
    size_t length_for_crc = ptr - s; // 计算 CRC 计算时应使用的长度
    uint8_t crc_calculated = calculate_crc(s, length_for_crc); // 计算 CRC
    if (crc_received != crc_calculated) {
        return NULL; // CRC 校验失败
    }

    // Check for closing '>'
    ptr+=2; // Skip CRC
    if (*ptr != '>') {
        return NULL; // Invalid format
    }

    return &results; // Return the pointer to results
}

/**
 * @brief 和Orin NX的通信数据编码
 * @note 传入一个存储有Keys和Values的CommandResults结构体，得到一串用于串口发送的字符串
 */
char* command_encode(const CommandResults* command) {
    static char s[128]; // Allocate static buffer for output
    char* ptr = s;

    ptr += sprintf(ptr, "<"); // Start with '<'

    for (int i = 0; i < MAX_KEYS; i++) {
        if (command->values[i] >= 0) { // Only add if value is not below zero
            ptr += sprintf(ptr, "%d%c ", command->values[i], command->keys[i]);
        }
    }

    // 计算CRC并附加到字符串末尾
    uint8_t crc = calculate_crc(s, strlen(s));
    ptr += sprintf(ptr, "%02X>\r\n", crc); // Append CRC and CRLF

    return s; // Return the encoded string
}

/*Orin串口接收回调函数,解析数据 */
static void orinRxCallback()
{
    if (orin_usart_instance->received_count > 10) {
        // 收到的数据包不是空包
        decoded_results = command_decode(orin_usart_instance->recv_buff);
        if (decoded_results != NULL) {
            for (int i = 0; i < MAX_KEYS; i++) {
                int target_position = decoded_results->values[i];
                motorInstance[i].target_position = target_position;
            }
        }
    }
    needToSendToOrin = true;
}

/**
 * @brief 和Orin NX的通信的初始化函数
 */
void talk_with_jetson_init() {
    USART_Init_Config_s conf;
    conf.module_callback = orinRxCallback;
    conf.usart_handle = &huart3;
    conf.recv_buff_size = 255u; // mx 255(u8)
    orin_usart_instance = USARTRegister(&conf);
}

/**
 * @brief 和Orin NX的发送函数
 * @note 传入用于发送的一串字符串，进行串口发送
 */
void talk_with_jetson_send(char* send_buf) {
    size_t send_size = strlen(send_buf);
    USARTSend(orin_usart_instance, send_buf, send_size, USART_TRANSFER_DMA);
}
