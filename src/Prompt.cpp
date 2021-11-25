//
// Created by simon on 19/11/21.
//

#include "includes/Prompt.h"

void Prompt::printLogo() {
	std::cout << "\n\r"
				 "   ___       _              _____                                _     \n\r"
				 "  |_  |     | |            /  __ \\                              ( )    \n\r"
				 "    | | ___ | |__  _ __    | /  \\/ ___  _ ____      ____ _ _   _|/ ___ \n\r"
				 "    | |/ _ \\| '_ \\| '_ \\   | |    / _ \\| '_ \\ \\ /\\ / / _` | | | | / __|\n\r"
				 "/\\__/ / (_) | | | | | | |  | \\__/\\ (_) | | | \\ V  V / (_| | |_| | \\__ \\\n\r"
				 "\\____/ \\___/|_| |_|_| |_|   \\____/\\___/|_| |_|\\_/\\_/ \\__,_|\\__, | |___/\n\r"
				 "                                                            __/ |      \n\r"
				 "                                                           |___/       \n\r"
				 " _____                                __    _     _  __                \n\r"
				 "|  __ \\                              / _|  | |   (_)/ _|               \n\r"
				 "| |  \\/ __ _ _ __ ___   ___     ___ | |_   | |    _| |_ ___            \n\r"
				 "| | __ / _` | '_ ` _ \\ / _ \\   / _ \\|  _|  | |   | |  _/ _ \\           \n\r"
				 "| |_\\ \\ (_| | | | | | |  __/  | (_) | |    | |___| | ||  __/           \n\r"
				 " \\____/\\__,_|_| |_| |_|\\___|   \\___/|_|    \\_____/_|_| \\___|           \n\r"
				 "                                                                       \n\r"
				 "                                                                       \n\r";
}

void Prompt::instructions() {
	std::cout << " Place the living cells in the cellular automaton before starting the simulation\n\r";
	std::cout << " Press the [directional arrows] to move between the cells\n\r";
	std::cout << " Press [spacebar] to confirm cell choice\n\r";
	std::cout << " Press [g] to place a glider in the cellular automaton\n\r";
	std::cout << " Press [s] to place a spaceship in the cellular automaton\n\r";
	std::cout << " Press [enter] to start the simulation\n\r";

	std::cout << "\n\r Instructions for playing the simulation: \n\r";
	std::cout << " Press [s] to stop the simulation, press [r] to resume the simulation\n\r";
	std::cout << " Press [q] to quit the program and terminate the simulation\n\r";
	std::cout << " Press [f] to change the simulation speed\n\r";

	std::cout << " \n\r Adjust the terminal size and font according to your preferences.\n\r"
				 " When you're ready press [spacebar] to start\n\r";
}