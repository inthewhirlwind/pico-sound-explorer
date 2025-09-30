#ifndef SOUND_EXPLORER_H
#define SOUND_EXPLORER_H

#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/timer.h"
#include <stdio.h>
#include <math.h>

// Hardware pin definitions
#define PWM_OUTPUT_PIN 0        // GPIO0 for PWM audio output
#define FREQUENCY_POT_PIN 26    // ADC0 for frequency control
#define DUTY_CYCLE_POT_PIN 27   // ADC1 for duty cycle control
#define ADSR_ATTACK_POT_PIN 28  // ADC2 for ADSR attack
// Note: Pico 2 only has 3 ADC pins (GPIO 26, 27, 28), so ADSR decay uses a fixed value

#define WAVEFORM_BUTTON_PIN 2   // Button for waveform selection
#define OUTPUT_TOGGLE_PIN 3     // Button for output on/off

#define LED_SQUARE_PIN 4        // LED indicator for square wave
#define LED_TRIANGLE_PIN 5      // LED indicator for triangle wave
#define LED_SAWTOOTH_PIN 6      // LED indicator for sawtooth wave
#define LED_SINE_PIN 7          // LED indicator for sine wave

// System constants
#define SAMPLE_RATE 44100       // Audio sample rate
#define PWM_WRAP 255            // PWM counter wrap value (8-bit resolution)
#define MIN_FREQUENCY 20        // Minimum frequency in Hz
#define MAX_FREQUENCY 20000     // Maximum frequency in Hz
#define ADC_MAX_VALUE 4095      // 12-bit ADC maximum value

// Waveform types
typedef enum {
    WAVEFORM_SQUARE = 0,
    WAVEFORM_TRIANGLE,
    WAVEFORM_SAWTOOTH,
    WAVEFORM_SINE,
    WAVEFORM_COUNT
} waveform_type_t;

// ADSR envelope states
typedef enum {
    ADSR_IDLE = 0,
    ADSR_ATTACK,
    ADSR_DECAY,
    ADSR_SUSTAIN,
    ADSR_RELEASE
} adsr_state_t;

// System state structure
typedef struct {
    waveform_type_t current_waveform;
    float frequency;
    float duty_cycle;
    bool output_enabled;
    uint32_t phase_accumulator;
    uint32_t phase_increment;
    
    // ADSR parameters
    float attack_time;
    float decay_time;
    float sustain_level;
    float release_time;
    
    // ADSR state
    adsr_state_t adsr_state;
    float envelope_level;
    float adsr_timer;
    
    // Button states for debouncing
    bool waveform_button_pressed;
    bool output_button_pressed;
    uint32_t last_waveform_press;
    uint32_t last_output_press;
} sound_system_t;

// Global system state
extern sound_system_t g_sound_system;

#endif // SOUND_EXPLORER_H