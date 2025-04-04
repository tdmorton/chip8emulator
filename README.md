# Images
Space Invaders:
![Space Invaders](https://github.com/tdmorton/chip8emulator/blob/main/images/space_invaders.png "Space Invaders")

Tetris:
![Tetris](https://github.com/tdmorton/chip8emulator/blob/main/images/tetris.png "Tetris")

# Building

Project requires g++, make, and SDL2:

```
$ sudo apt-get install make libsdl2-dev libsdl2-image-dev
```

To build, run

```
$ make
```

This will create a chip8 file

# Running

To run the program, use:

```
$ ./chip8 [-d] [-h] rom.ch8
```

The rom filename will need to end in ".ch8" to be parsed correctly.

The escape key can be pressed at any time to exit.

# Cleaning

To clean up the directory, run

```
make clean
```

This will remove the chip8 executable

