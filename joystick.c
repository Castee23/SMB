
#include "joystick.h"

// Iniciacion GPIO
    GPIO_InitTypeDef joystick_up = {
        .Pin = GPIO_PIN_10, 
        .Mode = GPIO_MODE_IT_RISING,
        .Pull = GPIO_NOPULL
    };
    GPIO_InitTypeDef joystick_right = {
        .Pin = GPIO_PIN_11, 
        .Mode = GPIO_MODE_IT_RISING,
        .Pull = GPIO_NOPULL
    };
    GPIO_InitTypeDef joystick_down = {
        .Pin = GPIO_PIN_12, 
        .Mode = GPIO_MODE_IT_RISING,
        .Pull = GPIO_NOPULL
    };
    GPIO_InitTypeDef joystick_left = {
        .Pin = GPIO_PIN_14, 
        .Mode = GPIO_MODE_IT_RISING,
        .Pull = GPIO_NOPULL
    };
    GPIO_InitTypeDef joystick_center = {
        .Pin = GPIO_PIN_15, 
        .Mode = GPIO_MODE_IT_RISING,
        .Pull = GPIO_NOPULL
    };

// Identificadores 
osThreadId_t         tid_Joystick;
osTimerId_t          timer_50ms;
osMessageQueueId_t   gesture_queue;

// Funciones

static  void Thread_Joystick(void *arg);
static  void Timer_Callback(void *arg);
static  void EXTI15_10_IRQHandler(void);
void         Init_Hardware_Joystick(void);
int          Init_Joystick(void);
    
// Variables globales
static  uint8_t gesto = 0;
volatile uint32_t press_start_time = 0;
volatile uint32_t press_duration = 0;
    
int Init_Joystick(void) {
    Init_Hardware_Joystick();
    timer_50ms = osTimerNew(Timer_Callback, osTimerOnce, NULL, NULL);
    tid_Joystick = osThreadNew(Thread_Joystick, NULL, NULL);
    gesture_queue = osMessageQueueNew(16, sizeof(uint8_t), NULL);
  return(0);
}

void Thread_Joystick(void *arg) {
  while (1) {
    osThreadFlagsWait(0x01, osFlagsWaitAny, osWaitForever);
    osTimerStart(timer_50ms, 50);
  }
}

void Timer_Callback (void *arg) {
  osStatus_t status;
  status=osMessageQueuePut(gesture_queue, &gesto, 0U, 0U);
}

void EXTI15_10_IRQHandler(void){
  osThreadFlagsSet(tid_Joystick, 0x01);

  if (__HAL_GPIO_EXTI_GET_IT(joystick_up.Pin) != RESET) { // arriba
      __HAL_GPIO_EXTI_CLEAR_IT(joystick_up.Pin);
      gesto = 1;
  } else if (__HAL_GPIO_EXTI_GET_IT(joystick_right.Pin) != RESET) { // derecha
      __HAL_GPIO_EXTI_CLEAR_IT(joystick_right.Pin);
      gesto = 2;
  } else if (__HAL_GPIO_EXTI_GET_IT(joystick_down.Pin) != RESET) { // abajo
      __HAL_GPIO_EXTI_CLEAR_IT(joystick_down.Pin);
      gesto = 3;
  } else if (__HAL_GPIO_EXTI_GET_IT(joystick_left.Pin) != RESET) { // izq
      __HAL_GPIO_EXTI_CLEAR_IT(joystick_left.Pin);
      gesto = 4;
  } else if (__HAL_GPIO_EXTI_GET_IT(joystick_center.Pin) != RESET) { // centro
      __HAL_GPIO_EXTI_CLEAR_IT(joystick_center.Pin);
      uint32_t current_time = osKernelGetTickCount();
      press_duration = current_time - press_start_time;
        if (press_duration > 1000) { 
          printf("Pulsacion larga detectada.\n");
          gesto = 6; 
        } else {
          printf("Pulsacion corta detectada.\n");
          gesto = 5; 
        }
      press_start_time = current_time; 
  }
}

void Init_Hardware_Joystick(void){

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);  
  
  HAL_GPIO_Init(GPIOB, &joystick_up);
  HAL_GPIO_Init(GPIOB, &joystick_right);
  HAL_GPIO_Init(GPIOE, &joystick_down);
  HAL_GPIO_Init(GPIOE, &joystick_left);
  HAL_GPIO_Init(GPIOE, &joystick_center);
}




