#ifndef STM32_MIOSIX_GAMEOFLIFE_GAMEOFLIFE_H
#define STM32_MIOSIX_GAMEOFLIFE_GAMEOFLIFE_H

#include <iostream>
#include <chrono>
#include <thread>
#include <random>
#include "Controller.h"

#define living "█" // living cell character representation

class GameOfLife {
private:
	int rows, cols; // size of the cellular automaton
    bool **currentState; // current state of the cellular automaton
    bool **previousState; // previous state of the cellular automaton

public:
    GameOfLife(int rows, int cols);
	~GameOfLife();

    int liveNeighbours(int x, int y);
    bool wasAlive (int x, int y);
    bool isAlive (int x, int y);
	void changeCellState(int x, int y);

    void showState(); // prints the matrix
	void compute(); // actual code for the simulation

	void spawnGlider(int x, int y);
	void spawnSpaceship(int x, int y);
	void randomInitialization(); //unused function because it can be used only for testing

	static void clearScreen();


};


#endif //STM32_MIOSIX_GAMEOFLIFE_GAMEOFLIFE_H
