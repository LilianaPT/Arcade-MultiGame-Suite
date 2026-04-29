//====== CAR RACING GAME ======

#ifndef CARGAME_H
#define CARGAME_H

#include <vector>
#include <string>
#include <utility>

/*class CarGame
 Logic for a vertical-scrolling racing game.
 This class handles high-performance input via Windows API, 
 dynamic object pooling for obstacles, and efficient buffer-based 
 rendering to the console.*/
class CarGame
{
private:
	// --- Constants & Dimensions ---
	static const int WIDTH = 80;					// Total horizontal space of the track
	static const int HEIGHT = 30;					// Total vertical space of the track
	
	// --- Game Entities ---
	int playerX, playerY;							// Current (x, y) coordinates for the player's car
	int score;										// Accumulated points (20 per obstacle cleared)
	bool gameOver;									// Flag to break the active game loop
	
	/*A vector of (x, y) pairs representing the position of enemy cars.
	 Uses std::pair for efficient coordinate storage.*/
	std::vector<std::pair<int, int>> obstacles;

	// --- Movement & Timing Variables ---
	int roadOffset;				// Used to animate the center divider lines
	int sleepTime;				// Control variable for game speed (Difficulty)
	int framesSinceLastSpawn;	// Counter to manage enemy car frequency
	int spawnRate;				// Threshold for spawning new obstacles

	// --- Private Methods (Internal Logic) ---
	void draw();					// Builds the frame in a string buffer and renders via ConsoleUtils
	void input();					// Uses GetAsyncKeyState for non-blocking player movement
	void logic();					// Handles movement updates, collision detection, and score scaling
	void resetGame();				// Re-initializes all variables to default for a new session
	void handlePostGameOption();	// Displays the End-Game menu and handles restart/quit intent
	void pauseMenu();				// Suspends game logic and renders the pause UI overlay

public:
	// --- Public Interface ---
	CarGame();				// Constructor: Sets initial game state and memory allocations
	void play();			// Main Game Executive: Controls the outer "Play Again" loop
};

#endif

