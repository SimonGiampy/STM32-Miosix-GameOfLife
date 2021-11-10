//
// Created by simon on 07/11/21.
//

#include "MyInput.h"

//using namespace std;

int MyInput::inputTest() {

	if (!isatty(STDIN_FILENO)) { //checks whether the file descriptor refers to a terminal
		std::cerr << "Standard input is not a terminal.\n";
		return 1; // failure
	}

	/* Save old terminal configuration. */
	if (tcgetattr(STDIN_FILENO, &oldConfig) == -1 || tcgetattr(STDIN_FILENO, &config) == -1) {
		std::cerr << "Cannot get terminal settings: %s.\n";
		return 1; // failure
	}


	std::cout << "sizeOf tc_flag_t = unsigned int = " << sizeof(config.c_lflag) << std::endl;
	std::cout << "sizeOf cc_t = unsigned char array x32 = " << sizeof(config.c_cc) << std::endl;

	/* Set new terminal configuration. */
	// removed PARMRK flag from the c_iflag
	//config.c_iflag &= ~( IGNBRK | BRKINT | IGNCR | IGNPAR); // input flags, not sure if they are really needed
	//config.c_iflag &= ~(IGNBRK | BRKINT | INLCR );

	// non-canonical mode activated with ~ICANON
	// ~ISIG implies reading some special terminating key combinations to be read as normal input
	// ~ECHO does not echo out the input characters
	// TOSTOP and IEXTEN probably not needed
	//removed IEXTEN from the c_lflag list
	config.c_lflag &= ~(ICANON | ISIG | ECHO);
	config.c_iflag &= ~(INLCR | IGNCR | ICRNL);

	config.c_cc[VMIN] = 0; //minimum number of characters for canonical read
	config.c_cc[VTIME] = 0; //timeout for non canonical read
	//config.c_cc[VSTART] = 0;
	//config.c_cc[VSTOP] = 0;

	config.c_cflag &= ~(ONLCR);

	std::cout << "iflag = " << config.c_iflag << std::endl;
	std::cout << "oflag = " << config.c_oflag << std::endl;
	std::cout << "lflag = " << config.c_lflag << std::endl;
	std::cout << "cflag = " << config.c_cflag << std::endl;


	config.c_cc[VEOL] = '\r';
	// if the custom settings for the terminal cannot be set, it resets the default configuration saver previously
	if (tcsetattr(STDIN_FILENO, TCSANOW, &config) == -1) {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldConfig);
		std::cerr << "Cannot set terminal settings: %s.\n";
		return 1; // failure
	}

	// The very first input source is the terminal
	//fd.fd = STDIN_FILENO;
	//fd.events = POLLIN;
	//fd.revents = 0;
	fd = STDIN_FILENO;
	//srcs = 1;

	/* Add input devices from command line. */
	/*
	for (arg = 1; arg < argc; arg++) {
		//reads data from the peripherals with a non - blocking mechanism
		// reference can be found at: https://linux.die.net/man/3/open

		int fd = open(argv[arg], O_RDONLY | O_NOCTTY | O_NONBLOCK);
		if (fd == -1) { //cannot read input device
			std::cerr << "Skipping input device " << argv[arg] << std::endl;
			continue;
		}

		fstream fs;
		try {
			fs.open(argv[arg], fstream::in);
		}
		catch (const ios_base::failure& e) {
			std::cout << "Exception opening/reading file peripheral";
		}


		fd[srcs].fd = fd;
		fd[srcs].events = POLLIN;
		fd[srcs].revents = 0;
		srcs++;
	}
	*/

	std::cout << "Ready. Press Q to exit.\n";
	std::cout.flush();

	bool done = false;
	while (!done) { // until the user doesn't terminate the program

		//it waits for one of a set of file descriptors to become ready to perform I/O.
		//struct pollfd* source = &fd;
		//nsrcs = poll(source, srcs, 100);
		/*
		if (nsrcs == -1) {
			std::cerr << "invalid poll";
			break;
		}
		 */

		/* Terminal is not an input source. */
		//if (fd.revents & POLLIN) {
			//n = read(fd.fd, keys, sizeof keys);
		n = read(fd, keys, sizeof keys);
			/*
			 * file read without file descriptor and buffer size allocation
			 */
			if (n > 0) {
				for (i = 0; i < n; i++) {
					if (keys[i] == 'q' || keys[i] == 'Q') {
						done = true; //closes the program
					} else {
						std::cout << "Key " << keys[i] << " pressed\n";
					}
				}
				std::cout.flush(); //flushes the output buffer on the console
			}
			//fd.revents = 0;
		//}


	}

	/* Restore terminal settings after terminating the execution */
	tcsetattr(fd, TCSAFLUSH, &oldConfig);

	std::cout << "All done" << std::endl;
	return 0; // successful execution
}

void MyInput::initialConfiguration() {
	//TODO: prompt tutorial for the usage of the game

	//TODO: delete the method that queries the cursor position, instead calculate it every time the cursor is moved

	// ask the user to choose the dimensions of the matrix, or calculate them from the terminal size
	int rows = 10, cols = 10;
	GameOfLife game(rows, cols);

	int success = setUpTerminal();
	if (success != 0) return; // failed to set up terminal parameters

	//game.showState();
	moveCursor(-currentX, -currentY);

	bool done = false;
	char input;
	while (!done) {
		input = userInput();
		switch (input) {
			case 'c': {
				std::cout << "changing state x = " << currentX << ", y = " << currentY << "\n\r";
				game.changeCellState(currentX, currentY);
			} break;
			case 'e': {
				game.compute(); //start simulation
			} break;
			case 'u': case 'd': case 'l': case 'r': {} break;
			default: { // quit signal
				done = true;
			}
		}
	}

	//game.compute();

	// reset terminal config before quitting the simulation
	resetTerminal();
}

