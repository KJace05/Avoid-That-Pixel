# Avoid That Pixel

A handheld dodge game built on a custom Arduino Uno shield, featuring a 16x2 character LCD, real-time obstacle dodging, and buzzer sound feedback. Designed from schematic and PCB layout to embedded firmware.

## Overview

Avoid That Pixel is a single-player reaction game: a player-controlled character must dodge incoming obstacles displayed on a 16x2 LCD before they collide. The project covers the full hardware-to-firmware pipeline; circuit design, PCB fabrication, and embedded C++ programming.

## Hardware

- **Custom Arduino Uno shield**, designed in KiCad 10.0
  - Schematic capture and footprint selection
  - Copper pour (ground plane) for improved signal return paths and reduced routing complexity
  - ERC/DRC error resolution
  - Fabricated via JLCPCB
- 16x2 character LCD for game display
  - 220 Ohm resistor
- Pushbutton for player input
  - 10k resistor
- (3) Multicolored LEDs for health bar
  - (3) 1k resistor
- 10k Potentiometer for LCD brightness control
- Piezo buzzer (active) for audio feedback
  - 1k resistor
- Arduino Uno (base board)

## Firmware

Written in C++ for the Arduino platform. Key implementation details:

- **Non-blocking timing** using `millis()` instead of `delay()`, allowing obstacle movement, button input, and buzzer playback to run concurrently without freezing gameplay
- **Custom LCD characters** for the player sprite and obstacles, defined via the LCD's CGRAM
- **Collision detection** comparing player and obstacle screen positions each game tick
- **Buzzer audio** for collision/game-over feedback, implemented without blocking the main game loop

## Repository Structure

```
avoid-that-pixel/
├── firmware/
│   └── avoid_that_pixel.ino     # Main Arduino sketch
├── hardware/
│   ├── avoid_that_pixel.kicad_pro
│   ├── avoid_that_pixel.kicad_sch
│   └── avoid_that_pixel.kicad_pcb
├── docs/
│   └── media/                  # PCB renders, gameplay photos
└── README.md
```

## Getting Started

1. Flash `firmware/avoid_that_pixel.ino` to an Arduino Uno using the Arduino IDE.
2. Mount the custom shield (or wire components per the schematic in `hardware/`) onto the Uno.
3. Power on and play — dodge the incoming obstacles on the LCD before they reach the player position.

## Tools Used

- **TinkerCad** — original design and testing
-  **KiCad** — schematic capture and PCB layout
- **Arduino IDE / C++** — firmware development
- **JLCPCB** — PCB fabrication

## Author

Kaiden Esteves (KJace05)
