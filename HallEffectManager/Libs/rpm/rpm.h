#ifndef RPM_H
#define RPM_H

#include "main.h"

#define INACTIVITY_TIMEOUT 4000 // >4s/Revolution will set the RPM to 0
#define NUM_RPM 4

typedef struct {
    uint32_t current_rpm;
    uint32_t total_pulses;
    uint32_t last_seen_time;
    uint32_t pulses_per_rev;
} rpmData;

static rpmData rpms[NUM_RPM] = {0};

void RPM_Init();
void RPM_set_pulses_per_rev(int rpm, int pulse_per_rev);




#endif