int MyInput::setUpTerminal() {
	clearScreen();

	if (!isatty(STDIN_FILENO)) { //checks whether the file descriptor refers to a terminal
		std::cerr << "Standard input is not a terminal.\n";
		return 1; // failure
	}

	/* Save old terminal configuration. */
	if (tcgetattr(STDIN_FILENO, &oldConfig) == -1 || tcgetattr(STDIN_FILENO, &config) == -1) {
		std::cerr << "Cannot get terminal settings: %s.\n";
		return 1; // failure
	}

	config.c_lflag &= ~(ICANON | ISIG | ECHO);
	//disabling ICRNL bit on iflag doesn't work
	config.c_iflag |= (ICRNL | INLCR );
	config.c_oflag |= ONLCR;

	config.c_cc[VMIN] = 0; //minimum number of characters for canonical read
	config.c_cc[VTIME] = 0; //timeout for non canonical read

	// if the custom settings for the terminal cannot be set, it resets the default configuration saver previously
	if (tcsetattr(STDIN_FILENO, TCSANOW, &config) == -1) {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldConfig);
		std::cerr << "Cannot set terminal settings: %s.\n";
		return 1; // failure
	}

	fd = STDIN_FILENO;

	//std::cout << "Ready. Press Q to exit.\n\r";
	//std::cout.flush();

	return 0;
}

char MyInput::userInput() {
	bool done = false;
	char typed = 0;

	while (!done) { // until the user doesn't terminate the program

		ssize_t data = read(fd, keys, sizeof keys);
		if (data > 0) {
			if ((keys[0] == 27) && (keys[1] == 91)) { // special characters indicating a directional arrow
				done = true;
				switch (keys[2]) {
					case 65: { // up
						moveCursor(0, -2);
						typed = 'u';
					} break;
					case 66: { // down
						moveCursor(0, 2);
						typed = 'd';
					} break;
					case 67: { // right
						moveCursor(3, 0);
						typed = 'r';
					} break;
					case 68: { // left
						moveCursor(-3, 0);
						typed = 'l';
					} break;
				}
			} else {
				for (i = 0; i < data; i++) {
					if (keys[i] == 'q' || keys[i] == 'Q') {
						done = true; //closes the program
						typed = 'q';
					} else if (keys[i] == ' ') {
						typed = 'c';
						done = true;
					} else if (keys[i] == 13) {
						typed = 'e';
						done = true;
					}
					// add new conditions for more controls of the game simulation
					std::cout << "Key " << keys[i] << " pressed\n\r";
				}
			}

			std::cout.flush(); //flushes the output buffer on the console
		}
	}
	return typed;
}

/**
 * clears terminal
 */
void MyInput::clearScreen() {
	std::cout << "\033[H\033[J";
}

/**
 * resets the previous configuration of the termios data structure
 */
void MyInput::resetTerminal() {
	/* Restore terminal settings after terminating the execution */
	tcsetattr(fd, TCSAFLUSH, &oldConfig);
	std::cout << "All done\n\r" << std::endl;
}

/**
 * given the relative coordinates, moves the terminal cursor accordingly
 * @param row difference in the y direction of the terminal
 * @param col differenct in the x direction of the terminal
 */
void MyInput::moveCursor(int row, int col) {
	updateCurrentPosition();
	col += currentY;
	row += currentX;
	std::cout << "\033[" << col << ";" << row << "H";
}

/**
 * retrieves the current position of the cursor on the terminal
 * @param x column position
 * @param y row position
 */
void MyInput::updateCurrentPosition() {
	char buf[30] = {0};
	int j, pow;
	char ch = 0;

	int *y = new int, *x = new int;

	ssize_t ret = write(1, "\033[6n", 4); // command sent to the terminal to retrieve the cursor position
	if (ret <= 0) {
		std::cerr << "error in retrieving current cursor position\n";
	}

	for(j = 0; ch != 'R'; ) {
		ssize_t c = read(fd, &ch, 1);
		if (c > 0) {
			buf[j] = ch;
			j++;
		}
	}

	for(j -= 2, pow = 1; buf[j] != ';'; j--, pow *= 10) {
		*x = *x + (buf[j] - '0' ) * pow;
	}
	//std::cout << "x = " << *x; // x coordinate

	for(j-- , pow = 1; buf[j] != '['; j--, pow *= 10) {
		*y = *y + (buf[j] - '0') * pow;
	}
	//std::cout << "y = " << *y; // y coordinate

	currentX = *x;
	currentY = *y;
	delete x;
	delete y;
}

/**
 * prints the size of the terminal
 * //TODO: save the initial terminal size in two members of the class so the cursor cannot bypass the boundaries
 */
void MyInput::printTerminalSize() {
	/*
	//this code works on linux with the library sys/ioctl.h (but miosix does not contain the necessary data structure)
	struct winsize w{};
	ioctl(STDOUT_FILENO, 0x5413, &w);
	//ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	std::cout << "terminal size: lines " << w.ws_row << ", columns " << w.ws_col << ".\n\r";
	 */

	//this code works on linux with the library sys/wait.h (included in miosix)
	auto const w(WEXITSTATUS(std::system("exit `tput cols`")));
	auto const h(WEXITSTATUS(std::system("exit `tput lines`")));

	std::cout << "terminal size = ";
	printf ("lines %d ", w);
	printf ("columns %d.\n", h);
}

