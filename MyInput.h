//
// Created by simon on 07/11/21.
//

#ifndef STM32_MIOSIX_GAMEOFLIFE_EDITOR_H
#define STM32_MIOSIX_GAMEOFLIFE_EDITOR_H

#include <unistd.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <string>
#include <fstream>
#include <iostream>
//#include "miosix/miosix.h" // for micro controller debugging only

#include "GameOfLife.h"

class MyInput {

private:
	unsigned char keys[16]{};
	struct termios config{}, oldConfig{}; //terminal configurations
	int fd = 0; // terminal file descriptor
	int cursorCol = 1, cursorRow = 1; // cursor coordinates
	int width = 0, height = 0; // terminal size

public:

	static void clearScreen();
	void moveCursor(int x, int y);
	static void setCursorPosition(int x, int y);
	void getTerminalSize();

	int setUpTerminal();
	void resetTerminal();
	char userInput();
	void initialConfiguration();
};


#endif //STM32_MIOSIX_GAMEOFLIFE_EDITOR_H
