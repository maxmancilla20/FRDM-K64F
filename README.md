markdown
# FRDM-K64F — Multi-Protocol Embedded Projects

Collection of bare-metal and FreeRTOS-based projects for the NXP FRDM-K64F
development board (Kinetis K64, ARM Cortex-M4), built on the KSDK/MCUXpresso
driver and CMSIS layers.

## Protocols & Features
- **CAN** — controller configuration and message handling
- **LIN** — master/slave communication
- **Ethernet** — TCP/IP stack integration
- **MQTT** (via Mosquitto) — publish/subscribe messaging over Ethernet
- **CoAP** — constrained application protocol for IoT messaging
- **FreeRTOS** — real-time task scheduling across the above modules

## Project Structure

├── board/ # Board-specific init (clocks, pins, peripherals)
├── CMSIS/ # ARM Cortex-M4 core access layer
├── component/ # Reusable middleware components
├── device/ # MCU-specific headers and startup definitions
├── drivers/ # Peripheral drivers (CAN, UART, GPIO, etc.)
├── source/ # Application-level source code per project
├── startup/ # Reset/vector table and boot code
└── utilities/ # Logging, debug print, helper functions


## Toolchain
- MCUXpresso IDE / Eclipse-based build (`.project`, `.cproject`)
- J-Link debug configuration included for SWD flashing/debugging

## Getting Started
1. Import the project into MCUXpresso IDE (File → Import → Existing Projects).
2. Select the target sub-project under `source/` for the protocol you want to run.
3. Build and flash via J-Link (launch config included in the repo root).

## Status
Active learning/reference project — protocols are implemented and tested
individually; integration across all protocols simultaneously is in progress.
