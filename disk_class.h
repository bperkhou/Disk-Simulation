#include <SDL.h>
#include <utility>

#ifndef DISK_CLASS
#define DISK_CLASS

typedef std::pair <float, float> vec;

class Disk {
	public:
		Disk();
		Disk(Uint32 pixel);
		Disk(vec c, float r, Uint32 pixel);
		Disk(vec c, float r, vec v, Uint32 pixel);
		void set_c(vec cin);
		void set_v(vec vin);
		void set_r(float radius);
		void set_color(Uint32 pixel);
		
		vec get_c();
		vec get_v();
		float get_r();
		
		void render(SDL_Surface *surface);
		void move(SDL_Surface *surface, float t);

	private:
		void render_backend(SDL_Surface *surface, float r, Uint32 pixel);
		void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
		bool resolve_wall_xcollisions(float w, float t);
		bool resolve_wall_ycollisions(float h, float t);
		Uint32 color;
		vec c;
		vec v;
		float r;
};

#endif /* DISK_CLASS */