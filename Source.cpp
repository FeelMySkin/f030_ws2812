#include "Source.h"

extern "C"
{
    void EXTI0_1_IRQHandler()
    {
        LL_EXTI_ClearFlag_0_31(BUT1_PIN);
        LL_EXTI_DisableIT_0_31(BUT1_PIN);
        LL_TIM_EnableCounter(BUT_TIM);
    }

    void TIM14_IRQHandler()
    {
        LL_EXTI_ClearFlag_0_31(BUT1_PIN);
        LL_EXTI_EnableIT_0_31(BUT1_PIN);
    }
}

int main()
{
    
    InitPeriph();
    InitRCC();
    InitGPIO();
    InitWS();
    InitTIM();
    

    while(1)
    {
        __WFI();
    }
}



void InitRCC()
{
    LL_RCC_HSI_Enable();
    while(!LL_RCC_IsActiveFlag_HSIRDY()) asm("NOP");
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) asm("NOP");

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2,LL_RCC_PLL_MUL_12);
    LL_RCC_PLL_Enable();
    while(!LL_RCC_PLL_IsReady()) asm("NOP");

    //LL_RCC_SetSysClkSource()
    SystemCoreClockUpdate();
    
}

void InitPeriph()
{
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3 | LL_APB1_GRP1_PERIPH_TIM14);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA | LL_AHB1_GRP1_PERIPH_GPIOB | LL_AHB1_GRP1_PERIPH_DMA1);
}

void InitGPIO()
{
    LL_GPIO_InitTypeDef gpio;
    gpio.Mode = LL_GPIO_MODE_INPUT;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio.Pin = BUT1_PIN;
    gpio.Pull = LL_GPIO_PULL_UP;
    gpio.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(BUT1_GPIO,&gpio);

    LL_EXTI_InitTypeDef exti;
    exti.Line_0_31 = GetExtiLine(BUT1_PIN);
    exti.LineCommand = ENABLE;
    exti.Mode = LL_EXTI_MODE_IT;
    exti.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
    LL_EXTI_Init(&exti);
    SetExtiSource(BUT1_GPIO,BUT1_PIN);

    EnableExtiIRQn(BUT1_PIN,1);
}

void InitWS()
{
    WS_TypeDef wss;
    wss.pixels = 1;
    wss.ws_af = WS_AF;
    wss.ws_dma = WS_DMA;
    wss.ws_dma_channel = WS_DMA_CH;
    wss.ws_gpio = WS_GPIO;
    wss.ws_pin = WS_PIN;
    wss.ws_tim = WS_TIM;
    wss.ws_tim_ch = WS_TIM_CH;
    ws.Init(wss);
    ws.SetPixelColor(0,Color(0,0,255));
    ws.Colorize();
}

void InitTIM()
{
    LL_TIM_InitTypeDef tim;
    tim.Autoreload = 15;
    tim.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    tim.CounterMode = LL_TIM_COUNTERMODE_UP;
    tim.Prescaler = SystemCoreClock/1000;
    tim.RepetitionCounter = 0;
    LL_TIM_Init(BUT_TIM,&tim);
    LL_TIM_SetOnePulseMode(BUT_TIM,LL_TIM_ONEPULSEMODE_SINGLE);

    EnableTimIRQn(BUT_TIM,0);
}