#include <stdio.h>
#include <SDL2/SDL.h>

// Define constants
const int WINDOW_WIDTH = 600;
const int WINDOW_HEIGHT = 600;
const int WINDOW_PITCH = WINDOW_WIDTH*sizeof(uint32_t);

const double G = 1.0;
const double PARTICLE_RADIUS = 6.0;
const double SOFTENING = PARTICLE_RADIUS / 3;


// Define vector
struct Vec3 {
	double x,y,z;

	Vec3& operator+=(const Vec3& other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	
	Vec3& operator-=(const Vec3& other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	
	Vec3& operator*=(double a) {
		x *= a;
		y *= a;
		z *= a;
		return *this;
	}

	Vec3& operator/=(double a) {
		x /= a;
		y /= a;
		z /= a;
		return *this;
	}
};

Vec3 operator*(const Vec3& v, const double a) {
	return Vec3 {
		a * v.x, a * v.y, a * v.z
	};
}

Vec3 operator/(const Vec3& v, const double a) {
	return Vec3 {
		v.x / a, v.y / a, v.z / a
	};
}

Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	return Vec3 {
		v1.x + v2.x, v1.y + v2.y, v1.z + v2.z
	};
}

Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	return Vec3 {
		v1.x - v2.x, v1.y - v2.y, v1.z - v2.z
	};
}

double dot(const Vec3& v1, const Vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

double abs(const Vec3& v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}


// Define color
struct Color {
	uint8_t r,g,b;
};

const Color RED {255,0,0};
const Color GREEN {0,255,0};
const Color BLUE {0,0,255};
const Color WHITE {255,255,255};
const Color BLACK {0,0,0};
const Color YELLOW {255,255,0};
const Color GRAY {51,51,51};


// Define Body
struct Body {
	Vec3 pos,vel;
	double mass;
	Color color;
};


// Calculate accelerations using the law of gravitation.
void update_acc(const Body* bodies, Vec3* acc, int N) {
	for(int n = 0; n < N; n++) {
		acc[n] = {0,0,0};
	}
	Vec3 tmp_acc, tmp_r;
	for(int i = 0; i < N-1; i++) {
		for(int j = i+1; j < N; j++) {
			tmp_r = bodies[j].pos - bodies[i].pos;
			tmp_acc = tmp_r/pow(abs(tmp_r) + SOFTENING, 3);
			acc[i] += tmp_acc * bodies[j].mass;
			acc[j] -= tmp_acc * bodies[i].mass;
		}
		acc[i] *= G;
	}
	acc[N-1] *= G;
}


// Draw bodies
void draw(void* pixels, const Body* bodies, int N) {
	for(int y = 0; y < WINDOW_HEIGHT; y++) {
		for(int x = 0; x < WINDOW_WIDTH; x++) {
			Color fill = GRAY;

			for(int n = 0; n < N; n++) {
				if(hypot(x - (double)WINDOW_WIDTH/2 - bodies[n].pos.x, -y + (double)WINDOW_HEIGHT/2 - bodies[n].pos.y) < PARTICLE_RADIUS) {
					fill = bodies[n].color;
				}
			}
			
			uint32_t* p = (uint32_t*)((uintptr_t)pixels + y * WINDOW_PITCH + x * sizeof(*p));
			*p++ =  fill.r<<24 | fill.g<<16 | fill.b<<8 | 0xff<<0;
		}
	}
}


int main() {
	// Initialize SDL window, renderer and texture
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
		
	// Preparation for simulation
	const int N = 3;
	Body bodies[N] = {
		Body{
			{0,0,0},
			{0,0,0},
			500,
			RED
		},
		Body{
			{-150,0,0},
			{0,-0.5,0},
			1,
			BLUE
		},
		Body{
			{100,0,0},
			{0,1.5,0},
			2,
			YELLOW
		}
	};
	Vec3 acc[N];
	double dt = 1.0;
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

	bool running = true;
	Uint32 frame_start = SDL_GetTicks();
	Uint32 sec_counter = SDL_GetTicks();
	// Main loop
	while(running) {
		frame_start = SDL_GetTicks();

		// Handle SDL events;
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
			}
		}

		// Simulate
		for(int n = 0; n < N; n++) {
			bodies[n].vel += acc[n]*dt/2;
			bodies[n].pos += bodies[n].vel*dt;
		}
		update_acc(bodies, acc, N);
		for(int n = 0; n < N; n++) {
			bodies[n].vel += acc[n]*dt/2;
		}

		// Draw bodies on the window
		draw(frame_buf, bodies, N);
		SDL_UpdateTexture(texture, NULL, frame_buf, WINDOW_PITCH);
		SDL_SetRenderTarget(renderer, NULL);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		// Calculate fps
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
