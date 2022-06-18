# Beshfuck
Brainfuck interpreter with procedures and threads.

## Features
### Procedures
Procedures declared using ``!`` keyword, and called using ``$``. Example: 
```
!print{.[-]} +++++++++++++++++++++++++++++++++ $print
```
### Constants
Instead of counting pluses you can use constants. It loads number into cell using ``=``. Example:
```
=66.=10.
```
This program prints letter 'B' and newline.
only values from 0 to 255 allowed (cells are unsigned 8 bit integers).
### Threads
Coming soon...

## Building
Install ``cmake`` and then type:
```sh
mkdir build && cd build
cmake ..
make
```
You can enable debug printing by passing ``-DDEBUG_PRINT=ON`` to cmake.

## License
All the code in this repository is released under the WTFPL license. Take a look at the [LICENSE](LICENSE) for more info.