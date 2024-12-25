#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <math.h>
#include "defs.h"

enum direction  {
	UP,
	DOWN,
	RIGHT,
	LEFT,
	ZERO
};

typedef struct sprite {
	SDL_Rect rect;
	float speed;
	float angle;
	float rad;
	enum direction direction;
	SDL_Texture* texture;
} Sprite;

Sprite ball;
Sprite bar;

bool init(SDL_Window** win, SDL_Renderer** rend) {
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		perror("SDL");
		return false;
	}
	*win = SDL_CreateWindow(
		"title",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WIN_WIDTH,
		WIN_HEIGHT,
		0
			);

	if (!*win) {
		perror("window");
		return false;
	}

	*rend = SDL_CreateRenderer(*win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!*rend) {
		perror("renderer");
		return false;
	}


	printf("init complete\n");
	return true;
}

void input(bool* running) {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
				*running = false;
				printf("break\n");
				break;
			case SDL_KEYUP:
				bar.direction = ZERO;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						*running = false;
						break;
					case SDLK_LEFT:
						bar.direction = LEFT;
					case SDLK_RIGHT:
						bar.direction = RIGHT;
						break;
				break;
				}
		}
	}
}

void render(SDL_Renderer* rend, SDL_Rect rect) {

	SDL_SetRenderDrawColor(rend, 0,0 ,0, 255);
	SDL_RenderClear(rend);

	SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
	SDL_RenderFillRect(rend, &ball.rect);
	
	SDL_RenderFillRect(rend, &bar.rect);

	SDL_RenderPresent(rend);
	SDL_RenderClear(rend);

}

void update(float* delta, SDL_Rect* rect, float* angle) {
	*angle = -180.0f;
	ball.rad = ball.angle * (M_PI / 180.0f);
	int last_frame_time;
	int wait_time = FRAME_TIME - (SDL_GetTicks() - last_frame_time);
	if (wait_time > 0 && wait_time <= FRAME_TIME) {
		SDL_Delay(wait_time);
	}

	*delta = (SDL_GetTicks() - last_frame_time) / 1000.0f;

	last_frame_time = SDL_GetTicks();

	float speed = 5.0f;



	ball.rect.x += cos(ball.rad) * speed * *delta;
	ball.rect.y += sin(ball.rad) * speed * *delta;

	//
	//rect->y += 10 * *delta;
	//
	
	if (rect->x >= WIN_WIDTH) {
		//angle = 360.0f - angle;
		//speed = -5.0f;
		*angle += 180.0f;
	} 
	else if (rect->x <= 0) {
		*angle += 180.0f;
	}
	else if (rect->y <= 0) {
		*angle += 180.0f;
	}
	else if (rect->y >= WIN_HEIGHT) {
		*angle = 180.0f;
	}

	switch (bar.direction) {
		case LEFT:
			bar.rect.x -= bar.speed;// * *delta;
			break;
		case RIGHT:
			bar.rect.x += bar.speed;// * *delta;
			break;
		case UP:
			bar.rect.y -= bar.speed;// * *delta;
			break;
		case DOWN:
			bar.rect.y += bar.speed;// * *delta;
			break;
		case ZERO:
			break;
	}
	

	if (bar.rect.x <= 5) {
		bar.direction = ZERO;
	}
	else if (bar.rect.x >= WIN_WIDTH + 5) {
		bar.direction = ZERO;
	}
	


}

void destroy(SDL_Window** win, SDL_Renderer** rend) {
	SDL_DestroyRenderer(*rend);
	SDL_DestroyWindow(*win);
	SDL_Quit();
}


void setup() {
	ball.rect.x = WIN_WIDTH / 2;
	ball.rect.y = WIN_HEIGHT / 2;
	ball.rect.w = 10;
	ball.rect.h = 10;
	ball.speed = 5.0f;
	ball.angle = 45.0f;

	bar.rect.x = WIN_WIDTH / 2;
	bar.rect.y = 600;
	bar.rect.w = 100;
	bar.rect.h = 10;
	bar.speed = 10;
	bar.direction = ZERO;
	
}

int main() {
	SDL_Window* win;
	SDL_Renderer* rend;
	float delta;
	bool running = false;
	running = init(&win, &rend);
	SDL_Rect rect;
	float angle = 0.0f;
	rect.w = 15;
	rect.h = 15;
	rect.x = 100;
	rect.y = 100;

	setup();

	while (running) {
		input(&running);
		render(rend, rect);
		update(&delta, &rect, &angle);
	}
	printf("exiting\n");
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	//destroy(&win, &rend);
}
