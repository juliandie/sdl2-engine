
#include "Game.h"

#define FPS_TO_MS(x)	(static_cast<float>(1) / x) * 1000
#define max(a,b) (((a) > (b)) ? (a) : (b))

Game* game = nullptr;


int main() {
	
	game = new Game();

	int flags = 0;

	if (false) {
		flags = SDL_WINDOW_FULLSCREEN;
	}

	game->init("title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGTH, flags);

	long lastTimer = Game::GetTicks();

	double t = 0.0;
	const double dt = 10;

	double currentTime = Game::GetTicks();
	double accumulator = 0.0;


	while (game->running()) {

		Uint64 start = SDL_GetPerformanceCounter();

		double newTime = Game::GetTicks();
		double frameTime = newTime - currentTime;
		currentTime = newTime;

		accumulator += frameTime;
				
		game->handleEvents();

		while (accumulator >= dt) {
			game->update(dt);

			game->ticks++;
			accumulator -= dt;
			t += dt;
		}


		game->render(game->ticks);

		game->frames++;

		if ((Game::GetTicks() - lastTimer) >= 1000) {
			lastTimer += 1000;
			game->frames = 0;
			game->ticks = 0;
		}

		Uint64 end = SDL_GetPerformanceCounter();

		float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
		SDL_Delay(max(0,floor(FPS_TO_MS(144) - elapsedMS)));
	}

	game->clean();

	return 0;
}



