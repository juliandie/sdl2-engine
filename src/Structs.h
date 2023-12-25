#pragma once
#include "Game.h"

#define PI 3.14159265

// typedef struct Task Task;
typedef struct Entity Entity;

#define SIZE 50

typedef struct Field {
	int field[SIZE][SIZE];
	int food[SIZE][SIZE];
	int build[SIZE][SIZE];
	int dont[SIZE][SIZE];
	int unusable[SIZE][SIZE];
};

typedef struct Food {
	int x;
	int y;
};

typedef struct Mass {
	int maxFields = 5;
	float hunger = 0.2f;
	float growth = 0.1f;
	float counter = 1.0f;
};

struct Entity {
	SDL_Texture* texture;

	SDL_Rect position;

	void update() {
				
	}

	void render(SDL_Renderer* renderer) {

		SDL_Rect tmp;
		tmp.x = position.x - position.w / 2;
		tmp.y = position.y - position.h / 2;
		tmp.w = position.w;
		tmp.h = position.h ;

		SDL_RenderCopy (  renderer, texture, 0, &tmp);
		// SDL_RenderCopyEx(renderer, texture, 0, &tmp, 180 -rotation, 0, SDL_FLIP_NONE);
	}
	
};


