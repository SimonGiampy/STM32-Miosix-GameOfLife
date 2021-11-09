//
// Created by simon on 07/11/21.
//

#include "my_input.h"

using namespace std;

int my_input::handle() {

	if (!isatty(STDIN_FILENO)) { //checks whether the file descriptor refers to a terminal
		cerr << "Standard input is not a terminal.\n";
		return 1; // failure
	}

	/* Save old terminal configuration. */
	if (tcgetattr(STDIN_FILENO, &oldconfig) == -1 || tcgetattr(STDIN_FILENO, &config) == -1) {
		cerr << "Cannot get terminal settings: %s.\n";
		return 1; // failure
	}


	cout << "sizeOf tc_flag_t = unsigned int = " << sizeof(config.c_lflag) << endl;
	cout << "sizeOf cc_t = unsigned char array x32 = " << sizeof(config.c_cc) << endl;

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

	cout << "iflag = " << config.c_iflag << endl;
	cout << "oflag = " << config.c_oflag << endl;
	cout << "lflag = " << config.c_lflag << endl;
	cout << "cflag = " << config.c_cflag << endl;


	config.c_cc[VEOL] = '\r';
	// if the custom settings for the terminal cannot be set, it resets the default configuration saver previously
	if (tcsetattr(STDIN_FILENO, TCSANOW, &config) == -1) {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldconfig);
		cerr << "Cannot set terminal settings: %s.\n";
		return 1; // failure
	}

	// The very first input source is the terminal
	//src.fd = STDIN_FILENO;
	//src.events = POLLIN;
	//src.revents = 0;
	src = STDIN_FILENO;
	srcs = 1;

	/* Add input devices from command line. */
	/*
	for (arg = 1; arg < argc; arg++) {
		//reads data from the peripherals with a non - blocking mechanism
		// reference can be found at: https://linux.die.net/man/3/open

		int fd = open(argv[arg], O_RDONLY | O_NOCTTY | O_NONBLOCK);
		if (fd == -1) { //cannot read input device
			cerr << "Skipping input device " << argv[arg] << endl;
			continue;
		}

		fstream fs;
		try {
			fs.open(argv[arg], fstream::in);
		}
		catch (const ios_base::failure& e) {
			cout << "Exception opening/reading file peripheral";
		}


		src[srcs].fd = fd;
		src[srcs].events = POLLIN;
		src[srcs].revents = 0;
		srcs++;
	}
	*/

	cout << "Ready. Press Q to exit.\n";
	cout.flush();

	bool done = false;
	while (!done) { // until the user doesn't terminate the program

		//it waits for one of a set of file descriptors to become ready to perform I/O.
		//struct pollfd* source = &src;
		//nsrcs = poll(source, srcs, 100);

		if (nsrcs == -1) {
			cerr << "invalid poll";
			break;
		}

		/* Terminal is not an input source. */
		//if (src.revents & POLLIN) {
			//n = read(src.fd, keys, sizeof keys);
		n = read(src, keys, sizeof keys);
			/*
			 * file read without file descriptor and buffer size allocation
			 */
			if (n > 0) {
				for (i = 0; i < n; i++) {
					if (keys[i] == 'q' || keys[i] == 'Q') {
						done = true; //closes the program
					} else {
						cout << "Key " << keys[i] << " pressed\n";
					}
				}
				cout.flush(); //flushes the output buffer on the console
			}
			//src.revents = 0;
		//}


	}

	/* Restore terminal settings after terminating the execution */
	tcsetattr(src, TCSAFLUSH, &oldconfig);

	cout << "All done" << endl;
	return 0; // successful execution
}

void my_input::basicPrint() {
	cout << "Helloworld\n";
	string gane;
	cin >> gane;
	cout << "name ? " << gane;
}

