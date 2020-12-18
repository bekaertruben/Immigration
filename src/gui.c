#include "gui.h"

#include <SDL2/SDL.h>

/*
 * MACs with OS X 15 and Retina displays require all figures to have doubled scales
 * The same is not required on other operating systems
 */
#ifdef __MAC_10_15
	#define DPI_SCALING 2
#else
	#define DPI_SCALING 1
#endif

static SDL_Renderer *renderer;
static SDL_Window *window;

struct mouse mouse = {0, 0, 0};

struct input_states input_states = {
    closed: 0,
    mouse_handled: 1,
    step: 0,
    play: 0,
    save: 0,
};

void gui_init(const char *title, int w, int h) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Error while initializing SDL: %s\n", SDL_GetError());
		exit(1);
	}

	window = SDL_CreateWindow(title, 0, 0, w, h, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
	if (window == NULL) {
		printf("Error: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
}

void gui_quit() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

void read_input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			input_states.closed = 1;
			break;

		case SDL_MOUSEBUTTONDOWN:
			input_states.mouse_handled = 0;
			mouse.x = event.button.x;
			mouse.y = event.button.y;
			mouse.button = (event.button.button == SDL_BUTTON_LEFT);
			break;
		
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_SPACE:
					input_states.step = 1; break;
			
			case SDLK_p:
					input_states.play = 1; break;
			
			case SDLK_s:
					input_states.save = 1; break;
			
			default:
				break;
			}
			break;
		
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_p)
				input_states.play = 0;
			break;
		
		default: // the event is not relevant to what we attemt to do
			break;
		}
	}
}

void gui_clear(Colour c) {
	SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_RenderClear(renderer);
}

void draw_rectangle(Colour c, int x, int y, int width, int height) {
    SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
	SDL_Rect rectangle = { x * DPI_SCALING, y * DPI_SCALING, width * DPI_SCALING, height * DPI_SCALING};
    SDL_RenderFillRect(renderer, &rectangle);
}

void gui_update() {
	SDL_RenderPresent(renderer);
}

void wait(int ms) {
    SDL_Delay(ms);
}
