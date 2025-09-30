# Raspberry Pi Pico 2 Sound Explorer

A comprehensive sound generation system for the Raspberry Pi Pico 2 featuring multiple waveforms, ADSR envelope control, and real-time parameter adjustment.

## Features

- **Multiple Waveforms**: Square wave (variable duty cycle), Triangle wave, Sawtooth wave, Sine wave
- **Frequency Control**: Variable frequency across 5 octaves (20Hz - 20kHz)
- **ADSR Envelope**: Attack, Decay, Sustain, Release envelope control with attack time potentiometer adjustment
- **User Interface**: Button controls with LED indicators and proper debouncing
- **UART Communication**: Real-time status reporting and system information
- **PWM Audio Output**: High-quality 8-bit PWM audio generation at 44.1kHz

## Hardware Requirements

### Components
- Raspberry Pi Pico 2
- 2x Push buttons (waveform selection, output toggle)
- 4x LEDs (waveform indicators)
- 3x Potentiometers (frequency, duty cycle, ADSR attack)
- Resistors for LED current limiting (220Ω recommended)
- Pull-up resistors for buttons (10kΩ, optional if using internal pull-ups)
- Audio output circuitry (RC low-pass filter recommended)

### Pin Configuration

| GPIO Pin | Function | Description |
|----------|----------|-------------|
| GPIO0 | PWM Output | Audio output signal |
| GPIO2 | Button Input | Waveform selection button |
| GPIO3 | Button Input | Output toggle button |
| GPIO4 | LED Output | Square wave indicator |
| GPIO5 | LED Output | Triangle wave indicator |
| GPIO6 | LED Output | Sawtooth wave indicator |
| GPIO7 | LED Output | Sine wave indicator |
| GPIO26 | ADC Input | Frequency control potentiometer |
| GPIO27 | ADC Input | Duty cycle control potentiometer |
| GPIO28 | ADC Input | ADSR Attack time potentiometer |

### Circuit Diagram

```
                    +3.3V
                      |
                     10kΩ
                      |
GPIO2 --------[Button]---GND

                    +3.3V
                      |
                     10kΩ
                      |
GPIO3 --------[Button]---GND

GPIO4 ----[220Ω]----[LED]----GND
GPIO5 ----[220Ω]----[LED]----GND
GPIO6 ----[220Ω]----[LED]----GND
GPIO7 ----[220Ω]----[LED]----GND

             +3.3V
               |
            [10kΩ Pot]
               |
GPIO26 --------+--------GND

             +3.3V
               |
            [10kΩ Pot]
               |
GPIO27 --------+--------GND

             +3.3V
               |
            [10kΩ Pot]
               |
GPIO28 --------+--------GND

GPIO0 ----[1kΩ]----+----[100nF]----GND (Audio Output)
                   |
                Audio Out
```

## Software Architecture

### Core Modules

1. **Waveform Generator** (`waveform_generator.c`)
   - PWM-based audio output
   - Multiple waveform algorithms
   - Real-time sample generation
   - Phase accumulator for frequency control

2. **ADSR Envelope** (`adsr_envelope.c`)
   - Attack, Decay, Sustain, Release processing
   - Real-time envelope calculation
   - Potentiometer-controlled parameters
   - State machine implementation

3. **UI Controls** (`ui_controls.c`)
   - Button debouncing
   - LED indicators
   - Potentiometer reading
   - User input handling

4. **UART Communication** (`uart_comm.c`)
   - Status reporting
   - System information display
   - Real-time monitoring

## Building and Installation

### Prerequisites

1. **Raspberry Pi Pico SDK**: Install the official Pico SDK
   ```bash
   git clone https://github.com/raspberrypi/pico-sdk.git
   cd pico-sdk
   git submodule update --init
   export PICO_SDK_PATH=$PWD
   ```

2. **Build Tools**: Ensure you have CMake and arm-none-eabi-gcc installed
   ```bash
   # Ubuntu/Debian
   sudo apt update
   sudo apt install cmake gcc-arm-none-eabi libnewlib-arm-none-eabi

   # macOS (with Homebrew)
   brew install cmake
   brew tap ArmMbed/homebrew-formulae
   brew install arm-none-eabi-gcc
   ```

### Build Process

