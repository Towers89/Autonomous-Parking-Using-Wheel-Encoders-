#include "../inc/Encoder.h"

static volatile uint32_t left_ticks  = 0;
static volatile uint32_t right_ticks = 0;


void Encoder_Init(void)
{
    // P5.0 = Right encoder B, P5.2 = Left encoder B
    P5->SEL0 &= ~(BIT0 | BIT2);
    P5->SEL1 &= ~(BIT0 | BIT2);
    P5->DIR  &= ~(BIT0 | BIT2);
    P5->REN  |=  (BIT0 | BIT2);
    P5->OUT  |=  (BIT0 | BIT2);

    P5->IES  &= ~(BIT0 | BIT2);  // rising edge
    P5->IFG  &= ~(BIT0 | BIT2);  // clear flags
    P5->IE   |=  (BIT0 | BIT2);  // enable

    NVIC->ISER[1] |= 1 << (PORT5_IRQn & 31);
}

void PORT5_IRQHandler(void)
{
    if (P5->IFG & BIT0)
    {
        P5->IFG &= ~BIT0;
        right_ticks++;
    }
    if (P5->IFG & BIT2)
    {
        P5->IFG &= ~BIT2;
        left_ticks++;
    }
}

void Encoder_Reset(void)
{
    left_ticks = 0;
    right_ticks = 0;
}

uint32_t Encoder_GetLeftTicks(void)
{
    return left_ticks;
}

uint32_t Encoder_GetRightTicks(void)
{
    return right_ticks;
}

uint32_t Encoder_GetAverageTicks(void)
{
    return (left_ticks + right_ticks) / 2;
}

double Encoder_GetDistancePerTickCM(void)
{
    double circumference_cm = ENCODER_PI * ENCODER_WHEEL_DIAMETER_CM;
    return circumference_cm / ENCODER_TICKS_PER_REV;
}


double Encoder_GetLeftDistanceCM(void)
{
    return Encoder_GetLeftTicks() * Encoder_GetDistancePerTickCM();
}

double Encoder_GetRightDistanceCM(void)
{
    return Encoder_GetRightTicks() * Encoder_GetDistancePerTickCM();
}

double Encoder_GetAverageDistanceCM(void)
{
    return Encoder_GetAverageTicks() * Encoder_GetDistancePerTickCM();
}
