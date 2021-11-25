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

	int success = terminal.setUpTerminal(); // sets up terminal parameters
	if (success != 0) return 1; // failed to set up terminal parameters

	char input[4];
	ssize_t data;
	do {
		data = read(0, input, sizeof input); // pressing space bar starts the program
	} while (data <= 0 || input[0] != ' ');

	terminal.setupSimulation(); // execute simulation

	return 0;
}