1. **Clone the repository**:
   ```bash
   git clone https://github.com/inthewhirlwind/pico-sound-explorer.git
   cd pico-sound-explorer
   ```

2. **Create build directory**:
   ```bash
   mkdir build
   cd build
   ```

3. **Configure CMake**:
   ```bash
   cmake ..
   ```

4. **Build the project**:
   ```bash
   make -j4
   ```

5. **Flash to Pico**:
   - Hold the BOOTSEL button while connecting the Pico to USB
   - Copy `sound_explorer.uf2` to the RPI-RP2 drive
   - The Pico will automatically reboot and run the program

### Development Workflow

For iterative development:
```bash
# After making code changes
cd build
make -j4
# Flash the new .uf2 file to the Pico
```

## Usage

### Basic Operation

1. **Power On**: Connect the Pico to USB power
2. **Waveform Selection**: Press the waveform button (GPIO2) to cycle through:
   - Square wave (LED on GPIO4)
   - Triangle wave (LED on GPIO5)
   - Sawtooth wave (LED on GPIO6)
   - Sine wave (LED on GPIO7)
3. **Output Control**: Press the output toggle button (GPIO3) to turn audio on/off
4. **Parameter Adjustment**: Use potentiometers to control:
   - Frequency (GPIO26): 20Hz to 20kHz
   - Duty cycle (GPIO27): Square wave duty cycle
   - ADSR Attack (GPIO28): Attack time 0-2 seconds

### UART Monitoring

Connect to the Pico's USB serial port (typically /dev/ttyACM0 on Linux) at 115200 baud to see:
- System startup information
- Real-time parameter changes
- ADSR state transitions
- Periodic status updates

Example output:
```
=====================================
  Raspberry Pi Pico 2 Sound Explorer
=====================================

System ready!

--- System Status ---
Waveform: Square
Frequency: 440.0 Hz
Duty Cycle: 50.0%
Output: ON

ADSR Parameters:
  Attack: 0.100 s
  Decay: 0.200 s
  Sustain: 70.0%
  Release: 0.500 s
ADSR State: Attack
Envelope Level: 45.2%
--------------------
```

## Technical Details

### Audio Generation
- **Sample Rate**: 44.1kHz
- **Resolution**: 8-bit PWM
- **Frequency Range**: 20Hz - 20kHz (5 octaves)
- **Waveform Algorithms**: 
  - Square: Duty cycle comparison
  - Triangle: Linear ramp up/down
  - Sawtooth: Linear ramp
  - Sine: 256-entry lookup table

### ADSR Envelope
- **Attack**: Linear rise from 0 to 100%
- **Decay**: Linear fall from 100% to sustain level
- **Sustain**: Constant level until note off
- **Release**: Linear fall from sustain to 0%
- **Update Rate**: 1kHz for smooth envelope changes

### Button Debouncing
- **Debounce Time**: 50ms
- **Method**: Software debouncing with timestamp checking
- **Non-Retriggering**: Prevents multiple triggers from single press

## Customization

### Modifying Waveforms
Add new waveforms by:
1. Adding enum value to `waveform_type_t` in `sound_explorer.h`
2. Implementing generation function in `waveform_generator.c`
3. Adding case to `generate_waveform_sample()`
4. Updating UI to handle new waveform

### Extending ADSR
- Modify time ranges in `ui_adc_to_time()`
- Add more ADC inputs for sustain/release control
- Implement exponential instead of linear curves

### Audio Output Enhancement
- Add RC low-pass filter for smoother output
- Implement higher resolution PWM (10-bit, 12-bit)
- Add multiple voices/polyphony

## Troubleshooting

### Build Issues
- Ensure PICO_SDK_PATH is set correctly
- Check that all submodules are initialized
- Verify CMake version is 3.13 or higher

### Hardware Issues
- Check all connections match the pin configuration
- Verify power supply is adequate (USB should be sufficient)
- Test potentiometers with multimeter
- Ensure LEDs have current-limiting resistors

### Audio Issues
- Check PWM output on oscilloscope
- Verify audio filter components
- Ensure proper grounding
- Check for electromagnetic interference

## Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Raspberry Pi Foundation for the Pico 2 and SDK
- Community contributors and testers
- Audio programming references and tutorials
