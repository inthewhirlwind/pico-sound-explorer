/**
 * UI Controls Implementation
 * 
 * This module handles user interface elements including buttons,
 * LEDs, and potentiometer inputs with proper debouncing.
 */

#include "ui_controls.h"
#include "adsr_envelope.h"

#define DEBOUNCE_TIME_US 50000  // 50ms debounce time

void ui_controls_init(void) {
    // Initialize button pins as inputs with pull-up resistors
    gpio_init(WAVEFORM_BUTTON_PIN);
    gpio_set_dir(WAVEFORM_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(WAVEFORM_BUTTON_PIN);
    
    gpio_init(OUTPUT_TOGGLE_PIN);
    gpio_set_dir(OUTPUT_TOGGLE_PIN, GPIO_IN);
    gpio_pull_up(OUTPUT_TOGGLE_PIN);
    
    // Initialize LED pins as outputs
    gpio_init(LED_SQUARE_PIN);
    gpio_set_dir(LED_SQUARE_PIN, GPIO_OUT);
    
    gpio_init(LED_TRIANGLE_PIN);
    gpio_set_dir(LED_TRIANGLE_PIN, GPIO_OUT);
    
    gpio_init(LED_SAWTOOTH_PIN);
    gpio_set_dir(LED_SAWTOOTH_PIN, GPIO_OUT);
    
    gpio_init(LED_SINE_PIN);
    gpio_set_dir(LED_SINE_PIN, GPIO_OUT);
    
    // Initialize ADC for potentiometer reading
    adc_init();
    adc_gpio_init(FREQUENCY_POT_PIN);
    adc_gpio_init(DUTY_CYCLE_POT_PIN);
    
    printf("UI Controls initialized\n");
}

void ui_update_buttons(sound_system_t *system) {
    uint32_t current_time = time_us_32();
    
    // Read button states (active low due to pull-up)
    bool waveform_pressed = !gpio_get(WAVEFORM_BUTTON_PIN);
    bool output_pressed = !gpio_get(OUTPUT_TOGGLE_PIN);
    
    // Handle waveform button with debouncing
    if (waveform_pressed && !system->waveform_button_pressed) {
        if ((current_time - system->last_waveform_press) > DEBOUNCE_TIME_US) {
            ui_handle_waveform_button(system);
            system->last_waveform_press = current_time;
        }
    }
    system->waveform_button_pressed = waveform_pressed;
    
    // Handle output toggle button with debouncing
    if (output_pressed && !system->output_button_pressed) {
        if ((current_time - system->last_output_press) > DEBOUNCE_TIME_US) {
            ui_handle_output_toggle(system);
            system->last_output_press = current_time;
        }
    }
    system->output_button_pressed = output_pressed;
}

void ui_read_potentiometers(sound_system_t *system) {
    static uint32_t last_read_time = 0;
    uint32_t current_time = time_us_32();
    
    // Only read ADC values every 5ms to avoid excessive CPU usage
    if ((current_time - last_read_time) < 5000) {
        return;
    }
    last_read_time = current_time;
    
    // Read frequency potentiometer
    adc_select_input(0); // FREQUENCY_POT_PIN is ADC0 (GPIO26)
    uint16_t freq_adc = adc_read();
    system->frequency = ui_adc_to_frequency(freq_adc);
    
    // Read duty cycle potentiometer
    adc_select_input(1); // DUTY_CYCLE_POT_PIN is ADC1 (GPIO27)
    uint16_t duty_adc = adc_read();
    system->duty_cycle = ui_adc_to_duty_cycle(duty_adc);
    
    // Read ADSR parameters
    adsr_read_parameters(system);
}

void ui_update_leds(sound_system_t *system) {
    // Turn off all LEDs first
    gpio_put(LED_SQUARE_PIN, false);
    gpio_put(LED_TRIANGLE_PIN, false);
    gpio_put(LED_SAWTOOTH_PIN, false);
    gpio_put(LED_SINE_PIN, false);
    
    // Turn on LED for current waveform
    switch (system->current_waveform) {
        case WAVEFORM_SQUARE:
            gpio_put(LED_SQUARE_PIN, true);
            break;
        case WAVEFORM_TRIANGLE:
            gpio_put(LED_TRIANGLE_PIN, true);
            break;
        case WAVEFORM_SAWTOOTH:
            gpio_put(LED_SAWTOOTH_PIN, true);
            break;
        case WAVEFORM_SINE:
            gpio_put(LED_SINE_PIN, true);
            break;
    }
}

void ui_handle_waveform_button(sound_system_t *system) {
    // Cycle through waveforms
    system->current_waveform = (system->current_waveform + 1) % WAVEFORM_COUNT;
    
    printf("Waveform changed to: ");
    switch (system->current_waveform) {
        case WAVEFORM_SQUARE:
            printf("Square Wave\n");
            break;
        case WAVEFORM_TRIANGLE:
            printf("Triangle Wave\n");
            break;
        case WAVEFORM_SAWTOOTH:
            printf("Sawtooth Wave\n");
            break;
        case WAVEFORM_SINE:
            printf("Sine Wave\n");
            break;
    }
    
    // Update LED indicators
    ui_update_leds(system);
}

void ui_handle_output_toggle(sound_system_t *system) {
    system->output_enabled = !system->output_enabled;
    
    printf("Audio output: %s\n", system->output_enabled ? "ON" : "OFF");
    
    if (system->output_enabled) {
        // Start ADSR envelope
        adsr_note_on(system);
    } else {
        // Release ADSR envelope
        adsr_note_off(system);
    }
}

float ui_adc_to_frequency(uint16_t adc_value) {
    // Convert ADC value to logarithmic frequency scale
    // 5 octaves from 20Hz to 20kHz
    float normalized = (float)adc_value / ADC_MAX_VALUE;
    
    // Logarithmic mapping for musical frequency control
    // log2(20000/20) = log2(1000) ≈ 9.97 (approximately 10 octaves total range)
    // We want 5 octaves, so we use half the range
    float log_freq = logf(MIN_FREQUENCY) + normalized * (logf(MAX_FREQUENCY) - logf(MIN_FREQUENCY));
    return expf(log_freq);
}

float ui_adc_to_duty_cycle(uint16_t adc_value) {
    // Convert ADC value to duty cycle (0.05 to 0.95 for practical square wave)
    float normalized = (float)adc_value / ADC_MAX_VALUE;
    return 0.05f + normalized * 0.9f;
}

float ui_adc_to_time(uint16_t adc_value) {
    // Convert ADC value to time parameter (1ms to 5s)
    float normalized = (float)adc_value / ADC_MAX_VALUE;
    return 0.001f + normalized * 4.999f;
}