#ifndef IMU_TASK_H_
#define IMU_TASK_H_

#include "struct_typedef.h"

extern void imu_task(void const *pvParameters);

extern const fp32 * get_accel_pointer(void);

#endif 
