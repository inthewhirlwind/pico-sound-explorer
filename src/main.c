/**
 * Raspberry Pi Pico 2 Sound Explorer
 * 
 * A comprehensive sound generation system featuring:
 * - Multiple waveforms (square, triangle, sawtooth, sine)
 * - Variable frequency control (20Hz - 20kHz)
 * - ADSR envelope control
 * - User interface with buttons and LEDs
 * - UART status reporting
 * 
 * Hardware connections:
 * - GPIO0: PWM audio output
 * - GPIO2: Waveform selection button
 * - GPIO3: Output toggle button
 * - GPIO4-7: LED indicators for waveforms
 * - GPIO26-29: ADC inputs for potentiometers
 */

#include "sound_explorer.h"
#include "waveform_generator.h"
#include "adsr_envelope.h"
#include "ui_controls.h"
#include "uart_comm.h"

// Global system state
sound_system_t g_sound_system = {
    .current_waveform = WAVEFORM_SQUARE,
    .frequency = 440.0f,
    .duty_cycle = 0.5f,
    .output_enabled = false,
    .phase_accumulator = 0,
    .phase_increment = 0,
    .attack_time = 0.1f,
    .decay_time = 0.2f,
    .sustain_level = 0.7f,
    .release_time = 0.3f,
    .adsr_state = ADSR_IDLE,
    .envelope_level = 0.0f,
    .adsr_timer = 0.0f,
    .waveform_button_pressed = false,
    .output_button_pressed = false,
    .last_waveform_press = 0,
    .last_output_press = 0
};

/**
 * System initialization
 */
void system_init(void) {
    // Initialize stdio for USB communication
    stdio_init_all();
    
    // Initialize subsystems
    waveform_generator_init();
    adsr_envelope_init();
    ui_controls_init();
    uart_comm_init();
    
    // Print startup information
    uart_print_startup_info();
    uart_print_status(&g_sound_system);
}

/**
 * Main system update loop
 */
void system_update(void) {
    static uint32_t last_update_time = 0;
    static uint32_t last_uart_update = 0;
    
    uint32_t current_time = time_us_32();
    float delta_time = (current_time - last_update_time) / 1000000.0f; // Convert to seconds
    
    if (delta_time > 0.001f) { // Update at ~1kHz
        // Update UI controls
        ui_update_buttons(&g_sound_system);
        ui_read_potentiometers(&g_sound_system);
        ui_update_leds(&g_sound_system);
        
        // Update ADSR envelope
        adsr_update(&g_sound_system, delta_time);
        
        // Update phase accumulator for waveform generation
        update_phase_accumulator(&g_sound_system);
        
        last_update_time = current_time;
    }
    
    // Periodic UART status update (every 5 seconds)
    if ((current_time - last_uart_update) > 5000000) {
        uart_periodic_update(&g_sound_system);
        last_uart_update = current_time;
    }
}

/**
 * Main function
 */
int main(void) {
    // Initialize the system
    system_init();
    
    printf("Pico Sound Explorer initialized successfully!\n");
    printf("Use buttons to control waveform and output.\n");
    printf("Adjust potentiometers for frequency, duty cycle, and ADSR parameters.\n");
    
    // Main loop
    while (1) {
        system_update();
        sleep_us(100); // Small delay to prevent excessive CPU usage
    }
    
    return 0;
}