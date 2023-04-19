#include "imu_task.h"
#include "cmsis_os.h"
#include "BMI088driver.h"

fp32 gyro[3], accel[3], temp;

void imu_task(void const *pvParameters){
    while(BMI088_init())
    {
        osDelay(10);
    }

    while(1){
        osDelay(10);
        BMI088_read(gyro, accel, &temp);
    }

}


const fp32 * get_accel_pointer(void){
    return accel;
}

