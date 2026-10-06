/* Includes -- STD -----------------------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>

/* Includes -- HAL -----------------------------------------------------------*/
#include "driver/gpio.h"

/* Includes -- BSP -----------------------------------------------------------*/
#include "bsp.h"
#include "bspPins.h"

/* References ----------------------------------------------------------------*/
extern uint16_t adcValues[16];

typedef struct
{
    int32_t position;
    uint8_t state;
    uint8_t pinData;
} bspEncoder_t;

bspEncoder_t encoders[3] = { 0 };

// static void processEncLogic(uint8_t index);

typedef struct
{
    uint8_t       pin;
    bool          state;
    gpio_config_t conf;
} bspIO_pin_entry_t;

bspIO_pin_entry_t conf_table[NUMBER_OF_PINS]
    = { { .pin   = 5,
          .state = 1,
          .conf  = { .intr_type    = GPIO_INTR_DISABLE,
                     .mode         = GPIO_MODE_OUTPUT,
                     .pin_bit_mask = 0x1 << 5,
                     .pull_down_en = GPIO_PULLDOWN_DISABLE,
                     .pull_up_en   = GPIO_PULLUP_DISABLE } },
        { .pin   = 0,
          .state = 1,
          .conf  = { .intr_type    = GPIO_INTR_DISABLE,
                     .mode         = GPIO_MODE_INPUT,
                     .pin_bit_mask = 0x1 << 0,
                     .pull_down_en = GPIO_PULLDOWN_DISABLE,
                     .pull_up_en   = GPIO_PULLUP_DISABLE } } };

/* Functions -----------------------------------------------------------------*/
//// IRQ handlers -- save pin data and update the pin.
// void EXTI15_10_IRQHandler(void)
//{
//	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_13) != RESET)
//	{
//		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_13);
//		//Save pin data
//		encoders[1].pinData = ((LL_GPIO_ReadInputPort((GPIO_TypeDef
//*)GPIOB_BASE) >> 13) & 0x0001) << 1; // pin PB13
// encoders[1].pinData |=
//((LL_GPIO_ReadInputPort((GPIO_TypeDef *)GPIOC_BASE) >> 4) & 0x0001) << 0; //
// pin PC4
//		//bspPrintf("EXTI 15_10, 13: 0x%02X\n", encoders[1].pinData);
//		//Process the state machine
//		processEncLogic(1);
//	}
//	else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_14) != RESET)
//	{
//		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_14);
//		encoders[0].pinData = ((LL_GPIO_ReadInputPort((GPIO_TypeDef
//*)GPIOB_BASE) >> 15) & 0x0001) << 0; // pin PB15
// encoders[0].pinData |=
//((LL_GPIO_ReadInputPort((GPIO_TypeDef *)GPIOB_BASE) >> 14) & 0x0001) << 1; //
// pin PB14
//		//bspPrintf("EXTI 15_10, 14: 0x%02X\n", encoders[0].pinData);
//		processEncLogic(0);
//	}
//	else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_15) != RESET)
//	{
//		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_15);
//		encoders[0].pinData = ((LL_GPIO_ReadInputPort((GPIO_TypeDef
//*)GPIOB_BASE) >> 15) & 0x0001) << 0; // pin PB15
// encoders[0].pinData |=
//((LL_GPIO_ReadInputPort((GPIO_TypeDef *)GPIOB_BASE) >> 14) & 0x0001) << 1; //
// pin PB14
//		//bspPrintf("EXTI 15_10, 15: 0x%02X\n", encoders[0].pinData);
//		processEncLogic(0);
//	}
// }
//
// void EXTI1_IRQHandler(void)
//{
//	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
//	{
//		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
//		//Save pin data
//		encoders[2].pinData = ((LL_GPIO_ReadInputPort((GPIO_TypeDef
//*)GPIOB_BASE) >> 1) & 0x0001) << 0; // pin PB1
// encoders[2].pinData |=
//((LL_GPIO_ReadInputPort((GPIO_TypeDef *)GPIOB_BASE) >> 2) & 0x0001) << 1; //
// pin PB2
//		//bspPrintf("EXTI 1: 0x%02X\n", encoders[2].pinData);
//		//Process the state machine
//		processEncLogic(2);
//	}
// }
//
// void EXTI2_IRQHandler(void)
//{
//	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
//	{
//		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
//		//Save pin data
//		encoders[2].pinData = ((LL_GPIO_ReadInputPort((GPIO_TypeDef
//*)GPIOB_BASE) >> 1) & 0x0001) << 0; // pin PB1
// encoders[2].pinData |=
//((LL_GPIO_ReadInputPort((GPIO_TypeDef *)GPIOB_BASE) >> 2) & 0x0001) << 1; //
// pin PB2
//		//bspPrintf("EXTI 2: 0x%02X\n", encoders[2].pinData);
//		//Process the state machine
//		processEncLogic(2);
//	}
// }
//
// void EXTI4_IRQHandler(void)
//{
//	if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_4) != RESET)
//	{
//		LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_4);
//		//Save pin data
//		encoders[1].pinData = ((LL_GPIO_ReadInputPort((GPIO_TypeDef
//*)GPIOB_BASE) >> 13) & 0x0001) << 1; // pin PB13
// encoders[1].pinData |=
//((LL_GPIO_ReadInputPort((GPIO_TypeDef *)GPIOC_BASE) >> 4) & 0x0001) << 0; //
// pin PC4
//		//bspPrintf("EXTI 4: 0x%02X\n", encoders[1].pinData);
//		//Process the state machine
//		processEncLogic(1);
//	}
// }
//
// void bspEncInit(void)
//{
//	//Capture the state of the pins at the beginning
//	encoders[0].pinData = ((LL_GPIO_ReadInputPort((GPIO_TypeDef
//*)GPIOB_BASE) >> 15) & 0x0001) << 0; // pin PB15 	encoders[0].pinData |=
//((LL_GPIO_ReadInputPort((GPIO_TypeDef *)GPIOB_BASE) >> 14) & 0x0001) << 1; //
// pin PB14 	processEncLogic(0); 	encoders[1].pinData =
//((LL_GPIO_ReadInputPort((GPIO_TypeDef *)GPIOB_BASE) >> 13) & 0x0001) << 1; //
// pin PB13 	encoders[1].pinData |= ((LL_GPIO_ReadInputPort((GPIO_TypeDef
//*)GPIOC_BASE) >> 4) & 0x0001) << 0; // pin PC4 	processEncLogic(1);
//	encoders[2].pinData = ((LL_GPIO_ReadInputPort((GPIO_TypeDef
//*)GPIOB_BASE) >> 1) & 0x0001) << 0; // pin PB1 	encoders[2].pinData |=
//((LL_GPIO_ReadInputPort((GPIO_TypeDef *)GPIOB_BASE) >> 2) & 0x0001) << 1; //
// pin PB2 	processEncLogic(2);
// }
//
// static void processEncLogic(uint8_t index)
//{
//	//00
//	//01  ^
//	//11  |
//	//10
//
//	//
//	//16 possible states 4 new 4 old
//	uint8_t nextState = encoders[index].state & 3;
//	nextState |= ((encoders[index].pinData << 2) & 0xC);
//	switch (nextState) {
//		case 0:
//		case 5:
//		case 10:
//		case 15:
//		break;
//		case 1:
//		case 7:
//		case 8:
//		case 14:
//		encoders[index].position--;
//		break;
//		case 2:
//		case 4:
//		case 11:
//		case 13:
//		encoders[index].position++;
//		break;
//		case 3:
//		case 12:
//		encoders[index].position -= 2;
//		break;
//		default:
//		encoders[index].position += 2;
//		break;
//	}
//// ***** DEBUG STATE HERE ***** //
// #if 0
//	int tempBits[4] = {0};
//	tempBits[0] = nextState & 0x01;
//	tempBits[1] = (nextState >> 1) & 0x01;
//	tempBits[2] = (nextState >> 2) & 0x01;
//	tempBits[3] = (nextState >> 3) & 0x01;
//	bspPrintf("ENCODER[%d] %d%d%d%d: 0x%02X, 0x%02X, %d\n", index,
// tempBits[3], tempBits[2], tempBits[1], tempBits[0], encoders[index].state,
// nextState, encoders[index].position); #endif 	encoders[index].state =
//(nextState >> 2);
// }

