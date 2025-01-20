/**
  ******************************************************************************
  * @file           : talkWithJetson.c
  * @author         : XuJiang
  * @brief          : 单片机和上位机通信的模块
  * @attention      : 通信格式< 12A 34B 56C 78D 90E 12F 34G 56H >\r\n 其中A-H表示八个舵机
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

    // Check for closing '>'
    while (isspace(*ptr)) {
        ptr++;
    }
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
        if (command->values[i] >= 0) { // Only add if value is not zero, except for the first item
            ptr += sprintf(ptr, "%d%c ", command->values[i], command->keys[i]);
        }
    }

    // Replace the final space with '>\r\n'
    *(ptr - 1) = '>'; // Replace last space with '>'
    sprintf(ptr, "\r\n"); // Append CRLF at the end

    return s; // Return the encoded string
}

/*Orin串口接收回调函数,解析数据 */
static void orinRxCallback()
{
    if (orin_usart_instance->received_count > 5) {
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
