//
// Created by simon on 07/11/21.
//

#include "Terminal.h"

/**
 * Main thread for the program execution.
 * First creates the matrices for the game based on the terminal size.
 * Then lets the user configure the cellular automaton.
 * After the user confirms the configuration, the simulation starts.
 * At the end the terminal configuration is reset
 */
void Terminal::setupSimulation() {

	getTerminalSize();

	int rows = height / 2 - 1, cols = width / 3 - 1;
	game = new GameOfLife(rows, cols);

	game->showState();
	moveCursor(1, 1); // moving to the cell 0,0 of the matrix
	std::cout.flush();

	bool quit = startingConfiguration(); // asks the user to place the living cells

	if (!quit) {
		game->compute(); //start simulation
	}

	// reset terminal config before quitting the program
	resetTerminal();
	delete game;
	std::cout << "Simulation ended.\n\r";
}

/**
 * lets the user decide which cells to activate, and move between the cells
 * @return true if the user quits the program before finishing the set up
 */
bool Terminal::startingConfiguration() {
	bool done = false;
	char input;
	while (!done) {
		input = userInput();
		switch (input) {
			case 'c': { // space bar confirms the cell life positioning
				game->changeCellState(cursorCol - 1, cursorRow - 1);
				setCursorPosition(cursorCol, cursorRow);
			} break;
			case 'e': { // enter command to start the simulation
				done = true;
			} break;
			case 'g': case 'G': { // spawn a glider
				if (cursorRow <= height - 6 && cursorCol <= width - 11)
					game->spawnGlider(cursorCol - 1, cursorRow - 1);
				moveCursor(0, 0);
			} break;
			case 's': case 'S': { // spawns a mid - size spaceship
				if (cursorRow <= height - 9 && cursorCol <= width - 16)
					game->spawnSpaceship(cursorCol - 1, cursorRow - 1);
				moveCursor(0, 0);
			} break;
			case 'q': { // quits the game
				resetTerminal();
				return true;
			}
			default: {}
		}
	}
	return false;
}

/**
 * changes the terminal configuration, setting it to non-canonical (raw) mode
 * @return 1 if the terminal parameters weren't changed due to some error, 0 otherwise
 */
int Terminal::setUpTerminal() {

	if (!isatty(STDIN_FILENO)) { //checks whether the file descriptor refers to a terminal
		std::cerr << "Standard input is not a terminal.\n\r";
		return 1; // failure
	}

	/* Save old terminal configuration. */
	if (tcgetattr(STDIN_FILENO, &oldConfig) == -1 || tcgetattr(STDIN_FILENO, &config) == -1) {
		std::cerr << "Cannot get terminal settings: %s.\n\r";
		return 1; // failure
	}

	// non-canonical mode activated with ~ICANON
	// ~ISIG implies reading some special terminating key combinations to be read as normal input
	// ~ECHO does not echo out the input characters
	config.c_lflag &= ~(ICANON | ISIG | ECHO);

	//disabling ICRNL bit on iflag doesn't restore the carriage return chars
	//config.c_iflag |= (ICRNL | INLCR);
	//config.c_oflag |= ONLCR;

	config.c_cc[VMIN] = 0; //minimum number of characters for canonical read
	config.c_cc[VTIME] = 1; //timeout for non-canonical read = 100ms

	// if the custom settings for the terminal cannot be set, it resets the default configuration saver previously
	if (tcsetattr(STDIN_FILENO, TCSANOW, &config) == -1) {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldConfig);
		std::cerr << "Cannot set terminal settings: %s.\n\r";
		return 1; // failure
	}

	std::cout.flush();

	return 0;
}

/**
 * reads and analyzes user input characters
 * @return the input char for setting up the simulation (excluding the directional arrows)
 */
char Terminal::userInput() {
	bool done = false;
	char typed = 0;
	unsigned char keys[16]{};
	while (!done) { // until the user doesn't send a valid input key

		ssize_t data = read(fd, keys, sizeof keys);
		if (data > 0) {
			if ((keys[0] == 27) && (keys[1] == 91)) { // special characters indicating a directional arrow
				done = true;
				switch (keys[2]) {
					case 65: { // up
						if (cursorRow > 2) moveCursor(0, -2);
					} break;
					case 66: { // down
						if (cursorRow < height - 3) moveCursor(0, 2);
					} break;
					case 67: { // right
						if (cursorCol < width - 6) moveCursor(3, 0);
					} break;
					case 68: { // left
						if (cursorCol > 3) moveCursor(-3, 0);
					} break;
				}
				typed = 'a'; // arrow
			} else {
				for (int i = 0; i < data; i++) {
					if (keys[i] == 'q' || keys[i] == 'Q') {
						done = true; //closes the program
						typed = 'q';
					} else if (keys[i] == ' ') { // spacebar button confirms the positioning
						typed = 'c';
						done = true;
					} else if (keys[i] == 10 || keys[i] == 13) { // enter button starts the simulation
						//char 13 = carriage return on miosix; char 10 = new line on linux
						typed = 'e';
						done = true;
					} else if (keys[i] == 'g' || keys[i] == 'G' || keys[i] == 's' || keys[i] == 'S') {
						typed = (char) keys[i];
						done = true;
					}
				}
			}
		}
	}
	return typed;
}

/**
 * clears terminal content
 */
void Terminal::clearScreen() {
	std::cout << "\033[H\033[J";
}

/**
 * resets the previous configuration of the termios data structure
 */
void Terminal::resetTerminal() {
	// Restore terminal settings after terminating the execution
	tcsetattr(fd, TCSAFLUSH, &oldConfig);
	std::cout << "All done\n\r";
}

/**
 * given the relative coordinates, moves the terminal cursor accordingly
 * @param x difference in the x direction of the terminal
 * @param y difference in the y direction of the terminal
 */
void Terminal::moveCursor(int x, int y) {
	cursorCol += x;
	cursorRow += y;
	std::cout << "\033[" << cursorRow << ";" << cursorCol << "f";
	std::cout.flush(); // flushes stdout so that the cursor position is updated
}

/**
 * given the absolute coordinates, sets the terminal cursor in the exact position
 * @param x absolute x coordinate
 * @param y absolute y coordinate
 */
void Terminal::setCursorPosition(int x, int y) {
	std::cout << "\033[" << y << ";" << x << "f";
	std::cout.flush();
}

/**
 * retrieves the size of the terminal (width and height) via writing to the terminal buffer and reading its response
 */
void Terminal::getTerminalSize() {
	char buf[30] = {0};
	int j, pow;
	char ch = 0;

	int x = 0, y = 0;

	std::cout << "\x1b[999;999H"; // moves the cursor to the extreme right and lower position on the terminal

	std::cout << "\x1b[6n"; // command sent to the terminal to retrieve the cursor position
	std::cout.flush();

	for(j = 0; ch != 'R'; ) {
		ssize_t c = read(fd, &ch, 1);
		if (c > 0) {
			buf[j] = ch;
			j++;
		}
	}

	for(j -= 2, pow = 1; buf[j] != ';'; j--, pow *= 10) {
		x = x + (buf[j] - '0' ) * pow;
	}
	for(j-- , pow = 1; buf[j] != '['; j--, pow *= 10) {
		y = y + (buf[j] - '0') * pow;
	}

	width = x;
	height = y;
	clearScreen();
	std::cout.flush();
}
