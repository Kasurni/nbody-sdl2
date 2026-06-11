#include <stdio.h>
#include <SDL2/SDL.h>
#include "linear_algebra.hpp"
#include "color.hpp"

const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int WINDOW_PITCH = WINDOW_WIDTH*sizeof(uint32_t);

const double G = 1.0;

struct Body {
	Vec3 pos,vel;
	double mass;
	Color color;
};

void update_acc(const Body* bodies, Vec3* acc, int n) {
	Vec3 tmp_acc;
	Vec3 tmp_r = { 0, 0, 0 };
	for(int i = 0; i < n; i++) {
		tmp_acc = { 0, 0, 0 };
		for(int j = 0; j < n; j++) {
			if(i == j) continue;
			tmp_r = bodies[j].pos - bodies[i].pos;
			tmp_acc += tmp_r/pow(abs(tmp_r), 3) * bodies[j].mass;
		}
		acc[i] = tmp_acc * G;
	}
}

void draw(void* pixels, const Body* bodies, int N) {
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			Color fill = GRAY;

			for(int n = 0; n < N; n++) {
				if(hypot(x - (double)WINDOW_WIDTH/2 - bodies[n].pos.x, -y + (double)WINDOW_HEIGHT/2 - bodies[n].pos.y) < 6) {
					fill = bodies[n].color;
				}
			}
			
			uint32_t* p = (uint32_t*)((uintptr_t)pixels + y * WINDOW_PITCH + x * sizeof(*p));
			*p++ =  fill.r<<24 | fill.g<<16 | fill.b<<8 | 0xff<<0;
		}
	}
}

int main() {

	if(SDL_Init(SDL_INIT_VIDEO) < 0) return 1;
	SDL_Window* window = SDL_CreateWindow(
		"nbody-sdl2",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH, WINDOW_HEIGHT, 
		0
	);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Texture* texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
			WINDOW_WIDTH, WINDOW_HEIGHT
	);

	uint8_t frame_buf[WINDOW_PITCH*WINDOW_HEIGHT];
	
	bool running = true;
	
	const int N = 3;
	Body bodies[N] = {
		Body{
			{0,0,0},
			{0,0,0},
			1000,
			RED
		},
		Body{
			{150,20,0},
			{-1,1.75,0},
			10,
			BLUE
		},
		Body{
			{150,-20,0},
			{0,1.75,0},
			10,
			YELLOW
		}
	};
	Vec3 acc[N];
	double dt = 1.0;

	// Adjust by the velocity of the center of mass
	Vec3 adjust;
	double mass_sum = 0;
	for(int n = 0; n < N; n++) {
		adjust += bodies[n].vel * bodies[n].mass;
		mass_sum += bodies[n].mass;
	}
	adjust /= mass_sum; 

	for(int n = 0; n < N; n++) {
		bodies[n].vel -= adjust;
	}

	Uint32 frame_start = SDL_GetTicks();
	Uint32 sec_counter = SDL_GetTicks();

	while(running) {
		frame_start = SDL_GetTicks();
		SDL_Event event;

		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
			}
		}

		for(int n = 0; n < N; n++) {
			bodies[n].vel += acc[n]*dt/2;
			bodies[n].pos += bodies[n].vel*dt;
		}
		update_acc(bodies, acc, N);
		for(int n = 0; n < N; n++) {
			bodies[n].vel += acc[n]*dt/2;
		}

		draw(frame_buf, bodies, N);
		SDL_UpdateTexture(texture, NULL, frame_buf, WINDOW_PITCH);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		if(frame_start - sec_counter >= 1000) {
			printf("\rFPS:%3d", 1000/(SDL_GetTicks() - frame_start));
			fflush(stdout);
			sec_counter = SDL_GetTicks();
		}
	}
	printf("\n");

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
