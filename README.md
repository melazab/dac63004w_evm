# DAC63004W EVM Driver

Control software for the Texas Instruments DAC63004W-WCSP EVM board.
![Description](docs/images/dac63004wcsp-evm-angled.png_large)

## Overview

This project provides a command-line interface for controlling the DAC63004W evaluation module.

## Features

- Command-line interface
- FTDI interface support
- Control software for DAC63004W evaluation module
- Voltage output on DAC channels 0-3 (Working on adding current output)
- DC and sine waveforms

## Prerequisites

- Linux operating system
- GCC compiler
- Make build system
- libft4222 library from FTDI

## Building

```bash
make
```

## Usage

Basic usage instructions will be added soon. For now, you can:

1. Build the project with `make`
2. Run one of the following valid executables:
   `./bin/dac63004 dc voltage <channel> <voltage>` or
   `./bin/dac63004 sine voltage <channel> <amplitude <frequency>`
3. Get help with: `./bin/dac63004 --help`

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see below for details:

```
MIT License

Copyright (c) 2024 Mohamed Elazab

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## Author

Mohamed Elazab

## Acknowledgments

- Texas Instruments for the DAC63004W
- FTDI for their interface library