// defaulted in bsp, this can be used to override pulls
void
bspIOPinInit (uint8_t pin, uint8_t pullMode)
{

    if (pin >= NUMBER_OF_PINS)
    {
        printf("%s: bad pin %d\n", __func__, pin);
        return;
    }

    gpio_config_t *pConf = &(conf_table[pin].conf);

    printf("%s: intr_type %d\n", __func__, pConf->intr_type);
    printf("%s: mode %d\n", __func__, pConf->mode);
    printf("%s: pin_bit_mask %llu\n", __func__, pConf->pin_bit_mask);
    printf("%s: pull_down_en %d\n", __func__, pConf->pull_down_en);
    printf("%s: pull_up_en %d\n", __func__, pConf->pull_up_en);

    int ret = gpio_config(pConf);
    bspIOPinWrite(pin, conf_table[pin].state);
    if (ret != 0)
    {
        printf("%s: config failed %d\n", __func__, pin);
    }

    printf("%s: exit\n", __func__);
}

void
bspIOPinWrite (uint8_t pin, bool value)
{
    // printf("%s: pin %d\n", __func__, pin);
    // printf("%s: value %d\n", __func__, value);
    if (pin >= NUMBER_OF_PINS)
    {
        printf("%s: bad pin %d\n", __func__, pin);
        return;
    }
    gpio_set_level(conf_table[pin].pin, value);
    conf_table[pin].state = value;
}

bool
bspIOPinRead (uint8_t pin)
{
    if (pin >= NUMBER_OF_PINS)
    {
        printf("%s: bad pin %d\n", __func__, pin);
        return false;
    }
    if (!(conf_table[pin].conf.mode & GPIO_MODE_DEF_INPUT))
    {
        // can't read, use state
        return conf_table[pin].state;
    }
    if (gpio_get_level(conf_table[pin].pin))
    {
        return true;
    }
    return false;
}

// perform the read operation on the selected analog pin.
uint32_t
bspIOPinReadAnalog (uint8_t pin)
{
    if (pin < 16)
    {
        return adcValues[pin];
    }
    return 0;
}

int32_t
bspIOEncRead (uint8_t index)
{
    if (index > 2)
    {
        return -1;
    }
    return encoders[index].position;
}

void
bspIOEncWrite (uint8_t index, int32_t value)
{
    if (index > 2)
    {
        return;
    }
    encoders[index].position = value;
}
