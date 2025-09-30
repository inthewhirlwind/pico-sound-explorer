#ifndef WAVEFORM_GENERATOR_H
#define WAVEFORM_GENERATOR_H

#include "sound_explorer.h"

/**
 * Initialize the waveform generator subsystem
 * Sets up PWM for audio output
 */
void waveform_generator_init(void);

/**
 * Generate a single sample for the current waveform
 * @param system Pointer to the sound system state
 * @return Sample value (0-255 for 8-bit PWM)
 */
uint8_t generate_waveform_sample(sound_system_t *system);

/**
 * Generate square wave sample with variable duty cycle
 * @param phase Current phase (0-65535)
 * @param duty_cycle Duty cycle (0.0-1.0)
 * @return Sample value (0-255)
 */
uint8_t generate_square_wave(uint16_t phase, float duty_cycle);

/**
 * Generate triangle wave sample
 * @param phase Current phase (0-65535)
 * @return Sample value (0-255)
 */
uint8_t generate_triangle_wave(uint16_t phase);

/**
 * Generate sawtooth wave sample
 * @param phase Current phase (0-65535)
 * @return Sample value (0-255)
 */
uint8_t generate_sawtooth_wave(uint16_t phase);

/**
 * Generate sine wave sample
 * @param phase Current phase (0-65535)
 * @return Sample value (0-255)
 */
uint8_t generate_sine_wave(uint16_t phase);

/**
 * Update the phase accumulator based on current frequency
 * @param system Pointer to the sound system state
 */
void update_phase_accumulator(sound_system_t *system);

/**
 * PWM interrupt handler for sample generation
 */
void pwm_interrupt_handler(void);

#endif // WAVEFORM_GENERATOR_H