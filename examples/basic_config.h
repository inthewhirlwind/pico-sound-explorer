/**
 * Basic Configuration Example for Pico Sound Explorer
 * 
 * This file shows how to customize the sound generator parameters
 * for different use cases. Copy and modify as needed.
 */

#ifndef BASIC_CONFIG_H
#define BASIC_CONFIG_H

// Alternative pin configurations for different hardware setups
#ifdef USE_ALTERNATIVE_PINS
#define PWM_OUTPUT_PIN 15       // Alternative PWM output pin
#define WAVEFORM_BUTTON_PIN 10  // Alternative button pin
#define OUTPUT_TOGGLE_PIN 11    // Alternative button pin
#define LED_SQUARE_PIN 12       // Alternative LED pins
#define LED_TRIANGLE_PIN 13
#define LED_SAWTOOTH_PIN 14
#define LED_SINE_PIN 16
#endif

// Custom frequency ranges for specific applications
#ifdef BASS_FREQUENCY_RANGE
#define MIN_FREQUENCY 20        // Focus on bass frequencies
#define MAX_FREQUENCY 200       // Up to 200Hz only
#endif

#ifdef TREBLE_FREQUENCY_RANGE
#define MIN_FREQUENCY 2000      // Focus on treble frequencies  
#define MAX_FREQUENCY 20000     // 2kHz to 20kHz
#endif

// Alternative ADSR timing for different musical styles
#ifdef PERCUSSIVE_ADSR
// Quick attack and release for drum-like sounds
#define DEFAULT_ATTACK_TIME 0.001f   // 1ms attack
#define DEFAULT_DECAY_TIME 0.1f      // 100ms decay
#define DEFAULT_SUSTAIN_LEVEL 0.3f   // 30% sustain
#define DEFAULT_RELEASE_TIME 0.2f    // 200ms release
#endif

#ifdef PAD_ADSR
// Slow attack and release for pad sounds
#define DEFAULT_ATTACK_TIME 1.0f     // 1s attack
#define DEFAULT_DECAY_TIME 0.5f      // 500ms decay
#define DEFAULT_SUSTAIN_LEVEL 0.8f   // 80% sustain
#define DEFAULT_RELEASE_TIME 2.0f    // 2s release
#endif

// Sample rate alternatives for different quality/performance trade-offs
#ifdef HIGH_QUALITY_AUDIO
#define SAMPLE_RATE 48000       // Higher quality, more CPU usage
#define PWM_WRAP 511            // 9-bit resolution
#endif

#ifdef LOW_POWER_AUDIO
#define SAMPLE_RATE 22050       // Lower quality, less CPU usage
#define PWM_WRAP 127            // 7-bit resolution
#endif

// Waveform customization examples
#ifdef CUSTOM_SINE_TABLE_SIZE
#define SINE_TABLE_SIZE 512     // Higher resolution sine table
#endif

#ifdef ENABLE_ANTI_ALIASING
#define USE_BAND_LIMITED_WAVEFORMS  // Enable band-limited waveforms
#endif

// Debug and monitoring options
#ifdef VERBOSE_UART_OUTPUT
#define UART_UPDATE_INTERVAL_MS 1000  // More frequent updates
#define ENABLE_WAVEFORM_DEBUG         // Output waveform generation info
#endif

#endif // BASIC_CONFIG_H