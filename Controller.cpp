//
// Created by simon on 17/11/21.
//

#include "Controller.h"

//for reference:
// https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads

Controller::Controller() {
	timeDelayIndex = 5;
}

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
		condition.wait(lock);
	}
	char result = actionsQueue.back();
	return result;
}

void Controller::put(char action) {
	std::unique_lock<std::mutex> lock(mutex);
	actionsQueue.push_front(action);
	condition.notify_one();
}

void Controller::putToSleep() {
	std::unique_lock<std::mutex> lock(mutex);
	actionsQueue.push_back('z');
	condition.notify_one();
}

bool Controller::hasSleeped() {
	std::unique_lock<std::mutex> lock(mutex);
	if (actionsQueue.empty()) return false;
	else return actionsQueue.back() == 'z';
}


bool Controller::inputManager() {

	// the main thread starts both threads and then waiting for the sequence to terminate
	//std::thread sleeper(&Controller::sleepingThread, this, 100);
	bool stopped = false;
	for (int i = 0; (i < timeDelays[timeDelayIndex] || stopped) && !terminate; i += 25) { //updates every 25 ms

		// here check for the time delay command change

		// analyze the queue from the head to the tail to analyze the commands
		while (!actionsQueue.empty()) {
			if (this->get() == 's') {
				stopped = true;
				actionsQueue.pop_back();
				std::cout << "stopped\n";
			} else if (this->get() == 'r') {
				stopped = false;
				actionsQueue.pop_back();
				std::cout << "resumed\n";
			} else if (this->get() == 'f') {
				timeDelayIndex = (timeDelayIndex + 1) % 7;
				actionsQueue.pop_back();
			}

			if (!actionsQueue.empty() && this->get() == 'q') { // quit signal should break out the cycle and terminate even if the timer didn't expire
				//std::unique_lock<std::mutex> lock(mutex);
				terminate = true;
				std::cout << "terminate\n";
				break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds (25));
	}

	//actionsQueue.clear(); // clears the keys queue before the new iteration
	if (terminate) reader.join();
	return terminate;
}

/**
 * thread that sleeps for the required amount of milliseconds
 * @param millis number of milliseconds between a frame and the other
 */
void Controller::sleepingThread(int millis) {
	// alternative approach that doesn't make use of the wait_for function
	// uses periodic wake-ups for checking if the termination signal has been sent
	const int wakeUps = 50;
	for (int i = 0; i < millis && !isTerminating(); i += millis / wakeUps) {
		std::this_thread::sleep_for(std::chrono::milliseconds (millis/wakeUps));
		if (isTerminating()) {
			return;
		}
	}
	if (!isTerminating()) {
		this->putToSleep();
		readyInput.notify_one();
	}

	// elegant implementation using a condition variable with a timer that elapses
	// apparently the wait_for function doesn't exist in the miosix os
	/*
	std::unique_lock<std::mutex> lock(mutex);
	if (!readySleep.wait_for(lock, std::chrono::milliseconds(millis), isTerminating)) {
		this->putToSleep();
		readyInput.notify_one();
		std::cout << "timer expired before quit signal\n";
	} else {
		std::cout << "terminate signal received\n";
	}
	 */
}

/**
 * checks if the user has requested the simulation termination
 * @return boolean flag for the exit command
 */
bool Controller::isTerminating() const {
	return terminate;
}

void Controller::startReader() {
	reader = std::thread(&Controller::readerThread, this);
	//reader.join();
}

/**
 * thread that reads user input commands during simulation
 */
void Controller::readerThread() {
	char input[4] = "";
	while (input[0] != 'q' && !terminate) {
		// it should not wait for user input if the queue contains the end of sleep signal

		ssize_t data = read(STDIN_FILENO, input, sizeof input);
		if (data > 0) {
			if (input[0] == 's' || input[0] == 'r' || input[0] == 'q' || input[0] == 'f') {
				this->put(input[0]);
			}
		}
	}
	// reader thread stops when reads the exit command
}

void Controller::setTermination() {
	terminate = true;
}
