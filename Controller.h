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

	// semaphore code
	std::condition_variable condition;

	// synchronized queue
	std::condition_variable readyInput, readySleep;
	std::mutex mutex;
	std::list<char> actionsQueue;

	bool terminate = false; // simulation termination check


public:
	Controller();

	// semaphore code
	//void acquire();
	//void release();

	// synchronized queue code
	void put(char action);
	char get();
	bool hasSleeped();
	void putToSleep();

	// concurrency management
	bool inputManager(); // returns true if the user wishes to terminate the simulation
	void sleepingThread(int millis);
	void readerThread();
	bool isTerminating() const;
};



#endif //STM32_MIOSIX_GAMEOFLIFE_CONTROLLER_H
