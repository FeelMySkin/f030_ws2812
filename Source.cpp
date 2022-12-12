#include "Source.h"
#include <stdlib.h>

struct Flags{
	bool but1_pressed:1;
	bool but2_pressed:1;
	bool but3_pressed:1;
    bool but1_req:1;
    bool but2_req:1;
    bool but3_req:1;
}flags;

Color Blue = Color(0,0,255);
Color Red = Color(255,0,0);
Color Yellow = Color(255,255,0);
Color ultra_tst;

enum ColorStateMachine
{
    DefaultBlue,
    DefaultYellow,
    DefaultRed,
	BlinkBlue,
	BlinkYellow,
	BlinkRed
}color_state;

extern "C"
{
    void EXTI0_1_IRQHandler()
    {
        if(LL_EXTI_IsActiveFlag_0_31(BUT1_PIN))
        {
            DisableExtiIRQn(BUT1_PIN);
            //LL_EXTI_DisableIT_0_31(BUT1_PIN);
			//LL_EXTI_ClearFlag_0_31(BUT1_PIN);
            LL_TIM_SetCounter(BUT_TIM,0);
            LL_TIM_EnableCounter(BUT_TIM);
        }
        if(LL_EXTI_IsActiveFlag_0_31(BUT2_PIN))
        {
            DisableExtiIRQn(BUT2_PIN);
            //LL_EXTI_DisableIT_0_31(BUT2_PIN);
            LL_TIM_SetCounter(BUT_TIM,0);
            LL_TIM_EnableCounter(BUT_TIM);
        }
    }

    void EXTI2_3_IRQHandler()
    {
        if(LL_EXTI_IsActiveFlag_0_31(BUT3_PIN))
        {
            DisableExtiIRQn(BUT3_PIN);
            //LL_EXTI_DisableIT_0_31(BUT3_PIN);
            LL_TIM_SetCounter(BUT_TIM,0);
            LL_TIM_EnableCounter(BUT_TIM);
        }
    }

    void TIM14_IRQHandler()
    {
		LL_TIM_ClearFlag_UPDATE(BUT_TIM);
        if(LL_EXTI_IsActiveFlag_0_31(BUT1_PIN))
        {
            LL_EXTI_ClearFlag_0_31(BUT1_PIN);
            if(LL_GPIO_ReadInputPort(BUT1_GPIO) & BUT1_PIN) flags.but1_pressed = false;
            else flags.but1_pressed = true;
            flags.but1_req = true;
            EnableExtiIRQn(BUT1_PIN,0);
        }

        if(LL_EXTI_IsActiveFlag_0_31(BUT2_PIN))
        {
            LL_EXTI_ClearFlag_0_31(BUT2_PIN);
            if(LL_GPIO_ReadInputPort(BUT2_GPIO) & BUT2_PIN) flags.but2_pressed = false;
            else flags.but2_pressed = true;
            flags.but2_req = true;
            EnableExtiIRQn(BUT2_PIN,0);
        }

        if(LL_EXTI_IsActiveFlag_0_31(BUT3_PIN))
        {
            LL_EXTI_ClearFlag_0_31(BUT3_PIN);
            if(LL_GPIO_ReadInputPort(BUT3_GPIO) & BUT3_PIN) flags.but3_pressed = false;
            else flags.but3_pressed = true;
            flags.but3_req = true;
            EnableExtiIRQn(BUT3_PIN,0);
        }
    }
}

bool tst = false;
bool strt = false;
uint8_t anim_state = 0;

int main()
{
	//while(!strt) asm("NOP");
	
    InitPeriph();
    InitRCC();
    InitGPIO();
    InitWS();
    InitTIM();
	while(ws.IsWriting()) asm("NOP");
	srand(LL_TIM_GetCounter(WS_TIM));
    color_state = DefaultBlue;
	for(int i =0;i<100000;++i) asm("NOP");
	ws.SetPixelColor(0,Blue);
	ws.Colorize();
	tst = false;
    
    

    while(1)
    {
        if(flags.but1_req)
        {

            if(flags.but1_pressed)
			{
				if(color_state == DefaultBlue) color_state = BlinkBlue;
				else color_state = DefaultBlue;
			}
            flags.but1_req = false;
        }
		
         if(flags.but2_req)
        {

            if(flags.but2_pressed)
			{
				if(color_state == DefaultYellow) color_state = BlinkYellow;
				else color_state = DefaultYellow;
			}
            flags.but2_req = false;
        }

         if(flags.but3_req)
        {

            if(flags.but3_pressed)
			{
				if(color_state == DefaultRed) color_state = BlinkRed;
				else color_state = DefaultRed;
			}
            flags.but3_req = false;
        }
		if(tst)
		{
			tst = false;
			ws.SetPixelColor(0,ultra_tst);
			ws.Colorize();
		}
        Colorize();
    }
}

