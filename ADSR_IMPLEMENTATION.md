# ADSR Implementation Notes

## Overview
This implementation adds full 4-parameter ADSR (Attack, Decay, Sustain, Release) control to the Pico Sound Explorer using analog multiplexing to overcome the 4 ADC pin limitation.

## Hardware Solution
- **Total ADC channels needed**: 6 (frequency, duty cycle, attack, decay, sustain, release) 
- **Available ADC pins**: 4 (GPIO26-29)
- **Solution**: Analog multiplexer (CD4053) controlled by GPIO22

## Pin Assignments
- **GPIO26** (ADC0): Frequency potentiometer OR Sustain potentiometer (multiplexed)
- **GPIO27** (ADC1): Duty cycle potentiometer OR Release potentiometer (multiplexed)  
- **GPIO28** (ADC2): Attack potentiometer (dedicated)
- **GPIO29** (ADC3): Decay potentiometer (dedicated)
- **GPIO22**: Multiplexer select (LOW=freq/duty, HIGH=sustain/release)

## Parameter Ranges
- **Attack**: 0-2 seconds (linear scaling)
- **Decay**: 0-2 seconds (linear scaling)
- **Sustain**: 0-100% level (linear scaling)
- **Release**: 0-5 seconds (linear scaling)

## Implementation Details
- Multiplexer switching occurs in `adsr_read_parameters()` with 10µs settling time
- UI controls read frequency/duty cycle first, then ADSR parameters
- Both `ui_controls_init()` and `adsr_envelope_init()` share ADC initialization
- UART output updated to reflect multiplexed configuration

## Testing
To test this implementation:
1. Connect 6 potentiometers as specified in hardware_schematic.txt
2. Connect CD4053 multiplexer with GPIO22 as select line
3. Monitor UART output to verify all parameters are being read
4. Test ADSR envelope behavior with different parameter settings