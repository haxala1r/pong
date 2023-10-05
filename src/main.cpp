#include <iostream>
/* Cmake handles locating the include files and libraries 
 * through some good old dark magic.
 */
#include "SDL.h"
#include "geometry.hpp"
#include "app.hpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


using namespace std;

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "Cannot init SDL2";
		return -1;
	}
	TTF_Init();
	try {
		App app(SCREEN_WIDTH, SCREEN_HEIGHT);
		app.GameLoop();
	} catch (const char *e) {
		cout << e << endl;
	}
	SDL_Delay(500);
	SDL_Quit();
	return 0;
}
