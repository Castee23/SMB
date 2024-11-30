#include "clock.h"

 
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Clock; 
volatile uint8_t horas = 0;
volatile uint8_t minutos = 0;
volatile uint8_t segundos = 0;

void Thread_Clock (void *argument);
 
int Init_Clock (void) {
 
  tid_Clock = osThreadNew(Thread_Clock, NULL, NULL);
  if (tid_Clock == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread_Clock (void *argument) {
 
  while (1) {
        osDelay(1000);
        segundos++;
        if (segundos == 60) {
            segundos = 0;
            minutos++;
            if (minutos == 60) {
                minutos = 0;
                horas++;
                if (horas == 24) {
                    horas = 0;
                }
            }
        }
    }
}
