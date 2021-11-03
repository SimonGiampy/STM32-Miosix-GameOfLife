#include <iostream>
#define ROWS 10
#define COLS 10


#ifndef STM32_MIOSIX_GAMEOFLIFE_GAMEOFLIFE_H
#define STM32_MIOSIX_GAMEOFLIFE_GAMEOFLIFE_H


class GameOfLife {
private:
    bool currentState[ROWS][COLS];
    bool prevState[ROWS][COLS];
public:
    GameOfLife();
    void updateState(int x, int y, bool b);
    int liveNeighbours(int x, int y);
    bool wasAlive (int x, int y);
    bool isAlive (int x, int y);
    void updatePrevState(int x, int y, bool b);
    void showState();
	static void compute();
	void customInitialization();
	void randomInitialization();
};


#endif //STM32_MIOSIX_GAMEOFLIFE_GAMEOFLIFE_H
