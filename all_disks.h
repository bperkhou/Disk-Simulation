#include <SDL.h>
#include "disk_class.h"
#include <queue>
#include <vector>
#include <math.h>


// collisions object
// automatically stores ID1 < ID2, always preserve this
// t_col is ALWAYS with respect to the beginning of the frame
struct Col {
	double t_col;
	int ID1;
	int ID2;
	Col(int ID1, int ID2, double t_col) :
		t_col(t_col), ID1(std::min(ID1, ID2)), ID2(std::max(ID1, ID2)) {}
	bool operator<(const Col &a) const {return (a.t_col < t_col);}
};

// for given disk: which disk it will hit first and when
// t_col is ALWAYS with respect to the beginning of the frame
struct Col_ID {
	double t_col;
	int ID;
	Col_ID() : t_col(-1) {}
	bool operator==(const Col_ID &a) const {
		return (t_col == a.t_col) && (ID == a.ID);
	}
};

//ASSUMES screen width 640 and height 480
class Disks {
	public:
		Disks(int num, Uint32 pixel);
		~Disks();
		void render_all(SDL_Surface *surface);
		void update(SDL_Surface *surface, double t_frame);
		const double rmin = 5;
		//rmax determined per circle to avoid overlap
		const double vmin = 280;
		const double vmax = 280;
		const int SCREEN_WIDTH = 640;
		const int SCREEN_HEIGHT = 480;

	private:
		double TIME;
		int n;
		Disk *disks;
		Col_ID *first_cols;
		std::priority_queue <Col, std::vector<Col>, std::less<Col> > cols;
		//returns time remaining in current frame after resolving collisions
		double resolve_collisions(SDL_Surface *surface, double t_frame);
		void move(SDL_Surface *surface, double t);
		double col_time(int ID1, int ID2, double t_used);
		double col_time_disk(Disk &a, Disk &b, double t_used);
		double dot(vec a, vec b);
		vec add(vec a, vec b);
		vec sub(vec a, vec b);
		vec mult(double a, vec b);
		void update_disks_disk(Disk &a, Disk &b);
		void update_disks(int ID1, int ID2);
		void update_cols(int ID1, int ID2, double t_used);
		
};
