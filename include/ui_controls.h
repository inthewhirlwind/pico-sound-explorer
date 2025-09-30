#ifndef UI_CONTROLS_H
#define UI_CONTROLS_H

#include "sound_explorer.h"

/**
 * Initialize UI controls (buttons, LEDs, potentiometers)
 */
void ui_controls_init(void);

/**
 * Update button states with debouncing
 * @param system Pointer to the sound system state
 */
void ui_update_buttons(sound_system_t *system);

/**
 * Read potentiometer values and update system parameters
 * @param system Pointer to the sound system state
 */
void ui_read_potentiometers(sound_system_t *system);

/**
 * Update LED indicators based on current waveform
 * @param system Pointer to the sound system state
 */
void ui_update_leds(sound_system_t *system);

/**
 * Handle waveform selection button press
 * @param system Pointer to the sound system state
 */
void ui_handle_waveform_button(sound_system_t *system);

/**
 * Handle output toggle button press
 * @param system Pointer to the sound system state
 */
void ui_handle_output_toggle(sound_system_t *system);

/**
 * Convert ADC value to frequency in the specified range
 * @param adc_value Raw ADC reading (0-4095)
 * @return Frequency in Hz (20-20000)
 */
float ui_adc_to_frequency(uint16_t adc_value);

/**
 * Convert ADC value to duty cycle
 * @param adc_value Raw ADC reading (0-4095)
 * @return Duty cycle (0.0-1.0)
 */
float ui_adc_to_duty_cycle(uint16_t adc_value);

/**
 * Convert ADC value to time parameter (for ADSR)
 * @param adc_value Raw ADC reading (0-4095)
 * @return Time in seconds (0.001-5.0)
 */
float ui_adc_to_time(uint16_t adc_value);

#endif // UI_CONTROLS_H