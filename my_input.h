//
// Created by simon on 07/11/21.
//

#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
//#include <linux/input.h>
#include <termios.h>
//#include <poll.h>
#include <sys/ioctl.h>
#include <string>
#include <fstream>
#include <iostream>
//#include "miosix/miosix.h" // for micro controller debugging only

#ifndef STM32_MIOSIX_GAMEOFLIFE_EDITOR_H
#define STM32_MIOSIX_GAMEOFLIFE_EDITOR_H


class my_input {

public:
	unsigned char       keys[16];
	//struct input_event  event; //deleting input.h fields
	struct termios      config, oldconfig; //terminal configurations
	//struct pollfd       src[32]; // multiple peripherals input handling (max 32)
	int src;
	ssize_t             srcs, i, n;
	int                 arg, nsrcs;
	int currentX = 0, currentY = 0;

	int handle();
	void basicPrint();
	int freeMover();
	void clearScreen();
	void moveCursor(int x, int y);
	void getCurrentPosition(int *x, int *y);
	static void printTerminalSize();
};


#endif //STM32_MIOSIX_GAMEOFLIFE_EDITOR_H
