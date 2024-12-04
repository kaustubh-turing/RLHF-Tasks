#include "stm32f10x.h"

#define INTERRUPT_PRIORITY_HIGH 3
#define INTERRUPT_PRIORITY_LOW 1

void TIM2_IRQHandler() {
    // High priority ISR
    // Add your code here
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
}

void TIM3_IRQHandler() {
    // Low priority ISR
    // Add your code here
    TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

int main() {
    // Initialize timers and set priorities
    NVIC_SetPriority(TIM2_IRQn, INTERRUPT_PRIORITY_HIGH);
    NVIC_SetPriority(TIM3_IRQn, INTERRUPT_PRIORITY_LOW);
    NVIC_EnableIRQ(TIM2_IRQn);
    NVIC_EnableIRQ(TIM3_IRQn);
    while (true) {
        // Main loop
    }
    return 0;
} 