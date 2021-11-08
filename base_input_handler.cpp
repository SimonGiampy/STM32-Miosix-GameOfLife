//
// Created by simon on 07/11/21.
//


//#define  _POSIX_C_SOURCE  200809L //needed ?
//#define  _GNU_SOURCE
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/input.h>
#include <termios.h>
#include <poll.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

/* Maximum number of input sources, including the terminal. */
#ifndef   MAX_INPUTS
#define   MAX_INPUTS  32
#endif

/* Maximum wait for events, in milliseconds (1000 ms = 1 second). */
#ifndef   INTERVAL_MS
#define   INTERVAL_MS  100
#endif

// the command needed to find the mouse event device is:
// cat /proc/bus/input/devices

// run the program with this command example
// ./mouse.out /dev/input/event7

// code copied from the following link:
// https://stackoverflow.com/questions/52233626/mouse-event-handling-in-linux

int main(int argc, char *argv[]) {
	unsigned char       keys[16];
	struct input_event  event;
	struct termios      config, oldconfig; //terminal configurations
	struct pollfd       src[MAX_INPUTS]; // multiple peripherals input handling
	size_t              done;
	ssize_t             srcs, i, n;
	int                 arg, nsrcs;

	if (!isatty(STDIN_FILENO)) { //checks whether the file descriptor refers to a terminal
		fprintf(stderr, "Standard input is not a terminal.\n");
		return EXIT_FAILURE;
	}

	/* Save old terminal configuration. */
	if (tcgetattr(STDIN_FILENO, &oldconfig) == -1 ||
		tcgetattr(STDIN_FILENO, &config) == -1) {
		fprintf(stderr, "Cannot get terminal settings: %s.\n", strerror(errno));
		return EXIT_FAILURE;
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
		const int  saved_errno = errno;
		tcsetattr(STDIN_FILENO, TCSANOW, &oldconfig);
		fprintf(stderr, "Cannot set terminal settings: %s.\n", strerror(saved_errno));
		return EXIT_FAILURE;
	}

	// The very first input source is the terminal
	src[0].fd = STDIN_FILENO;
	src[0].events = POLLIN;
	src[0].revents = 0;
	srcs = 1;

	/* Add input devices from command line. */
	for (arg = 1; arg < argc; arg++) {
		int  fd;

		//reads data from the peripherals with a non - blocking mechanism
		// reference can be found at: https://linux.die.net/man/3/open
		fd = open(argv[arg], O_RDONLY | O_NOCTTY | O_NONBLOCK);
		if (fd == -1) { //cannot read input device
			fprintf(stderr, "Skipping input device %s: %s.\n", argv[arg], strerror(errno));
			continue;
		}

		if (srcs >= MAX_INPUTS) {
			fprintf(stderr, "Too many event sources.\n");
			return EXIT_FAILURE;
		}

		/* Optional: Grab input device, so only we receive its events. */
		// uncomment this in order to prevent the mouse to be seen when interacting with the code
		//ioctl(fd, EVIOCGRAB, 1);

		src[srcs].fd = fd;
		src[srcs].events = POLLIN;
		src[srcs].revents = 0;
		srcs++;
	}

	printf("Ready. Press Q to exit.\n");
	fflush(stdout);

	done = 0;
	while (!done) { // until the user doesn't terminate the program

		//it waits for one of a set of file descriptors to become ready to perform I/O.
		nsrcs = poll(src, srcs, INTERVAL_MS);

		if (nsrcs == -1) {
			if (errno == EINTR)
				continue;
			fprintf(stderr, "poll(): %s.\n", strerror(errno));
			break;
		}

		/* Terminal is not an input source. */
		if (src[0].revents & POLLIN) {
			n = read(src[0].fd, keys, sizeof keys);
			if (n > 0) {
				for (i = 0; i < n; i++) {
					if (keys[i] == 'q' || keys[i] == 'Q')
						done = 1; //closes the program
					if (keys[i] >= 32 && keys[i] <= 126)
						printf("Key '%c' = 0x%02x = %u pressed\n", keys[i], keys[i], keys[i]);
					else
					if (keys[i])
						printf("Key '\\%03o' = 0x%02x = %u pressed\n", keys[i], keys[i], keys[i]);
					else
						printf("NUL key (0) pressed\n");
				}
				fflush(stdout); //flushes the output buffer on the console
			}
			src[0].revents = 0;
		}

		/* Check the other input sources. */
		for (i = 1; i < srcs; i++) {
			if (src[i].revents & POLLIN) {
				while (1) {
					n = read(src[i].fd, &event, sizeof event);
					if (n != sizeof event)
						break;

					if (event.type == EV_KEY && event.code == BTN_LEFT) {
						if (event.value > 0)
							printf("Left mouse button pressed\n");
						else
							printf("Left mouse button released\n");
					}

					if (event.type == EV_KEY && event.code == BTN_RIGHT) {
						if (event.value > 0)
							printf("Right mouse button pressed\n");
						else
							printf("Right mouse button released\n");
					}
				}
				fflush(stdout);
			}
			src[i].revents = 0;
		}
	}

	/* Close input devices. */
	for (i = 1; i < srcs; i++)
		close(src[i].fd);

	/* Restore terminal settings after terminating the execution */
	tcsetattr(src[0].fd, TCSAFLUSH, &oldconfig);

	printf("All done.\n");
	return 0; // successful execution
}
