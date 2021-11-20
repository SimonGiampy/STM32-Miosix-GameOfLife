//
// Created by simon on 07/11/21.
//

#ifndef STM32_MIOSIX_GAMEOFLIFE_EDITOR_H
#define STM32_MIOSIX_GAMEOFLIFE_EDITOR_H

#include <unistd.h>
#include <termios.h>
#include <string>
#include <iostream>
//#include "miosix/miosix.h" // needed dependencies for the Miosix OS

#include "GameOfLife.h"

class Terminal {

private:
	unsigned char keys[16]{};
	struct termios config{}, oldConfig{}; //terminal configurations
	int fd = 0; // terminal file descriptor
	int cursorCol = 1, cursorRow = 1; // cursor coordinates
	int width = 0, height = 0; // terminal size
	GameOfLife *game{};

public:

	static void clearScreen();
	void moveCursor(int x, int y);
	static void setCursorPosition(int x, int y);
	void getTerminalSize();

	int setUpTerminal();
	void setupSimulation();
	void startingConfiguration();
	void resetTerminal();

	char userInput();
};


#endif //STM32_MIOSIX_GAMEOFLIFE_EDITOR_H
