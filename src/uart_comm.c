/**
 * UART Communication Implementation
 * 
 * This module handles status reporting and system information
 * output via UART for monitoring and debugging.
 */

#include "uart_comm.h"

void uart_comm_init(void) {
    // UART is initialized via stdio_init_all() in main
    printf("UART Communication initialized\n");
}

void uart_print_startup_info(void) {
    printf("\n");
    printf("=====================================\n");
    printf("  Raspberry Pi Pico 2 Sound Explorer\n");
    printf("=====================================\n");
    printf("\n");
    printf("Features:\n");
    printf("- Multiple waveforms: Square, Triangle, Sawtooth, Sine\n");
    printf("- Frequency range: 20Hz - 20kHz\n");
    printf("- Variable duty cycle for square wave\n");
    printf("- ADSR envelope control\n");
    printf("- Real-time parameter adjustment\n");
    printf("\n");
    printf("Hardware Controls:\n");
    printf("- GPIO2: Waveform selection button\n");
    printf("- GPIO3: Output toggle button\n");
    printf("- GPIO4-7: LED indicators\n");
    printf("- GPIO26: Frequency potentiometer\n");
    printf("- GPIO27: Duty cycle potentiometer\n");
    printf("- GPIO28: ADSR Attack potentiometer\n");
    printf("- GPIO29: ADSR Decay potentiometer\n");
    printf("- GPIO0: PWM audio output\n");
    printf("\n");
    printf("System ready!\n");
    printf("=====================================\n");
    printf("\n");
}

const char* uart_get_waveform_name(waveform_type_t waveform) {
    switch (waveform) {
        case WAVEFORM_SQUARE:   return "Square";
        case WAVEFORM_TRIANGLE: return "Triangle";
        case WAVEFORM_SAWTOOTH: return "Sawtooth";
        case WAVEFORM_SINE:     return "Sine";
        default:                return "Unknown";
    }
}

const char* uart_get_adsr_state_name(adsr_state_t state) {
    switch (state) {
        case ADSR_IDLE:    return "Idle";
        case ADSR_ATTACK:  return "Attack";
        case ADSR_DECAY:   return "Decay";
        case ADSR_SUSTAIN: return "Sustain";
        case ADSR_RELEASE: return "Release";
        default:           return "Unknown";
    }
}

void uart_print_status(sound_system_t *system) {
    printf("\n--- System Status ---\n");
    printf("Waveform: %s\n", uart_get_waveform_name(system->current_waveform));
    printf("Frequency: %.1f Hz\n", system->frequency);
    printf("Duty Cycle: %.1f%%\n", system->duty_cycle * 100.0f);
    printf("Output: %s\n", system->output_enabled ? "ON" : "OFF");
    printf("\nADSR Parameters:\n");
    printf("  Attack: %.3f s\n", system->attack_time);
    printf("  Decay: %.3f s\n", system->decay_time);
    printf("  Sustain: %.1f%%\n", system->sustain_level * 100.0f);
    printf("  Release: %.3f s\n", system->release_time);
    printf("ADSR State: %s\n", uart_get_adsr_state_name(system->adsr_state));
    printf("Envelope Level: %.1f%%\n", system->envelope_level * 100.0f);
    printf("Phase: 0x%08X\n", system->phase_accumulator);
    printf("--------------------\n\n");
}

void uart_periodic_update(sound_system_t *system) {
    static bool first_update = true;
    
    if (first_update) {
        printf("Starting periodic status updates...\n");
        first_update = false;
    }
    
    printf("Status Update - ");
    printf("Waveform: %s, ", uart_get_waveform_name(system->current_waveform));
    printf("Freq: %.1fHz, ", system->frequency);
    printf("Output: %s, ", system->output_enabled ? "ON" : "OFF");
    printf("ADSR: %s (%.1f%%)\n", 
           uart_get_adsr_state_name(system->adsr_state),
           system->envelope_level * 100.0f);
}