void Colorize()
{
    switch(color_state)
    {
        case DefaultBlue:
            ws.SetPixelColor(0,Blue);
        break;

        case DefaultRed:
            ws.SetPixelColor(0,Red);
        break;

        case DefaultYellow:
            ws.SetPixelColor(0,Yellow);
        break;
		
		case BlinkBlue:
			ws.SetPixelColor(0, Color(0,0,LL_TIM_GetCounter(BLINK_TIM)/4));
		break;
		
		case BlinkRed:
			ws.SetPixelColor(0, Color(LL_TIM_GetCounter(BLINK_TIM)/4,0,0));
		break;
		
		case BlinkYellow:
			ws.SetPixelColor(0, Color(LL_TIM_GetCounter(BLINK_TIM)/4,LL_TIM_GetCounter(BLINK_TIM)/4,0));
		break;
			

    }
    ws.Colorize();
}



void InitRCC()
{
   LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);
    LL_RCC_HSI_Enable();
    while(!LL_RCC_HSI_IsReady()) asm("NOP");
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) asm("NOP");

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI_DIV_2,LL_RCC_PLL_MUL_10);
   // LL_RCC_PLL_Enable();
	//while(!LL_RCC_PLL_IsReady()) asm("NOP");
    //LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    //while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) asm("NOP");
    SystemCoreClockUpdate();
    
}

void InitPeriph()
{
	LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3 | LL_APB1_GRP1_PERIPH_TIM14 | LL_APB1_GRP1_PERIPH_PWR);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA | LL_AHB1_GRP1_PERIPH_GPIOB | LL_AHB1_GRP1_PERIPH_DMA1 | LL_AHB1_GRP1_PERIPH_FLASH);
}

void InitGPIO()
{
    LL_GPIO_InitTypeDef gpio;
    gpio.Mode = LL_GPIO_MODE_INPUT;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio.Pull = LL_GPIO_PULL_UP;
    gpio.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    
    gpio.Pin = BUT1_PIN;
    LL_GPIO_Init(BUT1_GPIO,&gpio);
    gpio.Pin = BUT2_PIN;
    LL_GPIO_Init(BUT2_GPIO,&gpio);
    gpio.Pin = BUT3_PIN;
    LL_GPIO_Init(BUT3_GPIO,&gpio);


    LL_EXTI_InitTypeDef exti;
    exti.LineCommand = ENABLE;
    exti.Mode = LL_EXTI_MODE_IT;
    exti.Trigger = LL_EXTI_TRIGGER_RISING_FALLING;
    
    exti.Line_0_31 = GetExtiLine(BUT1_PIN);
    LL_EXTI_Init(&exti);
    SetExtiSource(BUT1_GPIO,BUT1_PIN);
    EnableExtiIRQn(BUT1_PIN,1);
    
    exti.Line_0_31 = GetExtiLine(BUT2_PIN);
    LL_EXTI_Init(&exti);
    SetExtiSource(BUT2_GPIO,BUT2_PIN);
    EnableExtiIRQn(BUT2_PIN,1);
    
    exti.Line_0_31 = GetExtiLine(BUT3_PIN);
    LL_EXTI_Init(&exti);
    SetExtiSource(BUT3_GPIO,BUT3_PIN);
    EnableExtiIRQn(BUT3_PIN,1);
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
	wss.bit_width = WS_8_BIT;
	wss.col_pos = WS_GRB;
    ws.Init(wss);
    ws.SetPixelColor(0,Color(0,0,255));
    ws.Colorize();
}

void InitTIM()
{
	/* Debounce Timeout TIM*/
    LL_TIM_InitTypeDef tim;
    tim.Autoreload = 15;
    tim.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    tim.CounterMode = LL_TIM_COUNTERMODE_UP;
    tim.Prescaler = SystemCoreClock/1000;
    tim.RepetitionCounter = 0;
    LL_TIM_Init(BUT_TIM,&tim);
    LL_TIM_SetOnePulseMode(BUT_TIM,LL_TIM_ONEPULSEMODE_SINGLE);
	
	LL_TIM_EnableIT_UPDATE(BUT_TIM);

    EnableTimIRQn(BUT_TIM,0);
	
	
	/* Animation TIM */
	tim.Autoreload = 1020;
	tim.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	tim.CounterMode = LL_TIM_COUNTERMODE_CENTER_UP_DOWN;
	tim.Prescaler = SystemCoreClock/1000;
	tim.RepetitionCounter = 0;
	LL_TIM_Init(BLINK_TIM,&tim);
	LL_TIM_EnableCounter(BLINK_TIM);
}