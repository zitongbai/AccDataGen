/**
 * **************************************************************
 * @file usb_task.h
 * @author xiaobaige (buaaxiaobaige@163.com)
 * @brief USB传输任务
 * @version 0.1
 * @date 2022-12-07
 * 
 * @copyright Copyright (c) 2022
 * 
 * **************************************************************
 */

#ifndef USB_TASK_H_
#define USB_TASK_H_

#include "struct_typedef.h"

extern void usb_tx_task(void const * argument);
extern void usb_rx_task(void const * argument);

#endif
