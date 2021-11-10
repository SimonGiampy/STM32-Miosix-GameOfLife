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
	ssize_t i = 0, n = 0; //, srcs
	int currentX = 0, currentY = 0; // cursor coordinates

public:

	static void clearScreen();
	void moveCursor(int x, int y);
	void updateCurrentPosition();
	static void printTerminalSize();

	int inputTest();
	int setUpTerminal();
	void resetTerminal();
	char userInput();
	void initialConfiguration();
};


#endif //STM32_MIOSIX_GAMEOFLIFE_EDITOR_H
