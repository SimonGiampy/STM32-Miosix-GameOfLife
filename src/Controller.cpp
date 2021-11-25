//
// Created by simon on 17/11/21.
//

#include "includes/Controller.h"

/**
 * constructor initializes the delay between frames to 1000ms
 */
Controller::Controller() {
	timeDelayIndex = 5;
}

/**
 * destructor joins the input reader thread
 */
Controller::~Controller() {
	if (reader.joinable()) reader.join(); // joins reader thread when the program finishes its execution
}

/* Synchronized Queue, using FIFO policy */

/**
 * returns element at the tail of the queue. Must be called when the queue is not empty
 * @return char at the back of the queue
 */
char Controller::get() {
	std::unique_lock<std::mutex> lock(mutex);
	while(actionsQueue.empty()) {
		listCondition.wait(lock);
	}
	char result = actionsQueue.back();
	return result;
}

/**
 * adds an element at the head of the list
 * @param action input character to be put in the list
 */
void Controller::put(char action) {
	std::unique_lock<std::mutex> lock(mutex);
	actionsQueue.push_front(action);
	listCondition.notify_one();
}

/**
 * manages the user input while between a frame and the next one.
 * Wakes up every 25ms in order to check if the user sent some commands in input, and processes them.
 * @return true if the user stopped the execution of the simulation
 */
bool Controller::inputManager() {
	// the main thread starts calls this function after displaying a frame
	// and waits for this function to finish its execution before showing the next frame

	bool stopped = false;
	for (int i = 0; (i < timeDelays[timeDelayIndex] || stopped) && !terminate; i += 25) { //updates every 25 ms

		// analyze the queue from the head to the tail to analyze the commands
		while (!actionsQueue.empty()) {
			if (this->get() == 's' || this->get() == 'S') { // stopped
				stopped = true;
				actionsQueue.pop_back(); // resumes
			} else if (this->get() == 'r' || this->get() == 'R') {
				stopped = false;
				actionsQueue.pop_back();
			} else if (this->get() == 'f' || this->get() == 'F') { // change simulation speed
				timeDelayIndex = (timeDelayIndex + 1) % 7;
				actionsQueue.pop_back();
			}

			// quit signal should break out the cycle and terminate even if the timer didn't expire
			if (!actionsQueue.empty() && (this->get() == 'q' || this->get() == 'Q')) {
				terminate = true;
				break;
			}
		}

		//wakes up after 25ms and sees if the user has sent an input in the meantime
		std::this_thread::sleep_for(std::chrono::milliseconds (25));
	}

	// does not clear the queue before the new iteration, so that the new incoming input will be processed

	if (terminate) reader.join(); // joins the reader thread if the user terminated the program
	return terminate;
}

/**
 * creates a new thread and starts it, in order to read the user input
 */
void Controller::startReader() {
	reader = std::thread(&Controller::readerThread, this);
}

/**
 * thread that reads user input commands during simulation, and sends the commands in a list
 */
void Controller::readerThread() {
	char input[4] = "";
	while (input[0] != 'q' && !terminate) {
		// it should not wait for user input if the queue contains the quit signal

		ssize_t data = read(STDIN_FILENO, input, sizeof input);
		if (data > 0) {
			// accepted input commands
			if (input[0] == 's' || input[0] == 'r' || input[0] == 'q' || input[0] == 'f' ||
					input[0] == 'S' || input[0] == 'R' || input[0] == 'Q' || input[0] == 'F') {
				this->put(input[0]); // adds command to the list
			}
		}
	}
	// reader thread stops when reads the exit command
}

/**
 * the Game of Life main thread calls this function if it sees that the cellular automaton is completely empty
 * and there is no meaning in letting the simulation continue
 */
void Controller::setTermination() {
	terminate = true;
}
