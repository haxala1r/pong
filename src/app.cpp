#include <cmath>
#include <string>
#include <iostream>
#include <cstdlib>
/* Thank you cmake for your magic with header files. */
#include "SDL.h"
#include "SDL_events.h"
#include "SDL_render.h"
#include "SDL_ttf.h"
#include "app.hpp"
#include "geometry.hpp"


#define BALL_R 5
#define BALL_SPEED 10
#define PADDLE_HEIGHT 80
#define PADDLE_WIDTH 10
#define PADDLE_X 20
#define PADDLE_SPEED 14

using std::abs;
using std::sin;
using std::cos;
using std::string;
using std::cout; /* testing - remove later. */

App::App(int scrw, int scrh) {
	/* main is responsible for calling SDL_Init */
	window = SDL_CreateWindow("Pong", scrw/3, scrh/3, scrw, scrh, 0);
	if (window == nullptr)
		throw "Can't create window";
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
		throw "Can't create renderer";

	screen_width = scrw;
	screen_height = scrh;

	font = TTF_OpenFont("./assets/Pacifico.ttf", 50);
	if (font == nullptr)
		throw "Can't open font file.";

	key_lup = key_ldown = key_rup = key_rdown = false;
	srand(time(0));
}

App::~App() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

void App::DrawBall() {
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	/* This isn't the most efficient algorithm (probably) but it's good
	 * enough (TM), and doesn't require more than high school maths.
	 */
	for (int x = 0; x < BALL_R; x++) {
		for (int y = 0; y < BALL_R; y++) {
			if ((x*x + y*y) <= (BALL_R * BALL_R)) {
				SDL_RenderDrawPoint(renderer, ballx + x, bally + y);
				SDL_RenderDrawPoint(renderer, ballx - x, bally - y);
				SDL_RenderDrawPoint(renderer, ballx + x, bally - y);
				SDL_RenderDrawPoint(renderer, ballx - x, bally + y);
			}
		}
	}
}

void App::DrawPaddles() {
	/* PADDLE_X is used as the boundary where a ball collides with
	 * the paddle, so we make sure the left paddle's right side (and vice versa)
	 * are exactly at the PADDLE_X (and SCREEN_WIDTH - PADDLE_X for the right paddle)
	 */
	SDL_Rect lp, rp;
	lp.y = paddle_left;
	lp.x = PADDLE_X - PADDLE_WIDTH;
	rp.y = paddle_right;
	rp.x = screen_width - PADDLE_X;
	lp.w = rp.w = PADDLE_WIDTH;
	lp.h = rp.h = PADDLE_HEIGHT;
	
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(renderer, &lp);
	SDL_RenderFillRect(renderer, &rp);
}

void App::RenderText(string text, int x, int y) {
	SDL_Color fg = { 0xFF, 0xFF, 0xFF, SDL_ALPHA_OPAQUE};
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;

	SDL_Surface *srf = TTF_RenderText_Solid(font, text.c_str(), fg);
	dst.w = srf->w;
	dst.h = srf->h;
	SDL_Texture *txt = SDL_CreateTextureFromSurface(renderer, srf);
	SDL_RenderCopy(renderer, txt, nullptr, &dst);
	
	SDL_FreeSurface(srf);
	SDL_DestroyTexture(txt);
}

void App::DrawScores() {
	RenderText(std::to_string(score_left), 240, 20);
	RenderText(std::to_string(score_right), screen_width - 240, 20);
}

void App::DrawGame() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(renderer);
	DrawPaddles();
	DrawBall();
	DrawScores();
}

void App::ResetBall() {
	ballx = screen_width / 2;
	bally = screen_height / 2;
	ballvx = -5; /* todo: randomize these a bit. */
	ballvy = 0;
}

void App::GameInit() {
	/* NOTE: the coordinates here refer to the center of whatever.
	 * ie. ballx and bally are the center of the ball. same goes for paddles.
	 */
	paddle_left = screen_height / 2;
	paddle_right = screen_height / 2;
	score_left = score_right = 0;
	ResetBall();
}

