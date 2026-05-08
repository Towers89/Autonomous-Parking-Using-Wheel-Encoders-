#include "msp.h"
#include <stdint.h>


void Sonar_UART_Init(void)
{
    // Hold EUSCI_A2 in reset
    EUSCI_A2->CTLW0 |= 1;

    // No modulation
    EUSCI_A2->MCTLW = 0;

    // SMCLK, keep reset set
    EUSCI_A2->CTLW0 |= 0x00C1;

    // 12 MHz / 9600 = 1250
    EUSCI_A2->BRW = 1250;

    // P3.2 = UCA2RXD
    P3->SEL0 |= BIT2;
    P3->SEL1 &= ~BIT2;
    P3->DIR  &= ~BIT2;

    // Release reset
    EUSCI_A2->CTLW0 &= ~1;

    // Disable interrupts
    EUSCI_A2->IE &= ~0x0F;
}

char Sonar_ReadChar(void)
{
    while (!(EUSCI_A2->IFG & BIT0)); // wait for RX
    return EUSCI_A2->RXBUF;
}

uint16_t Sonar_ReadDistance(void)
{
    char c;
    char d1, d2, d3, cr;

    do
    {
        c = Sonar_ReadChar();
    } while (c != 'R');

    d1 = Sonar_ReadChar();
    d2 = Sonar_ReadChar();
    d3 = Sonar_ReadChar();
    cr = Sonar_ReadChar();

    if (d1 < '0' || d1 > '9' ||
        d2 < '0' || d2 > '9' ||
        d3 < '0' || d3 > '9')
    {
        return 0;
    }

    if (cr != '\r')
    {
        return 0;
    }

    return (uint16_t)((d1 - '0') * 100 +
                      (d2 - '0') * 10 +
                      (d3 - '0'));
}
int Sonar_ReadChar_NonBlocking(char *out)
{
    if (EUSCI_A2->IFG & EUSCI_A_IFG_RXIFG)
    {
        *out = EUSCI_A2->RXBUF;
        return 1;
    }
    return 0;
}