int my_input::freeMover() {

	clearScreen();

	if (!isatty(STDIN_FILENO)) { //checks whether the file descriptor refers to a terminal
		cerr << "Standard input is not a terminal.\n";
		return 1; // failure
	}

	/* Save old terminal configuration. */
	if (tcgetattr(STDIN_FILENO, &oldconfig) == -1 || tcgetattr(STDIN_FILENO, &config) == -1) {
		cerr << "Cannot get terminal settings: %s.\n";
		return 1; // failure
	}

	config.c_lflag &= ~(ICANON | ISIG | ECHO);
	config.c_cc[VMIN] = 0; //minimum number of characters for canonical read
	config.c_cc[VTIME] = 0; //timeout for non canonical read

	// if the custom settings for the terminal cannot be set, it resets the default configuration saver previously
	if (tcsetattr(STDIN_FILENO, TCSANOW, &config) == -1) {
		tcsetattr(STDIN_FILENO, TCSANOW, &oldconfig);
		cerr << "Cannot set terminal settings: %s.\n";
		return 1; // failure
	}

	src = STDIN_FILENO;

	cout << "Ready. Press Q to exit.\n";
	cout.flush();

	bool done = false;
	while (!done) { // until the user doesn't terminate the program

		ssize_t data = read(src, keys, sizeof keys);
		if (data > 0) {
			if ((keys[0] == 27) && (keys[1] == 91)) { // special characters indicating a directional arrow
				switch (keys[2]) {
					case 65: { // up
						cout << "██";
						moveCursor(0, -2);
					} break;
					case 66: { // down
						cout << "██";
						moveCursor(0, 2);
					} break;
					case 67: { // right
						cout << "██";
						moveCursor(2, 0);
					} break;
					case 68: { // left
						cout << "██";
						moveCursor(-2, 0);
					} break;
				}
			} else {
				for (i = 0; i < data; i++) {
					if (keys[i] == 'q' || keys[i] == 'Q') {
						done = true; //closes the program
					} else {
						cout << "Key " << keys[i] << " pressed\n";
					}
				}
			}

			cout.flush(); //flushes the output buffer on the console
		}

	}

	/* Restore terminal settings after terminating the execution */
	tcsetattr(src, TCSAFLUSH, &oldconfig);
	cout << "All done" << endl;
	return 0;
}

void my_input::clearScreen() {
	cout << "\033[H\033[J"; //clear terminal
}

/**
 * given the relative coordinates, moves the terminal cursor accordingly
 * @param row difference in the y direction of the terminal
 * @param col differenct in the x direction of the terminal
 */
void my_input::moveCursor(int row, int col) {
	getCurrentPosition(&currentX, &currentY);
	col += currentY;
	row += currentX;
	cout << "\033[" << col << ";" << row << "H";
}

/**
 * retrieves the current position of the cursor on the terminal
 * @param x column position
 * @param y row position
 */
void my_input::getCurrentPosition(int *x, int *y) {
	char buf[30]={0};
	int j, pow;
	char ch = 0;

	*y = 0; *x = 0;

	//printTerminalSize(); // prints the dimension in single characters of the terminal window

	write(1, "\033[6n", 4); // command sent to the terminal to retrieve the cursor position

	for(j = 0; ch != 'R'; ) {
		ssize_t c = read(src, &ch, 1);
		if (c > 0) {
			buf[j] = ch;
			j++;
		}
	}

	for(j -= 2, pow = 1; buf[j] != ';'; j--, pow *= 10) {
		*x = *x + (buf[j] - '0' ) * pow;
	}
	//cout << "x = " << *x; // x coordinate

	for(j-- , pow = 1; buf[j] != '['; j--, pow *= 10) {
		*y = *y + (buf[j] - '0') * pow;
	}
	//cout << "y = " << *y; // y coordinate
}

/**
 * prints the size of the terminal
 * //TODO: save the initial terminal size in two members of the class so the cursor cannot bypass the boundaries
 */
void my_input::printTerminalSize() {
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	printf ("lines %d\n", w.ws_row);
	printf ("columns %d\n", w.ws_col);
}