/* Returns true if the ball actually bounces. */
void App::SetBallSpeed(int x, int y, int px, int py) {
	/* When a ball bounces from a paddle, its current speed is
	 * completely disregarded. This is a good enough algorithm for this.
	 */
	//px += PADDLE_WIDTH / 2;
	py += PADDLE_HEIGHT / 2;
	double intersect_y = (py - y) / ((double)PADDLE_HEIGHT / 2);
	double angle = intersect_y * 0.8;
	
	ballvx = abs(cos(angle)) * (((x - px) > 0) ? 1 : -1) * BALL_SPEED;
	ballvy = -sin(angle) * BALL_SPEED;
}

bool App::CollisionCheck(int newx, int newy, int paddle_x, int paddle_y) {
	/* Check if the a ball at newx, newy with radius of BALL_R collides
	 * with the paddle at paddle_x, paddle_y with dimensions PADDLE_WIDTH, PADDLE_HEIGHT
	 * Why didn't I use these objects from the start? because I didn't think this
	 * far ahead.
	 */
	Point points[] = { Point(paddle_x, paddle_y), Point(paddle_x + PADDLE_WIDTH, paddle_y),
	Point(paddle_x + PADDLE_WIDTH, paddle_y + PADDLE_HEIGHT), Point(paddle_x, paddle_y + PADDLE_HEIGHT)};
	Rectangle rect(points[0], points[1], points[2], points[3]);
	Circle ballie(Point(newx, newy), BALL_R);

	return rect.Intersects(ballie);
}


void App::GameTick() {
	/* Handle Paddle movement */
	if (key_lup)
		paddle_left -= PADDLE_SPEED;
	if (key_ldown)
		paddle_left += PADDLE_SPEED;
	if (paddle_left < 0)
		paddle_left = 0;
	if ((paddle_left + PADDLE_HEIGHT) > screen_height)
		paddle_left = screen_height - PADDLE_HEIGHT;

	if (key_rup)
		paddle_right -= PADDLE_SPEED;
	if (key_rdown)
		paddle_right += PADDLE_SPEED;
	if (paddle_right < 0)
		paddle_right = 0;
	if ((paddle_right + PADDLE_HEIGHT) > screen_height)
		paddle_right = screen_height - PADDLE_HEIGHT;

	/* Ball movement */
	int newx = ballx + ballvx;
	int newy = bally + ballvy;
	
	/* check for upper and lower bounds */
	if (((newy - BALL_R) < 0) || ((newy + BALL_R) > screen_height)) {
		ballvy = -ballvy;
		GameTick();
		return;
	}

	/* paddles. */
	if (CollisionCheck(newx, newy, PADDLE_X - PADDLE_WIDTH, paddle_left)) {
		SetBallSpeed(ballx, bally, PADDLE_X, paddle_left);
		GameTick();
		return;
	}
	
	if (CollisionCheck(newx, newy, screen_width - PADDLE_X, paddle_right)) {
		SetBallSpeed(ballx, bally, screen_width - PADDLE_X + PADDLE_WIDTH, paddle_right);
		GameTick();
		return;
	}

	ballx = newx;
	bally = newy;
	if (ballx < 0) {
		status = (score_right >= 9) ? RightWon : RightScored ;
	} else if (ballx > screen_width) {
		status = (score_left >= 9) ? LeftWon : LeftScored;
	} else {
		status = KeepGoing;
	}
}

void App::GameLoop() {
	GameInit();
	while (true) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				return;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_UP: key_rup = true; break;
				case SDLK_DOWN: key_rdown = true; break;
				case SDLK_w: key_lup = true; break;
				case SDLK_s: key_ldown = true; break;
				default:
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
				case SDLK_UP: key_rup = false; break;
				case SDLK_DOWN: key_rdown = false; break;
				case SDLK_w: key_lup = false; break;
				case SDLK_s: key_ldown = false; break;
				default:
					break;
				}
				break;
			}
		}

		/* We're doing some game logic here. I'm still
		 * figuring this gamedev thing out, not sure if it's really
		 * okay to just dump this here, or if I should put it in GameTick
		 */
		
		GameTick();
		switch (status) {
			case LeftScored:
				score_left += 1;
				ResetBall();
				break;
			case LeftWon:
				return;
				break;
			case RightScored:
				score_right += 1;
				ResetBall();
				break;
			case RightWon:
				return;
				break;
			case KeepGoing: // FALLTHROUGH
			default:
				break;
		}
		DrawGame();
		SDL_RenderPresent(renderer);
		SDL_Delay(20);
	}
}

