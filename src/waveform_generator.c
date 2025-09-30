/**
 * Waveform Generator Implementation
 * 
 * This module handles PWM-based audio generation with support for
 * multiple waveform types and real-time parameter control.
 */

#include "waveform_generator.h"
#include "adsr_envelope.h"

// Sine wave lookup table (256 entries for efficiency)
static const uint8_t sine_table[256] = {
    128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170, 173,
    176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211, 213, 215,
    218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240, 241, 243, 244,
    245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254, 254, 255, 255, 255,
    255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251, 250, 250, 249, 248, 246,
    245, 244, 243, 241, 240, 238, 237, 235, 234, 232, 230, 228, 226, 224, 222, 220,
    218, 215, 213, 211, 208, 206, 203, 201, 198, 196, 193, 190, 188, 185, 182, 179,
    176, 173, 170, 167, 165, 162, 158, 155, 152, 149, 146, 143, 140, 137, 134, 131,
    128, 124, 121, 118, 115, 112, 109, 106, 103, 100,  97,  93,  90,  88,  85,  82,
     79,  76,  73,  70,  67,  65,  62,  59,  57,  54,  52,  49,  47,  44,  42,  40,
     37,  35,  33,  31,  29,  27,  25,  23,  21,  20,  18,  17,  15,  14,  12,  11,
     10,   9,   7,   6,   5,   5,   4,   3,   2,   2,   1,   1,   1,   0,   0,   0,
      0,   0,   0,   0,   1,   1,   1,   2,   2,   3,   4,   5,   5,   6,   7,   9,
     10,  11,  12,  14,  15,  17,  18,  20,  21,  23,  25,  27,  29,  31,  33,  35,
     37,  40,  42,  44,  47,  49,  52,  54,  57,  59,  62,  65,  67,  70,  73,  76,
     79,  82,  85,  88,  90,  93,  97, 100, 103, 106, 109, 112, 115, 118, 121, 124
};

static uint pwm_slice_num;

void waveform_generator_init(void) {
    // Set up PWM on the audio output pin
    gpio_set_function(PWM_OUTPUT_PIN, GPIO_FUNC_PWM);
    pwm_slice_num = pwm_gpio_to_slice_num(PWM_OUTPUT_PIN);
    
    // Configure PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 1.0f); // No clock division for high frequency
    pwm_config_set_wrap(&config, PWM_WRAP);
    
    pwm_init(pwm_slice_num, &config, true);
    
    // Set up PWM interrupt for sample generation
    pwm_clear_irq(pwm_slice_num);
    pwm_set_irq_enabled(pwm_slice_num, true);
    irq_set_exclusive_handler(PWM_IRQ_WRAP, pwm_interrupt_handler);
    irq_set_enabled(PWM_IRQ_WRAP, true);
    
    // Set initial PWM frequency for audio sample rate
    // Clock speed / (wrap + 1) = sample rate
    // For 44.1kHz: 125MHz / 2834 ≈ 44.1kHz
    pwm_set_wrap(pwm_slice_num, 2833);
}

uint8_t generate_square_wave(uint16_t phase, float duty_cycle) {
    uint16_t duty_threshold = (uint16_t)(duty_cycle * 65535);
    return (phase < duty_threshold) ? 255 : 0;
}

uint8_t generate_triangle_wave(uint16_t phase) {
    if (phase < 32768) {
        // Rising edge: 0 to 255
        return (uint8_t)((phase * 255) / 32767);
    } else {
        // Falling edge: 255 to 0
        return (uint8_t)(255 - (((phase - 32768) * 255) / 32767));
    }
}

uint8_t generate_sawtooth_wave(uint16_t phase) {
    // Linear ramp from 0 to 255
    return (uint8_t)(phase >> 8);
}

uint8_t generate_sine_wave(uint16_t phase) {
    // Use lookup table for efficiency
    uint8_t table_index = phase >> 8;
    return sine_table[table_index];
}

uint8_t generate_waveform_sample(sound_system_t *system) {
    uint16_t phase = system->phase_accumulator >> 16;
    uint8_t sample = 0;
    
    // Generate the base waveform
    switch (system->current_waveform) {
        case WAVEFORM_SQUARE:
            sample = generate_square_wave(phase, system->duty_cycle);
            break;
        case WAVEFORM_TRIANGLE:
            sample = generate_triangle_wave(phase);
            break;
        case WAVEFORM_SAWTOOTH:
            sample = generate_sawtooth_wave(phase);
            break;
        case WAVEFORM_SINE:
            sample = generate_sine_wave(phase);
            break;
        default:
            sample = 128; // DC offset for silence
            break;
    }
    
    // Apply ADSR envelope
    float envelope_level = adsr_get_level(system);
    sample = (uint8_t)((sample - 128) * envelope_level + 128);
    
    return sample;
}

void update_phase_accumulator(sound_system_t *system) {
    // Calculate phase increment for current frequency
    // phase_increment = (frequency * 2^32) / sample_rate
    system->phase_increment = (uint32_t)((system->frequency * 4294967296.0f) / SAMPLE_RATE);
}

void pwm_interrupt_handler(void) {
    // Clear the interrupt
    pwm_clear_irq(pwm_slice_num);
    
    if (g_sound_system.output_enabled) {
        // Generate the next sample
        uint8_t sample = generate_waveform_sample(&g_sound_system);
        
        // Update PWM duty cycle with the sample
        pwm_set_gpio_level(PWM_OUTPUT_PIN, sample);
        
        // Update phase accumulator
        g_sound_system.phase_accumulator += g_sound_system.phase_increment;
    } else {
        // Output silence (DC bias)
        pwm_set_gpio_level(PWM_OUTPUT_PIN, 128);
    }
}