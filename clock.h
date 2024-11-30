#ifndef CLOCK_H
  #define CLOCK_H
  
  #include "cmsis_os2.h"
  #include "stm32f4xx_hal.h"
  
  extern volatile uint8_t horas;
  extern volatile uint8_t minutos;
  extern volatile uint8_t segundos;
  
  extern osThreadId_t tid_Clock;
  
  int Init_Clock (void);

  #endif
  