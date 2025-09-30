/**
 * ADSR Envelope Implementation
 * 
 * This module implements Attack, Decay, Sustain, Release envelope
 * processing for dynamic amplitude control of generated waveforms.
 */

#include "adsr_envelope.h"

void adsr_envelope_init(void) {
    // Initialize ADC for reading potentiometer values
    adc_init();
    
    // Make sure GPIO pins are high-impedance, no pullups etc
    adc_gpio_init(ADSR_ATTACK_POT_PIN);  // GPIO28 - ADC2
    adc_gpio_init(ADSR_DECAY_POT_PIN);   // GPIO29 - ADC3
    // Note: Sustain and Release will use multiplexed ADC0 and ADC1
    // Those pins are initialized in ui_controls_init()
    
    // Initialize multiplexer select pin
    gpio_init(MUX_SELECT_PIN);
    gpio_set_dir(MUX_SELECT_PIN, GPIO_OUT);
    gpio_put(MUX_SELECT_PIN, false); // Start with frequency/duty cycle mode
}

void adsr_note_on(sound_system_t *system) {
    system->adsr_state = ADSR_ATTACK;
    system->adsr_timer = 0.0f;
    
    printf("ADSR: Note ON - Starting attack phase\n");
}

void adsr_note_off(sound_system_t *system) {
    if (system->adsr_state != ADSR_IDLE) {
        system->adsr_state = ADSR_RELEASE;
        system->adsr_timer = 0.0f;
        
        printf("ADSR: Note OFF - Starting release phase\n");
    }
}

void adsr_update(sound_system_t *system, float delta_time) {
    system->adsr_timer += delta_time;
    
    switch (system->adsr_state) {
        case ADSR_IDLE:
            system->envelope_level = 0.0f;
            break;
            
        case ADSR_ATTACK:
            if (system->attack_time > 0.0f) {
                system->envelope_level = system->adsr_timer / system->attack_time;
                if (system->envelope_level >= 1.0f) {
                    system->envelope_level = 1.0f;
                    system->adsr_state = ADSR_DECAY;
                    system->adsr_timer = 0.0f;
                }
            } else {
                // Instant attack
                system->envelope_level = 1.0f;
                system->adsr_state = ADSR_DECAY;
                system->adsr_timer = 0.0f;
            }
            break;
            
        case ADSR_DECAY:
            if (system->decay_time > 0.0f) {
                float decay_progress = system->adsr_timer / system->decay_time;
                if (decay_progress >= 1.0f) {
                    system->envelope_level = system->sustain_level;
                    system->adsr_state = ADSR_SUSTAIN;
                    system->adsr_timer = 0.0f;
                } else {
                    // Linear interpolation from 1.0 to sustain_level
                    system->envelope_level = 1.0f - decay_progress * (1.0f - system->sustain_level);
                }
            } else {
                // Instant decay
                system->envelope_level = system->sustain_level;
                system->adsr_state = ADSR_SUSTAIN;
                system->adsr_timer = 0.0f;
            }
            break;
            
        case ADSR_SUSTAIN:
            system->envelope_level = system->sustain_level;
            // Stay in sustain until note off
            break;
            
        case ADSR_RELEASE:
            if (system->release_time > 0.0f) {
                float release_progress = system->adsr_timer / system->release_time;
                if (release_progress >= 1.0f) {
                    system->envelope_level = 0.0f;
                    system->adsr_state = ADSR_IDLE;
                    system->adsr_timer = 0.0f;
                } else {
                    // Linear fade from current level to 0
                    float start_level = system->sustain_level;
                    system->envelope_level = start_level * (1.0f - release_progress);
                }
            } else {
                // Instant release
                system->envelope_level = 0.0f;
                system->adsr_state = ADSR_IDLE;
                system->adsr_timer = 0.0f;
            }
            break;
    }
    
    // Clamp envelope level to valid range
    if (system->envelope_level < 0.0f) system->envelope_level = 0.0f;
    if (system->envelope_level > 1.0f) system->envelope_level = 1.0f;
}

float adsr_get_level(sound_system_t *system) {
    return system->envelope_level;
}

void adsr_read_parameters(sound_system_t *system) {
    static uint32_t last_read_time = 0;
    uint32_t current_time = time_us_32();
    
    // Only read ADC values every 10ms to avoid excessive CPU usage
    if ((current_time - last_read_time) < 10000) {
        return;
    }
    last_read_time = current_time;
    
    // Read attack time potentiometer
    adc_select_input(2); // ADSR_ATTACK_POT_PIN is ADC2 (GPIO28)
    uint16_t attack_adc = adc_read();
    system->attack_time = (float)attack_adc / ADC_MAX_VALUE * 2.0f; // 0 to 2 seconds
    
    // Read decay time potentiometer  
    adc_select_input(3); // ADSR_DECAY_POT_PIN is ADC3 (GPIO29)
    uint16_t decay_adc = adc_read();
    system->decay_time = (float)decay_adc / ADC_MAX_VALUE * 2.0f; // 0 to 2 seconds
    
    // Read sustain and release using multiplexer
    // Switch to ADSR mode (sustain/release)
    gpio_put(MUX_SELECT_PIN, true);
    sleep_us(10); // Allow mux to settle
    
    // Read sustain level potentiometer (multiplexed on ADC0)
    adc_select_input(0);
    uint16_t sustain_adc = adc_read();
    system->sustain_level = (float)sustain_adc / ADC_MAX_VALUE; // 0 to 100%
    
    // Read release time potentiometer (multiplexed on ADC1)
    adc_select_input(1);
    uint16_t release_adc = adc_read();
    system->release_time = (float)release_adc / ADC_MAX_VALUE * 5.0f; // 0 to 5 seconds
    
    // Switch back to frequency/duty cycle mode
    gpio_put(MUX_SELECT_PIN, false);
}