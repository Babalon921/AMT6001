# AMT6001 — ADSR Synthesizer with Turbo Effects

> **University of Plymouth** · Audio & Music Technology · Programming Portfolio  
> Student: Harry Gray · October 2025

---

## Overview

A polyphonic ADSR synthesizer VST3/AU plugin built with the [JUCE](https://juce.com/) framework in C++. Features a real-time oscilloscope, ADSR envelope visualizer, harmonic control, bit-crush distortion, and a "Turbo" reverb mode — all controllable via MIDI keyboard or mouse input.

![Plugin GUI](docs/gui_preview.png)
> *Plugin running inside FL Studio 2024*

---

## Features

| Feature | Description |
|---|---|
| **8-Voice Polyphony** | Supports up to 8 simultaneous MIDI voices |
| **ADSR Envelope** | Real-time graphical envelope with Attack, Decay, Sustain, Release controls |
| **Harmonic Control** | Adjust fundamental and 2nd harmonic amplitudes independently |
| **Detune** | Shifts harmonics off-axis for chorus-like thickening |
| **Bit-Crush Distortion** | Variable bit-depth reduction for lo-fi distortion textures |
| **Turbo Mode** | One-click reverb applied to the master output |
| **Oscilloscope** | Live waveform display at 60fps |
| **MIDI Keyboard** | On-screen virtual keyboard for standalone use |
| **DAW Ready** | VST3 & AU — tested in FL Studio 2024 and validated with Pluginval |


---

## Project Structure

```
/Source
├── PluginProcessor.cpp/h   # Audio engine — MIDI rendering, effects, parameter management
├── PluginEditor.cpp/h      # GUI logic — layout, user interaction, parameter binding
├── BassNote.h              # Synthesiser voice — ADSR, harmonics, bit-crush
├── BassSound.h             # SynthesiserSound definition (JUCE requirement)
├── ADSRGraph.h             # Real-time ADSR envelope visualiser
└── Osc.h                   # Real-time oscilloscope (60fps, 512-point buffer)
```

### Signal Flow

```
MIDI Input → Parameter Change → Process Effects → GUI Feedback → Audio Output
```

---

## Technical Details

### OOP Design
- **Encapsulation** — Private member variables in `ADSRGraph.h` accessed only via setters (`setAttack()`, `setDecay()`, etc.)
- **Inheritance** — `BassNote` extends `juce::SynthesiserVoice`; editor implements `juce::Slider::Listener` and `juce::Button::Listener`
- **Polymorphism** — Virtual function overrides (`canPlaySound`, `startNote`, `renderNextBlock`) enable runtime voice dispatch
- **Abstraction** — `Osc.h` exposes a minimal interface (`pushBuffer`, `clear`) hiding internal buffer management

### Key JUCE Modules Used
`juce_core` · `juce_audio_basics` · `juce_audio_processors` · `juce_gui_basics` · `juce_graphics` · `juce_audio_utils` · `juce_dsp`

### Known Limitations
- 8-voice polyphony cap (voice stealing on 9+ simultaneous notes)
- Bit-crush only distortion (no tube/wave-shaping saturation)
- No anti-aliasing / oversampling — aliasing artefacts possible at high frequencies
- Fixed window size (may not scale correctly on high-DPI displays)
- Oscilloscope limited to 512 points (~11.6ms window at 44.1kHz)

---

## Bug Fixes & Notable Decisions

**Vector overflow** — Pre-allocated `audioPoints` to 512 elements with a wrap-around write pointer to prevent `std::vector` out-of-bounds errors in `Osc.h`.

**Phase discontinuity (clicking/popping)** — Replaced `if` with `while` for phase accumulation wrapping, guaranteeing phase stays within `[0, 2π)`.

**Logic error (ADSR not updating)** — `synth.getVoice(1)` was hardcoded as `1` instead of loop variable `i`, meaning only the first voice ever updated.

**MIDI not producing audio** — `keyboardState.processNextMidiBuffer()` was missing from `processBlock`, so the MIDI buffer was never forwarded to the synth.

---

## Testing

| Method | Result |
|---|---|
| DAW (FL Studio 2024) | No crashes under rapid parameter changes and complex MIDI input |
| Waveform Analysis (iZotope RX) | Output frequency within ±0.5Hz of expected (C6 = 1046.5Hz measured vs 1047Hz expected) |
| Pluginval | **ALL TESTS PASSED** |

---

## Building

1. Download and install [JUCE](https://juce.com/get-juce/)
2. Open `AMT6001.jucer` in Projucer
3. Set your IDE exporter (tested with Visual Studio 2022)
4. Build in **Release** mode
5. Copy the generated `.vst3` file to your DAW's plugin directory (e.g. `C:\Program Files\Common Files\VST3\`)

---

## Future Development

- Additional oscillator shapes (square, triangle, wavetable)
- LFO modulation
- Anti-aliasing / oversampling
- Preset system
- More saturation/distortion stages
- Logarithmic MIDI velocity response
- Resizable / HiDPI-aware GUI

---

## References
Key resources: [JUCE Documentation](https://docs.juce.com) · [Pluginval](https://www.tracktion.com/develop/pluginval) · [JUCE Forums](https://forum.juce.com)

---

*University of Plymouth · AMT6001 · 2025*
