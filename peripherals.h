#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "periph_functions.h"

#define BUT1_GPIO   GPIOA
#define BUT1_PIN    LL_GPIO_PIN_0
#define WS_GPIO     GPIOB
#define WS_PIN      LL_GPIO_PIN_1
#define WS_AF       LL_GPIO_AF_1

#define WS_TIM      TIM3
#define WS_TIM_CH   LL_TIM_CHANNEL_CH4
#define BUT_TIM     TIM14

#define WS_DMA      DMA1
#define WS_DMA_CH   LL_DMA_CHANNEL_3


#endif
