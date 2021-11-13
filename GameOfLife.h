#ifndef STM32_MIOSIX_GAMEOFLIFE_GAMEOFLIFE_H
#define STM32_MIOSIX_GAMEOFLIFE_GAMEOFLIFE_H

#include <iostream>
#include <chrono>
#include <thread>
#include <random>

#define living "█"

class GameOfLife {
private:
	int rows, cols;
    bool **currentState;
    bool **previousState;
	int timeDelay = 100;

public:
    GameOfLife(int rows, int cols);
	~GameOfLife();


    int liveNeighbours(int x, int y);
    bool wasAlive (int x, int y);
    bool isAlive (int x, int y);
	void changeCellState(int x, int y);

    void showState();
	void compute();

	void spawnGlider(int x, int y);
	void randomInitialization();

	static void clearScreen();

	void spawnSpaceship(int x, int y);
};


#endif //STM32_MIOSIX_GAMEOFLIFE_GAMEOFLIFE_H
