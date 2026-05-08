#ifndef SONAR_H_
#define SONAR_H_

#include <stdint.h>

void Sonar_UART_Init(void);
uint16_t Sonar_ReadDistance(void);

#endif
