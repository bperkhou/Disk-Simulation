#include <SDL.h>
#include "disk_class.h"
#include <queue>
#include <vector>
#include <math.h>


/* collisions object
 * automatically stores ID1 < ID2, this is always preserved
 * t_col is an absolute measurement (from start of simulation)
 */
struct Col {
	double t_col;
	int ID1;
	int ID2;
	Col(int ID1, int ID2, double t_col) :
		t_col(t_col), ID1(std::min(ID1, ID2)), ID2(std::max(ID1, ID2)) {}
	bool operator<(const Col &a) const {return (a.t_col < t_col);}
};

/* class for creating numerous disks and tracking their interactions.
 * assumes that there are 4 walls on the outer edges (as given by screen
 * width and height parameters).
 */
class Disks {
	public:
		//generates nonintersecting disks at random
		Disks(int num, Uint32 pixel, int screen_width, int screen_height);
		~Disks();
		// renders all disks on surface
		void render_all(SDL_Surface *surface);
		// input is the time alloted to the current frame
		void update(double t_frame);

		// predetermined constants for the randomly generated disks
		const double rmin = .1;
		const double vmin = 100;
		const double vmax = 180;


	private:
		// global time from start of system (as measured at beginning of cur frame)
		double TIME;
		
		// is there a way to make these "const" -- set them to a value and never change after?
		int SCREEN_WIDTH;
		int SCREEN_HEIGHT;
		
		// n is num disks, disks are the actual disks.
		int n;
		Disk *disks;
		
		// data structure for collisions (stores by order of occurence)
		std::priority_queue <Col, std::vector<Col>, std::less<Col> > cols;
		
		// loops over valid collisions within [TIME, TIME+t_frame] and updates the
		// system accordingly
		double resolve_collisions(double t_frame);
		
		// moves all disks by time t
		void move(double t);
		
		// functions for computing collision time
		double col_time(int ID1, int ID2, double t_cur);
		double col_time_disk(Disk &a, Disk &b, double t_cur);
		
		// vector operations for computing the physics
		double dot(vec a, vec b);
		vec add(vec a, vec b);
		vec sub(vec a, vec b);
		vec mult(double a, vec b);
		
		// update the velocities of disks at time of collision
		void update_disks(int ID1, int ID2);
		void update_disks_disk(Disk &a, Disk &b);
		
		// adds new collisions to the p_queue
		void update_cols(int ID1, int ID2, double t_cur);
		
		// checks if a collision is valid
		bool invalid_col(Col col, double t_cur);
};
