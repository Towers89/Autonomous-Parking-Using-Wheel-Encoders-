#ifndef ENCODER_H_
#define ENCODER_H_

#include "msp.h"
#include <stdint.h>

// Adjust these after calibration
#define ENCODER_WHEEL_DIAMETER_CM   7.0
#define ENCODER_TICKS_PER_REV       360
#define ENCODER_PI                  3.14159265358979323846

void Encoder_Init(void);
void Encoder_Reset(void);

uint32_t Encoder_GetLeftTicks(void);
uint32_t Encoder_GetRightTicks(void);
uint32_t Encoder_GetAverageTicks(void);

double Encoder_GetLeftDistanceCM(void);
double Encoder_GetRightDistanceCM(void);
double Encoder_GetAverageDistanceCM(void);
double Encoder_GetDistancePerTickCM(void);

#endif
