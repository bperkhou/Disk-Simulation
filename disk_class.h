#include <SDL.h>
#include <utility>

#ifndef DISK_CLASS
#define DISK_CLASS

// for storing centers and velocities
typedef std::pair <double, double> vec;

// class for dealing with an individual disk
class Disk {
	public:
		// Various constructors for convenience
		Disk();
		Disk(Uint32 pixel);
		Disk(vec c, double r, Uint32 pixel);
		Disk(vec c, double r, vec v, Uint32 pixel);
		
		// Setters:
		void set_c(vec cin);
		void set_v(vec vin);
		void set_r(double radius);
		void set_color(Uint32 pixel);
		
		// prints physics relevant attributes for debugging
		void print_disk();
		
		// getters
		vec get_c();
		vec get_v();
		double get_r();
		
		// renders disks on the surface (if it's on the surface)
		void render(SDL_Surface *surface);
		
		// moves disk based on velocity and input time
		void move(double t);

	private:
		// helper functions for rendering disks that loop through the relevant pixels
		void render_backend(SDL_Surface *surface, double r, Uint32 pixel);
		void set_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
		
		// the stored data for the disks
		Uint32 color;
		vec c;
		vec v;
		double r;
};

#endif /* DISK_CLASS */