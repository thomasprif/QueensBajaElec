#include "rpm.h"

void RPM_Init(){
    for(int i=0; i<NUM_RPM; i++){
        rpms[i].current_rpm = 0;
        rpms[i].total_pulses = 0;
        rpms[i].last_seen_time = 0;
        rpms[i].pulses_per_rev = 1;
    }
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
    int rpm_sensor;
    if(GPIO_Pin == RPM0_Pin) rpm_sensor = 0;
    if(GPIO_Pin == RPM1_Pin) rpm_sensor = 1;
    if(GPIO_Pin == RPM2_Pin) rpm_sensor = 2;
    if(GPIO_Pin == RPM3_Pin) rpm_sensor = 3;

    rpmData* rpm = &(rpms[rpm_sensor]);

    uint32_t now = HAL_GetTick();
    uint32_t pulse_diff = now - rpm->last_seen_time;

    if(pulse_diff > INACTIVITY_TIMEOUT/(rpm->pulses_per_rev))
        return;

    rpm->current_rpm = 60000 / (pulse_diff*rpm->pulses_per_rev);
    rpm->last_seen_time = now;
    rpm->total_pulses++;
}

void RPM_set_pulses_per_rev(int rpm, int pulse_per_rev){
    rpms[rpm].pulses_per_rev = pulse_per_rev;
}