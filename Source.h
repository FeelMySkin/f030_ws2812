#ifndef SOURCE_H
#define SOURCE_H

#include "defines.h"
#include "ws2812_controller.h"

WS2812Controller ws;


void InitRCC();
void InitPeriph();

void InitGPIO();
void InitWS();
void InitTIM();

#endif