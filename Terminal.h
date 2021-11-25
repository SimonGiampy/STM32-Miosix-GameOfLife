//
// Created by simon on 07/11/21.
//

#ifndef STM32_MIOSIX_GAMEOFLIFE_EDITOR_H
#define STM32_MIOSIX_GAMEOFLIFE_EDITOR_H

#include <unistd.h>
#include <termios.h>
#include <string>
#include <iostream>

#include "GameOfLife.h"

class Terminal {

private:
	struct termios config{}, oldConfig{}; //terminal configurations
	const int fd = STDIN_FILENO; // terminal file descriptor
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
	bool startingConfiguration();
	void resetTerminal();

	char userInput();
};


#endif //STM32_MIOSIX_GAMEOFLIFE_EDITOR_H
