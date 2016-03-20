#include <SDL.h>
#include "disk_class.h"


//ASSUMES screen width 640 and height 480
class Disks {
	public:
		Disks(int num, Uint32 pixel);
		~Disks();
		void render_all(SDL_Surface *surface);
		void update(SDL_Surface *surface, float t);
		static const float rmin = 5;
		//rmax determined per circle to avoid overlap
		static const float vmin = 60;
		static const float vmax = 180;
		static const int SCREEN_WIDTH = 640;
		static const int SCREEN_HEIGHT = 480;

	private:
		int n;
		Disk *disks;
		//returns time remaining in current frame after resolving collisions
		float resolve_collisions(SDL_Surface *surface, float t);
		void move(SDL_Surface *surface, float t);
		float col_time(Disk &a, Disk &b);
		float dot(vec a, vec b);
		vec add(vec a, vec b);
		vec sub(vec a, vec b);
		vec mult(float a, vec b);
		void col_update(Disk &a, Disk &b);
		
};
