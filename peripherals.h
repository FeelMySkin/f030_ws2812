#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "periph_functions.h"

#define BUT1_GPIO   GPIOA
#define BUT1_PIN    LL_GPIO_PIN_0
#define BUT2_GPIO   GPIOA
#define BUT2_PIN    LL_GPIO_PIN_1
#define BUT3_GPIO   GPIOA
#define BUT3_PIN    LL_GPIO_PIN_2

#define WS_GPIO     GPIOA
#define WS_PIN      LL_GPIO_PIN_9
#define WS_AF       LL_GPIO_AF_2

#define WS_TIM      TIM1
#define WS_TIM_CH   LL_TIM_CHANNEL_CH2
#define BUT_TIM     TIM14

#define WS_DMA      DMA1
#define WS_DMA_CH   LL_DMA_CHANNEL_5


#endif
