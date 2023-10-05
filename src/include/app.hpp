#pragma once
#include <string>
#include "SDL.h"
#include "SDL_ttf.h"

enum Direction {
	Up, Down, Left, Right, NoDirection
};

enum GameStatus {
	LeftWon,
	LeftScored,
	RightWon,
	RightScored,
	KeepGoing
};

/* This class holds the window, renderer, etc. Stuff
 * that is used to draw the game to the window.
 * Also some game logic, which I shoved into a few functions
 */
class App {
private:
	SDL_Window *window;
	SDL_Renderer *renderer;
	TTF_Font *font;
	int screen_width, screen_height;
	
	/* Current state of the game: did either side win?
	 * who scored last? etc. GameTick() updates this.
	 */
	GameStatus status;

	/* Scores. */
	int score_left, score_right;

	/* Y coordinates of the paddles */
	int paddle_left, paddle_right;
	bool key_lup, key_ldown, key_rup, key_rdown;

	/* Ball coordinates */
	int ballx, bally;
	/* Ball velocity */
	int ballvx, ballvy;
	
	void RenderText(std::string text, int x, int y);
	bool CollisionCheck(int newx, int newy, int paddle_x, int paddle_y);
public:
	App(int scrw, int scrh);
	~App();
	
	void DrawBall();
	void DrawPaddles();
	void DrawScores();
	void DrawGame();
	
	void ResetBall();
	void SetBallSpeed(int x, int y, int px, int py);
	bool BallBounce(int x, int y, int paddle_x, int paddle_y, Direction bounce_dir);
	/* Set up internal variables into a state ready for a new game. */
	void GameInit();
	void GameTick();
	/* Main loop. Calls GameTick() every iteration.
	 * Also handles input.
	 */
	void GameLoop();
};
