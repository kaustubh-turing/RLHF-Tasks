#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t led_state = 0;

ISR(TIM0_OVF_vect) {
    // Toggle LED state in interrupt
    led_state = !led_state;
    if (led_state) {
        PORTB |= (1 << PB5);  // Turn LED on
    } else {
        PORTB &= ~(1 << PB5); // Turn LED off
    }
}

int main() {
    // Set LED pin as output
    DDRB |= (1 << PB5);
    
    // Initialize timer
    TCCR0A = (1 << WGM01); // CTC mode
    TCCR0B = (1 << CS01);  // Prescaler: 8
    OCR0A = 62;            // Compare value for 1kHz
    
    // Enable interrupt
    sei();
    TIMSK0 |= (1 << TOIE0);
    
    while (true) {
        // Main loop
    }
    
    return 0;
} 