#ifndef JOYSTICK_H
 #define JOYSTICK_H
 
 #include "cmsis_os2.h"
 #include "stm32f4xx_hal.h"
 #include <stdio.h>
 
  extern volatile uint32_t press_start_time;
  extern volatile uint32_t press_duration;
 
  // Declaración de la estructura JOYSTICK global
  extern GPIO_InitTypeDef joystick_up;
  extern GPIO_InitTypeDef joystick_right;
  extern GPIO_InitTypeDef joystick_down;
  extern GPIO_InitTypeDef joystick_left;
  extern GPIO_InitTypeDef joystick_centre;
  
  void   Init_Hardware_Joystick(void);
  int    Init_Joystick(void);
  
  extern osThreadId_t       tid_Joystick;
  extern osMessageQueueId_t gesture_queue;
  extern osThreadId_t       timer_50ms;
 
 #endif
 