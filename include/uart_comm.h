#ifndef UART_COMM_H
#define UART_COMM_H

#include "sound_explorer.h"

/**
 * Initialize UART communication
 */
void uart_comm_init(void);

/**
 * Print current system status via UART
 * @param system Pointer to the sound system state
 */
void uart_print_status(sound_system_t *system);

/**
 * Print system information on startup
 */
void uart_print_startup_info(void);

/**
 * Print waveform names
 * @param waveform Waveform type
 * @return String representation of waveform
 */
const char* uart_get_waveform_name(waveform_type_t waveform);

/**
 * Print ADSR state names
 * @param state ADSR state
 * @return String representation of ADSR state
 */
const char* uart_get_adsr_state_name(adsr_state_t state);

/**
 * Periodic status update via UART
 * @param system Pointer to the sound system state
 */
void uart_periodic_update(sound_system_t *system);

#endif // UART_COMM_H