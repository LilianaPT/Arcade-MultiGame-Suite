#include "SnakeGame.h"
#include "ConsoleUtils.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <windows.h>		//Required for Sleep()

using namespace std;

/*Constructor
Initializes the game state, positions the snake in the center,
and seeds the random number generator for fruit spawning*/
SnakeGame::SnakeGame() {
	gameOver = false;
	dir = STOP;
	x = width / 2;
	y = height / 2;
	score = 0;
	nTail = 0;
	srand((unsigned int)time(0));
	spawnFruit();
	speed = 110;					//Initial delay in milliseconds
}

//Randomly places fruit within the boundaries of the game board.
void SnakeGame::spawnFruit() {
	fruitX = rand() % width;
	fruitY = rand() % height;
}

//Renders the game world
//Uses a string buffer and ConsoleUtils::gotoxy(0,0) to redraw the screen
//efficiently, which prevents the common "flickering" effect in console games.
void SnakeGame::draw() {
	ConsoleUtils::gotoxy(0, 0); //Jump to top (no flickering!)
	string buffer = "";

	//Build the top Wall
	for (int i = -1; i <= width; i++) {
		buffer += "#";
	}
	buffer += "\n";

	//Build the game board and entities (Snake and Fruit)
	for (int i = 0; i < height; i++) {
		for (int j = -1; j <= width + 2; j++) {
			if (j == -1 || j == width) {
				buffer += "#";				//Side Walls
			}
			else if (i == y && j == x) {
				buffer += "\033[32mO\033[0m";				//Snake Head
			}
			else if (i == fruitY && j == fruitX) {
				buffer += "\033[31m@\033[0m";				//Fruit
			}
			else {
				bool printTail = false;
				for (int k = 0; k < nTail; k++) {
					if (tailX[k] == j && tailY[k] == i) {
						buffer += "\033[32mo\033[0m";		//Snake Tail
						printTail = true;
						break;
					}
				}
				if (!printTail) {
					buffer += " ";
				}
			}
		}
		buffer += "\n";
	}

	//Built the bottom Wall
	for (int i = -1; i <= width; i++) {
		buffer += "#";
	}
	buffer += "\n";

	//Output the completed frame and UI elements
	cout << buffer;
	cout << "\n\033[36mScore: " << score << "\033[0m";
	cout << "\t\033[33mUse W,A,S,D to move and X to pause\033[0m" << endl;
}

//Non-blocking input handler.
//Uses _kbhit() to check for  keyboard activity without halting the game loop.
//Implements logic to prevent 180-degree turns (cannot move LEFT if currently RIGHT)
void SnakeGame::input() {
	if (_kbhit()) {
		int ch = _getch();
		if (ch == 0 || ch == 224) {			//Handle special keys.
			ch = _getch();
		}
		switch(ch) {
		case 75: case 'a': case 'A': if (dir != RIGHT) dir = LEFT; break;
		case 77: case 'd': case 'D': if (dir != LEFT) dir = RIGHT; break;
		case 72: case 'w': case 'W': if (dir != DOWN) dir = UP; break;
		case 80: case 's': case 'S': if (dir != UP) dir = DOWN; break;
		case 'x': case 'X': pauseMenu(); break;
		}
	}
}

//Suspends game execution
//Allows the user to resume gameplay or exit to the main arcade menu.
void SnakeGame::pauseMenu() {
	ConsoleUtils::gotoxy(0, height + 3);
	cout << "\033[35m=== GAME PAUSED ===\033[0m                                                          " << endl; // Added spaces to wipe old text
	cout << "\033[35m(C)ontinue or (Q)uit?\033[0m                   ";

	bool validChoice = false;
	while (!validChoice) {
		if (_kbhit()) {
			char choice = (char)_getch();
			if (choice == 'c' || choice == 'C') {
				validChoice = true;
				ConsoleUtils::gotoxy(0, height + 3);
				//Clear the pause menu text from the console
				cout << "                                         " << endl;
				cout << "                                         " << endl;
				return;
			}
			else if (choice == 'q' || choice == 'Q') {
				gameOver = true;
				return;
			}
		}
	}
}

//Displays the post-game menu.
//Prompts the user to restart the session or return to the main Arcade Multigame System
void SnakeGame::handlePostGameOptions() {
	system("cls");
	cout << "\t======================================" << endl;
	cout << "\t	    [GAME OVER!]       " << endl;
	cout << "\t======================================" << endl;
	cout << endl;
	cout << "\t  Final Score: " << score << endl;
	cout << endl;
	cout << "\t======================================" << endl;
	cout << "\t 1. Play Again" << endl;
	cout << "\t 2. Return to Arcade Menu" << endl;
	cout << "\t Selection: ";

	char choice;
	while (true) {
		choice = (char)_getch();
		if (choice == '1') {
			resetGame();				//Re-initialize game state for new session
			return;
		}
		else if (choice == '2') {
			gameOver = true;			//Signal outer loop to return to main menu
			return;
		}
	}
}

//Hard reset of all game variables.
//Clear the tail vectors and reset difficulty and positions.
void SnakeGame::resetGame() {
	gameOver = false;
	score = 0;
	nTail = 0;
	tailX.clear();
	tailY.clear();
	x = width / 2;
	y = height / 2;
	dir = STOP;
	speed = 110; 
	spawnFruit();
}

//Core Game Logic
//Manages tail movement, head-to-fruit collision, head-to-wall collision,
//and difficulty scaling based on player score.
void SnakeGame::logic() {
	//Tail logic
	int prevX = tailX.size() > 0 ? tailX[0] : x;
	int prevY = tailY.size() > 0 ? tailY[0] : y;
	int prev2X, prev2Y;

	if (nTail > 0) {
		tailX[0] = x;
		tailY[0] = y;

		for (int i = 1; i < nTail; i++) {
			prev2X = tailX[i];
			prev2Y = tailY[i];
			tailX[i] = prevX;
			tailY[i] = prevY;
			prevX = prev2X;
			prevY = prev2Y;
		}
	}

	//Directional movement
	switch (dir) {
	case LEFT: x--; break;
	case RIGHT: x++; break;
	case UP: y--; break;
	case DOWN: y++; break;
	}

	//Border Collision
	if (x >= width || x < 0 || y >= height || y < 0) {
		gameOver = true;
	}

	//Self Collision
	for (int i = 0; i < nTail; i++) {
		if (tailX[i] == x && tailY[i] == y) {
			gameOver = true;
		}
	}

	//Eat Fruit
	if (x == fruitX && y == fruitY) {
		score += 20;
		nTail++;
		tailX.push_back(0);				//Grow the vectors
		tailY.push_back(0);
		spawnFruit();
	}

	//Increase speed every 200 points
	if (score > 0 && score % 200 == 0) {
		if (speed > 35) {
			speed -= 15;				//Reduce the sleep time by 15ms.
		}		
	}
}

//Primary Control Loop
//Implements a nested loop structure to allow multiple playthroughs
//without existing the application
void SnakeGame::run() {
	ConsoleUtils::hideCursor();

	bool exitToMenu = false;
	//Active gameplay loop
	while (!exitToMenu) {
		while (!gameOver) {
			draw();
			input();
			logic();
			Sleep(speed);		//Slightly faster speed
		}

		//Post-game handling
		handlePostGameOptions();

		//Check if user chose to quit to Arcade Menu
		if (gameOver) {
			exitToMenu = true;
		}
	}
	system("cls");				//Clean up console before returning
}
