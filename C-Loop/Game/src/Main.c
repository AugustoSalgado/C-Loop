#include <stdio.h>
#include <SDL.h>
#include "./Constants.h"

int game_is_running = FALSE;
int last_frame_time = 0;
float delta_time = 0;
int movement_multiplier_x = 1;
int movement_multiplier_y = 1;
int speed = 100;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;


struct ball {
	float x;
	float y;
	float w;
	float h;
} ball;


int initialize_window(void) 
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		fprintf(stderr, "Error initialiazing SDL");
		return FALSE;
	}


	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		SDL_WINDOW_BORDERLESS
		);
	if (!window) {
		fprintf(stderr, "Error creating SDL custom window");
		return FALSE;
	}


	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "Error crearing SDL renderer");
		return FALSE;
	}


	return TRUE;
}


void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


void setup() {
	ball.x = 20;
	ball.y = 20;
	ball.w = 15;
	ball.h = 15;
}


void process_input() {
	SDL_Event input_event;
	SDL_PollEvent(&input_event);

	switch (input_event.type) {
		case SDL_QUIT:
			game_is_running = FALSE;
			break;
		case SDL_KEYDOWN:
			if (input_event.key.keysym.sym == SDLK_ESCAPE){
				game_is_running = FALSE;
			}
			if (input_event.key.keysym.sym == SDLK_UP && speed < 300) {
				speed *= 1.05;
			}
			if (input_event.key.keysym.sym == SDLK_DOWN && speed > 50) {
				speed *= 0.95;
			}
			break;
	}
}


void update() {

	if (ball.x + ball.w >= WINDOW_WIDTH || ball.x <= 0)
		movement_multiplier_x *= -1;
	if (ball.y + ball.h >= WINDOW_HEIGHT|| ball.y <= 0)
		movement_multiplier_y *= -1;

	ball.x += speed * movement_multiplier_x * delta_time;
	ball.y += speed * movement_multiplier_y * delta_time;

	last_frame_time = SDL_GetTicks();
}


void render() {
	SDL_SetRenderDrawColor(renderer, 10, 0, 20, 200);
	SDL_RenderClear(renderer);

	SDL_Rect fake_ball = { ball.x, ball.y, ball.w, ball.h };

	SDL_SetRenderDrawColor(renderer, 230, 230, 240, 155);
	SDL_RenderFillRect(renderer, &fake_ball);


	SDL_RenderPresent(renderer);
}



int main()
{
	game_is_running = initialize_window();

	setup();

	while (game_is_running == TRUE) {
		delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
		last_frame_time += delta_time;
		process_input();
		update();
		render();
	}

	destroy_window();

	return 0;
}

