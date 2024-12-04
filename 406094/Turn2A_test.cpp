#include <avr/io.h>
#include <avr/interrupt.h>

#define INTERRUPT_PRIORITY_HIGH 1
#define INTERRUPT_PRIORITY_LOW 0

volatile unsigned char interruptPriority = INTERRUPT_PRIORITY_LOW;

ISR(TIMER0_COMPA_vect) {
    if (interruptPriority == INTERRUPT_PRIORITY_LOW) {
        // Low priority ISR
        interruptPriority = INTERRUPT_PRIORITY_HIGH;
        // Add your code here
        interruptPriority = INTERRUPT_PRIORITY_LOW;
    }
}

ISR(TIMER1_COMPA_vect) {
    if (interruptPriority == INTERRUPT_PRIORITY_LOW) {
        interruptPriority = INTERRUPT_PRIORITY_HIGH;
        // High priority ISR
        // Add your code here
        interruptPriority = INTERRUPT_PRIORITY_LOW;
    }
}

int main() {
    // Initialize timers and enable interrupts
    sei();
    while (true) {
        // Main loop
    }
    return 0;
} 