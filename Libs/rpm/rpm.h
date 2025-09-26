#ifndef RPM_H
#define RPM_H

#include "main.h"

#define INACTIVITY_TIMEOUT 400000 // >4s/Revolution will set the RPM to 0
#define NUM_RPM 4

extern int RPM_EXTI_FLAG;

typedef struct {
    uint16_t current_rpm;
    uint32_t total_pulses;
    uint32_t last_seen_time;
    uint32_t pulses_per_rev;
} rpmData;

extern rpmData rpms[NUM_RPM];

void RPM_Init();
void RPM_set_pulses_per_rev(int rpm, int pulse_per_rev);
void RPM_Update();


#endif