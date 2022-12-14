# SST39SF040-Programmer
Arduino Mega 2560 SST39SF040 Flash Chip programmer

This was taken from an example which only wrote to one address. I've added more code to process commands and also allow writing manually in a terminal window.

| SST39SF040 Pins       |        Arduino Mega 2560 Pins |
|-----------------------|-------------------------------|
| A0 to A18             |       22 to 40.               |
| D0 to D7              |       44 to 51.               |
| #WE                   |       2.                      |
| #OE                   |       3.                      |
| #CS                   |       GND.                    |
| Vss                   |       GND.                    |
| Vdd                   |       +5v.                    |

The commands are listed on startup in a terminal window and are formatted as follows:

Write a byte: <W,addr,byte>
This writes a single "decimal" byte (not HEX!) to a decimal address. All writes are checked by reading back the data and messages sent back to the terminal

Read 256 byte block: <R,addr,0>
Set addr to the first 256 byte boundary that you want to read 256 bytes from. The second parameter is ignored and hence 0.

Erase entire chip: <E,0,0>
Both parameters are ignored and the whole chip is erased. There is no function to erase a sector which is the minimum you can erase in one go, plus I don't need to erase anything but the whole chip.

Get Chip ID: <I,0,0>

Dump chip contents: <D,a,b>
Where a is the start address in decimal and b is the end address in decimal

The streaming write commands will be added to this readme later and consist of 3 commands which I need to fully test before saying they're golden.

Please Note: The baud rate is set to 115200 but can be changed in both the Windows application and the Arduino Mega code.

This is a current work in progress and will change a fair bit over the next few weeks. C# application to flash a whole chip has now been added and that is also a WIP.

Enjoy!
Dan R
