## Synopsis

Arduino library for PCD8544 LCD controller.

Includes:
* library by it self;
* fence python tool to convert *.png pictures in to C arrays;
* examples;
* documentation.

## Code Example

For code examples refer to examples/*

## Motivation

N/A

## Installation

Requirements:
* Arduino IDE
* git

Clone repository inside sketchbook/libraries/.

If you have default configuration on linux machine then use following commands:

cd ~/sketchbook/libraries/

git clone https://github.com/kornal/PCD8544.git PCD8544

## API Reference

For API refer to PCD8544.h file.

If you want to prepare your own pictures refer to:
examples/PCD8544_Animation/graph/prepareHeaderFiles.*

## Tests

1. Build examples using Arduino IDE.
2. Prepare hardware environment - refer to doc/PCD8544_ATMega_Arduino_PinOut.pdf
3. Upload examples using Arduino IDE

For more advanced developers:
Instead of Arduino IDE use make files attached to each example.
Be aware that make files require at leas arduino-mk package installed.

Finally: this package has been tested only on linux environment:
Ubuntu 14.04 trusty 3.13.0-92-generic;
Arduino 1:1.0.5+dfsg2-2 (from Ubuntu repository).

## Contributors

Guthub communication channels.

## License

For license details refer to each file.

Summary:

MIT
utilities/png.py

GPLv2
rest of content
