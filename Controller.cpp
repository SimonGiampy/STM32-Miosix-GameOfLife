//
// Created by simon on 17/11/21.
//

#include "Controller.h"

//for reference:
// https://stackoverflow.com/questions/4792449/c0x-has-no-semaphores-how-to-synchronize-threads

Controller::Controller() = default;

/* Semaphore Implementation using condition variables and mutexes */
/*
void Controller::acquire() {
	std::unique_lock<std::mutex> lock(mutex);
	while (count == 0) {
		condition.wait(lock);
	}
	count--;
}

void Controller::release() {
	std::unique_lock<std::mutex> lock(mutex);
	count++;
	condition.notify_all();
}
*/

/* Synchronized Queue, using LIFO policy */

char Controller::get() {
	std::unique_lock<std::mutex> lock(mutex);
	while(actionsQueue.empty()) {
		condition.wait(lock);
	}
	char result = actionsQueue.front();
	//actionsQueue.pop_front();
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
	std::thread sleeper(&Controller::sleepingThread, this, 200);
	std::thread reader(&Controller::readerThread, this);

	std::unique_lock<std::mutex> lock(mutex);
	while (true) {
		readyInput.wait(lock);
		lock.unlock();
		if (this->hasSleeped()) { // quit signal should break out the cycle and terminate even if the timer didn't expire
			break;
		} else if (this->get() == 'q') {
			lock.lock();
			terminate = true;
			readySleep.notify_one();
			lock.unlock();
			break;
		}
		lock.lock();
	}
	sleeper.join();
	if (reader.joinable()) reader.join();

	actionsQueue.clear(); // clears the keys queue before the new iteration
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

/**
 * thread that reads user input commands during simulation
 */
void Controller::readerThread() {
	char input[4] = "";
	while (input[0] != 'q' && !this->hasSleeped()) {
		// it should not wait for user input if the queue contains the end of sleep signal

		ssize_t data = read(STDIN_FILENO, input, sizeof input);

		if (data > 0 ) {
			//std::cout << "data read = " << input << ".\n";
			if (input[0] == 's' || input[0] == 'q') {

				this->put(input[0]);
				readyInput.notify_one();

				if (input[0] == 's') {
					char resume[4];
					do {
						ssize_t resuming = read(STDIN_FILENO, resume, sizeof resume);
						if (resuming > 0) {
							if (resume[0] == 'r') { // resume input command
								this->put(resume[0]);
								readyInput.notify_one();
							}
						}
					} while (resume[0] != 'r');
				}
			}
		}


	}
}
