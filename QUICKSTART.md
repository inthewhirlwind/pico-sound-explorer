# Quick Start Guide - Pico Sound Explorer

## 🚀 Get Running in 5 Minutes

### Step 1: Hardware Setup (Minimal)
For a quick test, you only need:
- Raspberry Pi Pico 2
- Breadboard
- 2 push buttons
- 4 LEDs
- Jumper wires

**Essential Connections:**
```
GPIO0  -> Audio output (connect headphones with 1kΩ resistor)
GPIO2  -> Waveform button (to GND, use internal pull-up)
GPIO3  -> Output toggle button (to GND, use internal pull-up)
GPIO4  -> Square wave LED (with 220Ω resistor to GND)
GPIO5  -> Triangle wave LED (with 220Ω resistor to GND)
GPIO6  -> Sawtooth wave LED (with 220Ω resistor to GND)
GPIO7  -> Sine wave LED (with 220Ω resistor to GND)
```

### Step 2: Software Setup

1. **Install Pico SDK:**
```bash
git clone https://github.com/raspberrypi/pico-sdk.git
cd pico-sdk
git submodule update --init
export PICO_SDK_PATH=$PWD
```

2. **Clone and Build:**
```bash
git clone https://github.com/inthewhirlwind/pico-sound-explorer.git
cd pico-sound-explorer
chmod +x build.sh
./build.sh
```

3. **Flash to Pico:**
- Hold BOOTSEL button while plugging in USB
- Copy `build/sound_explorer.uf2` to RPI-RP2 drive
- Pico reboots automatically

### Step 3: First Test

1. **Connect USB serial monitor** (115200 baud)
2. **Press waveform button** (GPIO2) to cycle through waves
3. **Press output button** (GPIO3) to start/stop audio
4. **Connect headphones** to GPIO0 (through 1kΩ resistor)

You should see LEDs change and hear audio output!

### Step 4: Add Potentiometers (Optional)

Connect 10kΩ potentiometers to:
- GPIO26: Frequency control (20Hz-20kHz)
- GPIO27: Duty cycle (square wave only)
- GPIO28: ADSR attack time (0-2 seconds)

## 🎵 Usage

- **Waveform Button**: Cycles through Square → Triangle → Sawtooth → Sine
- **Output Button**: Toggles audio on/off with ADSR envelope
- **Serial Monitor**: Shows real-time system status

## 🔧 Troubleshooting

**No audio?**
- Check GPIO0 connection
- Verify headphones/speaker is working
- Try pressing output toggle button

**LEDs not working?**
- Check LED polarity (long leg = positive)
- Verify current-limiting resistors
- Test with multimeter

**Build fails?**
- Ensure PICO_SDK_PATH is set correctly
- Check that arm-none-eabi-gcc is installed
- Verify CMake version is 3.13+

**Buttons not responding?**
- Check ground connections
- Try external pull-up resistors (10kΩ)
- Verify button wiring

## 📝 Next Steps

1. **Add audio filter** for cleaner output
2. **Connect all potentiometers** for full control
3. **Build enclosure** for portable use
4. **Experiment with parameters** in the code
5. **Add MIDI input** or other interfaces

For complete documentation, see [README.md](README.md)

## 🎛️ Default Settings

- **Sample Rate**: 44.1kHz
- **Frequency**: 440Hz (A4)
- **Duty Cycle**: 50%
- **ADSR**: 100ms attack, 300ms decay, 70% sustain, 500ms release
- **Waveform**: Square wave

Enjoy making music with your Pico! 🎶