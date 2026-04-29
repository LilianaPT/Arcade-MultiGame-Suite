#include "CarGame.h"
#include "ConsoleUtils.h"
#include <iostream>
#include <conio.h>
#include <ctime>
#include <windows.h>

using namespace std;

/*Constructor
Initializes the game by calling the reset method to set default values.*/
CarGame::CarGame() {
	resetGame();
}

//Resets all game variables to their starting state.
void CarGame::resetGame() {
	playerX = WIDTH / 2;
	playerY = HEIGHT - 4;
	gameOver = false;
	score = 0;
	roadOffset = 0;
	sleepTime = 50;
	spawnRate = 15;				//Minimum frames between spawns
	framesSinceLastSpawn = 0;
	obstacles.clear();
	srand((unsigned int)time(0));
}

//Renders the game frame to the console.
//Uses a string buffer to store the entire frame before printing to minimize flickering.
void CarGame::draw() {
	string buffer = "";
	buffer.reserve((WIDTH + 1) * HEIGHT);		// Optimization to prevent multiple reallocations

	// Generate the track boundaries
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			if (j == 0 || j == WIDTH - 1) {
				buffer += '|';			//Side walls
			}
			else {
				buffer += ' ';
			}
		}
		buffer += '\n';
	}

	// Draw moving road lines based on the current roadOffset
	for (int i = 0; i < HEIGHT; i++) {
		if ((i + roadOffset) % 4 == 0) {
			int index = i * (WIDTH + 1) + WIDTH / 2;
			if (index >= 0 && index < buffer.length()) // Safety check
				buffer[index] = ':';
		}
	}

	//Draw Obstacles (Enemy Cars)
	for (auto& obs : obstacles) {
		int x = obs.first;
		int y = obs.second;

		// ASCII Art for enemy cars (marked with 'X')
		if (y > 0 && y < HEIGHT - 3 && x > 2 && x < WIDTH - 3) {
			for (int i = 0; i < 3; i++) {
				buffer[y * (WIDTH + 1) + x - 1 + i] = '^';
			}
			buffer[(y + 1) * (WIDTH + 1) + x - 2] = '/';
			buffer[(y + 1) * (WIDTH + 1) + x - 1] = 'X';
			buffer[(y + 1) * (WIDTH + 1) + x + 1] = 'X';
			buffer[(y + 1) * (WIDTH + 1) + x + 2] = '\\';
			buffer[(y + 2) * (WIDTH + 1) + x - 2] = '[';
			buffer[(y + 2) * (WIDTH + 1) + x] = 'X';
			buffer[(y + 2) * (WIDTH + 1) + x + 2] = ']';
		}
	}

	// Render the player's car (marked with 'A' and '0')
	if (playerY > 0 && playerX > 2 && playerX < WIDTH - 3) {
		for (int i = 0; i < 5; i++) {
			buffer[playerY * (WIDTH + 1) + playerX - 2 + i] = '^';
		}
		buffer[(playerY + 1) * (WIDTH + 1) + playerX - 2] = '/';
		buffer[(playerY + 1) * (WIDTH + 1) + playerX - 1] = 'A';
		buffer[(playerY + 1) * (WIDTH + 1) + playerX + 1] = 'A';
		buffer[(playerY + 1) * (WIDTH + 1) + playerX + 2] = '\\';
		buffer[(playerY + 2) * (WIDTH + 1) + playerX - 2] = '[';
		buffer[(playerY + 2) * (WIDTH + 1) + playerX] = '0';
		buffer[(playerY + 2) * (WIDTH + 1) + playerX + 2] = ']';
	}

	// Update console output
	ConsoleUtils::gotoxy(0, 0);
	cout << buffer;
	cout << "\n\033[36mScore: " << score << "\033[0m";
	cout << "\t\033[33mUse W,A,S,D to move and P to pause\033[0m" << endl;
	cout << "\n\t\033[31mAvoid enemy cars (X)\033[0m" << endl;
		
}

//Processes keyboard input.
// Uses GetAsyncKeyState for smoother real - time movement compared to standard buffered input.
void CarGame::input(){
	// Check for Left/Right movement keys
	if (GetAsyncKeyState('A') & 0x8000 || (GetAsyncKeyState(VK_LEFT) & 0x8000)) {
		if (playerX > 3) playerX--;
	}
	if (GetAsyncKeyState('D') & 0x8000 || (GetAsyncKeyState(VK_RIGHT) & 0x8000)) {
		if (playerX < WIDTH - 4) playerX++;
	}

	// Check for menu-based inputs (Pause/Quit)
	if (_kbhit()) {
		int key = _getch();

		if (key == 0 || key == 224) {
			key = _getch();
		}

		switch (key) {
		case 'p': case 'P': pauseMenu(); break;
		case 'q': case 'Q': gameOver = true; break;
		}
	}
}

//Logic for pausing the game.
//Halts execution until the user chooses to continue or quit.
void CarGame::pauseMenu() {
	ConsoleUtils::gotoxy(0, HEIGHT + 3);
	cout << "\033[35m=== GAME PAUSED ===\033[0m                                                          " << endl; // Added spaces to wipe old text
	cout << "\033[35m(C)ontinue or (Q)uit?\033[0m                   ";

	bool validChoice = false;
	while (!validChoice) {
		if (_kbhit()) {
			char choice = (char)_getch();
			if (choice == 'c' || choice == 'C') {
				validChoice = true;
				// Clear the pause text lines
				ConsoleUtils::gotoxy(0, HEIGHT + 3);
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

//Core game logic including movement, collision detection, and score tracking.
void CarGame::logic() {
	// Advance obstacles downward
	for (auto& obs : obstacles) obs.second++;

	// Remove obstacles that have passed the bottom of the screen and update score
	if (!obstacles.empty() && obstacles[0].second >= HEIGHT) {
		obstacles.erase(obstacles.begin());
		score += 20;

		// Difficulty scaling: Increase speed and spawn rate every 200 points
		if (score > 0 && score % 200 == 0) {
			if (sleepTime > 15) sleepTime -= 5;	//Speed up the loop
			if (spawnRate > 6) spawnRate -= 1;	//Decrease gap between enemies
		}
	}

	// Spawn logic for new obstacles
	framesSinceLastSpawn++;
	if (framesSinceLastSpawn >= spawnRate) {
		if (rand() % 5 == 0) {
			int startX = (rand() % (WIDTH - 10)) + 5;
			obstacles.push_back({ startX, 0 });
			framesSinceLastSpawn = 0;		//Reset timer
		}
	}

	// Collision detection using a simple hitbox approach
	for (auto& obs : obstacles) {
		if (abs(obs.first - playerX) < 5 && abs(obs.second - playerY) < 3) {
			gameOver = true;
		}
	}

	// Update road marking animation state
	roadOffset = (roadOffset + 1) % 4;
}

//Entry point for running the game.
//Contains the main game loop.
void CarGame::play() {
	ConsoleUtils::hideCursor();

	bool exitToMenu = false;
	while (!exitToMenu) {
		while (!gameOver) {
			draw();
			input();
			logic();
			Sleep(sleepTime);		//Speed for the race
		}

		handlePostGameOption();

		if (gameOver) {
			exitToMenu = true;
		}
	}
	system("cls");
}

//Menu displayed after the game ends.
//Handles the logic for playing again or returning to the main menu.
void CarGame::handlePostGameOption() {
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
			resetGame();
			return;
		}
		else if (choice == '2') {
			gameOver = true;
			return;
		}
	}
}
