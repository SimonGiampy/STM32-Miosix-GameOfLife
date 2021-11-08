//
// Created by simon on 07/11/21.
//

#include "my_input.h"
//TODO: retrieve terminal size upon starting the execution

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

	/* Set new terminal configuration. */
	config.c_iflag &= ~(IGNBRK | BRKINT | PARMRK); // input flags, not sure if they are really needed

	// non-canonical mode activated with ~ICANON
	// ~ISIG implies reading some special terminating key combinations to be read as normal input
	// ~ECHO does not echo out the input characters
	// TOSTOP and IEXTEN probably not needed
	config.c_lflag &= ~(ICANON | ISIG | ECHO | IEXTEN | TOSTOP);

	config.c_cc[VMIN] = 0; //minimum number of characters for canonical read
	config.c_cc[VTIME] = 0; //timeout for non canonical read
	config.c_cc[VSTART] = 0;
	config.c_cc[VSTOP] = 0;

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
					if (keys[i] == 'q' || keys[i] == 'Q')
						done = true; //closes the program
					if (keys[i] >= 32 && keys[i] <= 126)
						cout << "Key " << keys[i] << " pressed\n";
					else
						cout << "NUL key (0) pressed\n";
				}
				cout.flush(); //flushes the output buffer on the console
			}
			//src.revents = 0;
		//}

		/* Check the other input sources. */
		/*
		for (i = 1; i < srcs; i++) {
			if (src[i].revents & POLLIN) {
				while (true) {
					n = read(src[i].fd, &event, sizeof event);
					if (n != sizeof event)
						break;

					if (event.type == EV_KEY && event.code == BTN_LEFT) {
						if (event.value > 0)
							cout << "Left mouse button pressed\n";
						else
							cout << "Left mouse button released\n";
					}

					if (event.type == EV_KEY && event.code == BTN_RIGHT) {
						if (event.value > 0)
							cout << "Right mouse button pressed\n";
						else
							cout << "Right mouse button released\n";
					}
				}
				cout.flush();
			}
			src[i].revents = 0;
		}
		 */
	}

	/* Close input devices. */
	/*
	for (i = 1; i < srcs; i++)
		close(src[i].fd); // no need to close the standard input file
	*/


	/**
	 * close file streams opened
	 */

	/* Restore terminal settings after terminating the execution */
	tcsetattr(src, TCSAFLUSH, &oldconfig);

	cout << "All done" << endl;
	return 0; // successful execution
}

