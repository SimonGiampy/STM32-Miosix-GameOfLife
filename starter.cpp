//
// Created by simon on 08/11/21.
//

#include "Terminal.h"
#include "Prompt.h"

int main() {

	Terminal::clearScreen();
	Prompt::printLogo();
	Prompt::instructions();

	Terminal terminal;

	int success = terminal.setUpTerminal();
	if (success != 0) return 1; // failed to set up terminal parameters

	char input[4];
	ssize_t data;
	do {
		data = read(0, input, sizeof input);
	} while (data <= 0 || input[0] != ' '); // pressing space bar starts the program

	terminal.setupSimulation(); // execute simulation

	return 0;
}
