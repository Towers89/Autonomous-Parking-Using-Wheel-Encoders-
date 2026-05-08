/**
 * @file main.c
 *
 * @brief Main source code for the Autonomous Self Parking Robot
 * This code uses wheel encoders and a FSM machine in order to do a sequence based off of predetermine distance of a parking spot
 *
 *
 *
 * @author Samuel Torres
 *
 *
 */
#include "inc/Sonar.h"
#include "inc/Encoder.h"
#include <stdint.h>
#include <math.h>
#include "msp.h"
#include "inc/Clock.h"
#include "inc/CortexM.h"
#include "inc/GPIO.h"
#include "inc/EUSCI_A0_UART.h"
#include "inc/Motor.h"
#include "inc/BLE_UART.h"

typedef enum {
    BEGIN,
    REVERSE,
    TURN_IN,
    REVERSE_IN,
    STRAIGHTEN,
    MOVE_UP,
    DONE

} State;

#define BOT_SIZE             5.75
#define FORWARD_DIST_CM      65.0
#define ADJUST_TURN_TICKS   100
#define BACKUP_DIST_CM       11.0
#define REVERSE_IN_DIST_CM   22.0
#define MOVE_UP_DIST_CM       3.0

// Function prototypes
State Begin_State();
State Reverse_State();
State Turnin_State();
State Reversein_State();
State Straighten_State();
State Move_up_State();

int main(void)
{
    //State currentState = BEGIN;
        // Ensure that interrupts are disabled during initialization
        DisableInterrupts();

        // Initialize the 48 MHz Clock
        Clock_Init48MHz();

        // Initialize the RGB LED on the MSP432 microcontroller
        LED2_Init();

        // Initialize the EUSCI_A0_UART module
        EUSCI_A0_UART_Init_Printf();


        // Initialize the DC motors
        Motor_Init();
        Encoder_Init();
        // Enable the interrupts used by the modules
        EnableInterrupts();
        Clock_Delay1ms(2000);
       State currentState = BEGIN;
        while(1)
        {
                switch(currentState)
                {
                    case BEGIN:
                        currentState = Begin_State();
                        break;

                    case REVERSE:
                        currentState = Reverse_State();
                        break;

                    case TURN_IN:
                        currentState = Turnin_State();
                        break;

                    case REVERSE_IN:
                        currentState = Reversein_State();
                       break;

                    case STRAIGHTEN:
                        currentState = Straighten_State();
                        break;

                    case MOVE_UP:
                        currentState = Move_up_State();
                        break;

                    case DONE:
                        Motor_Stop();
                        break;

                    default:
                        Motor_Stop();
                        currentState = DONE;
                        break;
                }
            }
        }

State Begin_State(void)
            {
                static uint8_t started = 0;

                if (!started)
                {
                    Encoder_Reset();
                    //Motor_Forward(3000, 2885);
                    Motor_Forward(3000, 2950);
                    started = 1;
                }

                if (Encoder_GetAverageDistanceCM() >= FORWARD_DIST_CM)
                {
                    Motor_Stop();
                    started = 0;
                    return REVERSE;
                }

                return BEGIN;
            }

State Reverse_State(void)
            {
                static uint8_t started = 0;

                if (!started)
                {
                    Encoder_Reset();
                    Motor_Backward(3000, 2950);
                    started = 1;
                }

                if (Encoder_GetAverageDistanceCM() >= BACKUP_DIST_CM)
                {
                    Motor_Stop();
                    started = 0;
                    return TURN_IN;
                }

                return REVERSE;
            }

State Turnin_State(void)
            {
                static uint8_t started = 0;

                if (!started)
                {
                    Encoder_Reset();
                    Motor_Left(1500, 1450);
                    started = 1;
                }

                if (Encoder_GetAverageTicks() >= ADJUST_TURN_TICKS)
                {
                    Motor_Stop();
                    started = 0;
                    return REVERSE_IN;
                }

                return TURN_IN;
            }

State Reversein_State(void)
            {
                static uint8_t started = 0;

                if (!started)
                {
                    Encoder_Reset();
                    Motor_Backward(1500, 1450);
                    started = 1;
                }

                if (Encoder_GetAverageDistanceCM() >= REVERSE_IN_DIST_CM)
                {
                    Motor_Stop();
                    started = 0;
                    return STRAIGHTEN;
                }

                return REVERSE_IN;
            }

State Straighten_State(void)
            {
                static uint8_t started = 0;

                if (!started)
                {
                    Encoder_Reset();
                    Motor_Right(1500, 1450);
                    started = 1;
                }

                if (Encoder_GetAverageTicks() >= ADJUST_TURN_TICKS)
                {
                    Motor_Stop();
                    started = 0;
                    return MOVE_UP;
                }

                return STRAIGHTEN;
            }

State Move_up_State(void)
            {
                static uint8_t started = 0;

                if (!started)
                {
                    Encoder_Reset();
                    Motor_Forward(1500, 1450);
                    started = 1;
                }

                if (Encoder_GetAverageDistanceCM() >= MOVE_UP_DIST_CM)
                {
                    Motor_Stop();
                    started = 0;
                    return DONE;
                }

                return MOVE_UP;
            }
