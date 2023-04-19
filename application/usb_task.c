/**
 * **************************************************************
 * @file usb_task.c
 * @author xiaobaige (buaaxiaobaige@163.com)
 * @brief USB传输任务
 * @version 0.1
 * @date 2022-12-07
 * 
 * @copyright Copyright (c) 2022
 * @todo    detect task
 * **************************************************************
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include "usb_task.h"
#include "cmsis_os.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#include "imu_task.h"

// #define TO_SERIAL_HELPER        // 用于串口调试助手
#define TO_MATLAB               // 用于MATLAB分析


static void usb_printf(const char *fmt,...);
static uint8_t memCat(uint8_t *pTargetAddr, uint8_t Size, const char *Format, ...);

extern uint8_t usb_tx_buf[USB_TX_BUF_SIZE];
extern uint8_t usb_rx_buf[USB_RX_BUF_SIZE];

/**
 * @brief USB 接收任务
 * 
 * @param argument 
 */
void usb_rx_task(void const * argument){
    int data_len = 0;
    while(1){
        data_len = ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
    }
}


/**
 * @brief USB 发送任务
 * 
 * @param argument 
 */
void usb_tx_task(void const *argument){
    MX_USB_DEVICE_Init();
    const fp32 * accel;

    while(1){
        osDelay(10);
        accel = get_accel_pointer();

        if(accel != NULL){
            usb_printf("%f,%f,%f\r\n", accel[0], accel[1], accel[2]);
        }

    }
}


/**
 * @brief 通过USB CDC向上位机发送信息
 * 
 * @param fmt 
 * @param ... 
 */
static void usb_printf(const char *fmt,...)
{
    static va_list ap;
    uint16_t len = 0;
    va_start(ap, fmt);
    len = vsprintf((char *)usb_tx_buf, fmt, ap);
    va_end(ap);
    CDC_Transmit_FS(usb_tx_buf, len);
}



/**
 * @brief  MemCatenate 解析数据格式，进行内存拼接(C标准库为啥没这玩意 -_-!)
 * @param  TargetAddr  : 目标区块起始地址
            Size        : 目标区块大小
            Format      : 格式化字符串
            ...         : 不定长参数
 * @retval 拼接后的总长度(Byte)
 * @note   处理字符串时本函数不会在末尾加0x00
 */
static uint8_t memCat(uint8_t *pTargetAddr, uint8_t Size, const char *Format, ...)
{
    va_list pA;
    uint8_t *pTmp = pTargetAddr;
    va_start(pA, Format);
    memset(pTargetAddr, 0, Size);   // 清空缓冲区
    while(*Format != '\0')          // 遍历格式化字符串
    {
        if(*Format != '%')              // 非格式化情况
        {
            memcpy(pTmp++, Format, 1);
        }else                           // 格式化情况
        {
            switch(*++Format)               // 判断是何种情况
            {
                case 'c':           // 8字节
                {
                    char tmp = (char)va_arg(pA, int);
                    memcpy(pTmp++, &tmp, 1);
                    break;
                }
                case 'h':           // 16字节 short
                {
                    short tmp = (short)va_arg(pA, int);
                    memcpy(pTmp, &tmp, 2);
                    pTmp += 2;
                    break;
                }
                case 'f':           // 32字节 float
                {
                    float tmp = (float)va_arg(pA, double);
                    memcpy(pTmp, &tmp, 4);
                    pTmp += 4;
                    break;
                }
                case 'd':           // 32字节 int
                {
                    int tmp = (int)va_arg(pA, int);
                    memcpy(pTmp, &tmp, 4);
                    pTmp += 4;
                    break;
                }
                case 's':           // 字符串
                {
                    char *p_tmp = va_arg(pA, char*);
                    uint8_t tmp_size = strlen(p_tmp);
                    memcpy(pTmp, p_tmp, tmp_size);
                    pTmp += tmp_size;
                    break;
                }
            }
        }
        Format++;  // 继续遍历
    }
    return pTmp - pTargetAddr;
}
