# Miosix embedded OS project running on a STM32 micro-controller 
## Advanced Operating Systems Project at Polimi

John Conway's Game of Life implementation running on the Miosix embedded OS installed on an STM32 Nucleo micro-controller.

This is the **Project repository for the Advanced Operating Systems course of the Master's Degree Computer Science and 
Engineering at Politecnico di Milano.**

## Game of Life
The Game of Life, is a cellular automaton devised by the British mathematician John Conway in 1970. 
It is a zero-player game, meaning that its evolution is determined by its initial state, requiring no further input. 
One interacts with the Game of Life by creating an initial configuration and observing how it evolves. 
It is Turing complete and can simulate a universal constructor or any other Turing machine.
Further information and a description of the cellular automaton is provided in the 
[Wikipedia page](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).
The rules used in this program are the exact replication of the standard rules described in the Wiki.

## Project Report
In the Report folder, there is the official and complete documentation of the project. The _.tex_ file is the LaTeX 
documentation, which can be compiler on a local machine. The pdf file is the already compiled LaTeX document. 
There is a thorough high-level description of the code and the mechanics of the Game of Life program simulation.


### Miosix Installation
Miosix kernel is used as submodule as specified in
[official wiki page](https://miosix.org/wiki/index.php?title=Miosix_and_git_workflow).
In order to build the project, first clone the repository. Then move the contents of this folder to the Miosix repository.

### Repository usage
Follow this [guide](https://miosix.org/wiki/index.php?title=Linux_Quick_Start) to install correctly your board.
Clone the [Miosix repository](https://github.com/fedetft/miosix-kernel).
Copy this repository inside the `miosix-kernel/` path. Substitute the `main.cpp` file with the one in this repo and 
use the command
>make

to compile the executable. Note that the Makefile has been slightly changed from the provided standard one in order 
to include certain directories.

The project is built for this [STM32-Nucleo board](https://www.st.com/en/evaluation-tools/nucleo-f401re.html)
on top of [Miosix Embedded OS](http://miosix.org/).

### Flashing the Kernel and the program
After compiling the Kernel, it can be flashed to the micro-controller via command line with
> st-flash write main.bin 0x08000000

using the [stlink](https://github.com/stlink-org/stlink) utility. Note that if some compilation problem arises, 
it might be resolved by cleaning the compilation output with `make clean`.

### Access the board output
In order to access what the board prints on _screen_, after connecting the board to the computer, you should use the command:
> screen /dev/ttyACM0 19200

You can then exit _screen_ by pressing the keys combination Ctrl+A -> k -> y.

### Increase baud rate
Modify in the file `miosix-kernel/miosix/config/arch/cortexM4_stm32f4/stm32f401re_nucleo/board_settings.h` at the 
following line:

>const unsigned int defaultSerialSpeed=460800;

This has been found to be the highest working baud rate for this stm32 nucleo board.

### Useful Documentation
I have used also this [guide](https://en.wikibooks.org/wiki/Serial_Programming/termios) for the serial programming part.
The terminal configuration is changed by using the library `termios.h`


**Project Developer**: 
_Simone Giampà_ (Me)