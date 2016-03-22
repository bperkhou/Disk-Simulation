#include <SDL.h>
#include <utility>

#ifndef DISK_CLASS
#define DISK_CLASS

typedef std::pair <double, double> vec;

class Disk {
	public:
		Disk();
		Disk(Uint32 pixel);
		Disk(vec c, double r, Uint32 pixel);
		Disk(vec c, double r, vec v, Uint32 pixel);
		void set_c(vec cin);
		void set_v(vec vin);
		void set_r(double radius);
		void set_color(Uint32 pixel);
		
		vec get_c();
		vec get_v();
		double get_r();
		
		void render(SDL_Surface *surface);
		void move(SDL_Surface *surface, double t);

	private:
		void render_backend(SDL_Surface *surface, double r, Uint32 pixel);
		void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
		bool resolve_wall_xcollisions(double w, double t);
		bool resolve_wall_ycollisions(double h, double t);
		Uint32 color;
		vec c;
		vec v;
		double r;
};

#endif /* DISK_CLASS */