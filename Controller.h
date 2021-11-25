//
// Created by simon on 17/11/21.
//

#ifndef STM32_MIOSIX_GAMEOFLIFE_CONTROLLER_H
#define STM32_MIOSIX_GAMEOFLIFE_CONTROLLER_H

#include <atomic>
#include <mutex>
#include <condition_variable>
#include <list>
#include <thread>
#include <iostream>
#include <unistd.h>

class Controller {

private:

	// synchronized queue with FIFO policy
	std::condition_variable listCondition;
	std::mutex mutex;
	std::list<char> actionsQueue;

	bool terminate = false; // simulation termination check
	int timeDelayIndex; // expressed in milliseconds
	// iterate over this array that represents the delay times in milliseconds
	const int timeDelays[7] = {50, 100, 250, 500, 750, 1000, 2000};

	std::thread reader; // reads user input asynchronously

public:
	Controller();
	~Controller();

	// synchronized queue code
	void put(char action);
	char get();

	// concurrency management
	bool inputManager(); // returns true if the user wishes to terminate the simulation
	void startReader();
	void readerThread(); // thread that reads user input
	void setTermination(); // sets termination flag

};

#endif //STM32_MIOSIX_GAMEOFLIFE_CONTROLLER_H
