#include <stdio.h>
#include <stdint.h>

#include "common.h"

//void GPIOinit(void);

/*Global Variable*/
TIM_HandleTypeDef tim17;

/*function decleration*/
void timerInit(void)
{
  /* Enabling clock signal */
  __HAL_RCC_TIM17_CLK_ENABLE();
  
  /* Enabling interrupt commands*/ 
  HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM17_IRQn,0,0);
  HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM17_IRQn);

  /* Initialise Timer with HAL*/
  tim17.Instance = TIM17;
  tim17.Init.Prescaler     = HAL_RCC_GetPCLK2Freq() / 1000000 - 1;
  tim17.Init.CounterMode   = TIM_COUNTERMODE_UP;
  tim17.Init.Period        = 0xffff;
  tim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  tim17.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&tim17);

  HAL_TIM_Base_Start_IT(&tim17); /* enabling the interrupt*/
}

void timerDelay(void)
{
uint16_t delayVal=10;
  TIM17->CNT = 0;       /* Reset counter */
  while((TIM17->CNT)< delayVal) // Used a local variable timertick to increase the delay value so that we can have a delay of more than 65535 us.
	{ 
	if(TIM17->CNT == delayVal){
	TIM17->CNT = 0;
	}
	asm volatile ("nop\n");
  }	
}

void TIM1_TRG_COM_TIM17_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&tim17);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 ,0);
}


ParserReturnVal_t CmdtimerInit(int action)
{

  if(action==CMD_SHORT_HELP) return CmdReturnOk;
  if(action==CMD_LONG_HELP) {
    printf("Initialise Timer\n\n"
	   "This command initialises the Timer\n"
	   );

    return CmdReturnOk;
  }

  timerInit();
  printf("Timer Initialised\n");

  return CmdReturnOk;
}

/*ADD_CMD("timerinit",CmdtimerInit," Timer Initialisation")

ParserReturnVal_t CmdtimerDelay(int mode)
{
  uint32_t val,rc;
  
  if(mode != CMD_INTERACTIVE) return CmdReturnOk;

  rc = fetch_uint32_arg(&val);
  if(rc) {
    printf("Must specify delay value to the user\n");
    return CmdReturnBadParameter1;
  }

  GPIOA->ODR |= 0x01;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 ,1);
  timerDelay(val);
  GPIOA->ODR &= ~0x01;
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 ,0);
  
  return CmdReturnOk;
}

ADD_CMD("timerdelay",CmdtimerDelay,"Give Timer Delay")
*/

