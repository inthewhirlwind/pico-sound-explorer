#ifndef ADSR_ENVELOPE_H
#define ADSR_ENVELOPE_H

#include "sound_explorer.h"

/**
 * Initialize ADSR envelope system
 */
void adsr_envelope_init(void);

/**
 * Start the ADSR envelope (note on)
 * @param system Pointer to the sound system state
 */
void adsr_note_on(sound_system_t *system);

/**
 * Release the ADSR envelope (note off)
 * @param system Pointer to the sound system state
 */
void adsr_note_off(sound_system_t *system);

/**
 * Update ADSR envelope state and calculate current envelope level
 * @param system Pointer to the sound system state
 * @param delta_time Time elapsed since last update (in seconds)
 */
void adsr_update(sound_system_t *system, float delta_time);

/**
 * Get current envelope multiplier
 * @param system Pointer to the sound system state
 * @return Envelope level (0.0-1.0)
 */
float adsr_get_level(sound_system_t *system);

/**
 * Read ADSR parameters from potentiometers
 * @param system Pointer to the sound system state
 */
void adsr_read_parameters(sound_system_t *system);

#endif // ADSR_ENVELOPE_H