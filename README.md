# MarshmallowBot

Files, code, and 3D-print models for a small Wi-Fi-controlled robot that can optionally launch a mini marshmallow using a mousetrap mechanism.

## Features

- ESP32-C3 Super Mini controller
- Browser-based driving interface
- Direct Wi-Fi connection with no router required
- Two-wheel differential drive
- Optional servo-triggered mousetrap launcher
- 3D-printable chassis and launcher parts

## Compatible Hardware

The following components are compatible with the pictured build. Equivalent parts may also work, but verify dimensions, voltage, connector type, and servo rotation before ordering.

- [ESP32-C3 Super Mini with expansion board](https://a.co/d/09ZOaLiy)
- [Continuous-rotation drive servos with wheels](https://a.co/d/07Rn7g2P)
- [4-AA battery holder](https://a.co/d/02fSxSmw)
- [Servo wire connector](https://a.co/d/02fSxSmw)
- [Standard micro servo for optional launcher](https://a.co/d/05VWJJtf)

> **Check this link:** The 4-AA battery holder and servo wire connector currently use the same Amazon link. Confirm that the servo wire connector link is correct before publishing.

## Parts to 3D Print

- 1 chassis
- 1 tail wheel or skid
- 1 mousetrap launcher holder, optional

## Quick Start

1. Build the chassis and connect the servos.
2. Install the Espressif ESP32 board package in Arduino IDE.
3. Open and upload `MarshmallowBot.ino`.
4. Power the robot with the 4-AA holder after verifying polarity.
5. Join the Wi-Fi network **MarshMallowBot** using password `12345678`.
6. Open `192.168.4.1` in a web browser.

## Safety

The mousetrap can pinch fingers and propel objects. Keep hands clear while the trap is cocked. Use only soft mini marshmallows. Never aim at people, animals, faces, windows, or fragile objects. Adult supervision is recommended.

## Full Build Instructions

See the complete assembly, wiring, programming, launcher, and troubleshooting guide included with this project.

## Project Repository

[MarshmallowBot on GitHub](https://github.com/bryceehill/MarshmallowBot)

## Contributing

Improvements to the chassis, launcher, documentation, and code are welcome. Document hardware changes and include clear photographs when possible.


## Compatible Hardware

The following components are compatible with the pictured build. Equivalent
parts may also work, but verify dimensions, voltage, connector type, and servo
rotation before ordering.

- [ESP32-C3 Super Mini with expansion board](https://a.co/d/09ZOaLiy)
- [Continuous-rotation drive servos with wheels](https://a.co/d/07Rn7g2P)
- [4-AA battery holder](https://a.co/d/02fSxSmw)
- [Servo wire connector](https://a.co/d/02fSxSmw)
- [Standard micro servo for optional launcher](https://a.co/d/05VWJJtf)
