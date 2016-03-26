#include "all_disks.h"
#include <iostream>
#include <time.h>
#include <map>
#include <stdlib.h>


//initialize centers first, then pick random radii that don't cause intersections
Disks::Disks(int num, Uint32 pixel){
	n = num;
	TIME = 0;
	//srand(time(NULL));
	srand(0);
	disks = new Disk[n];
	for(int i = 0; i < n; i++){
		disks[i].set_color(pixel);
		disks[i].set_c(vec((double) (rand() % SCREEN_WIDTH), (double) (rand() % SCREEN_HEIGHT)));
		double vr = ((double) rand() / (double) RAND_MAX)*(vmax-vmin)+vmin;
		double vtheta = (double) rand();
		disks[i].set_v(vec(vr*cos(vtheta), vr*sin(vtheta)));
		disks[i].set_r(0);
	}
	for(int i=0; i < n; i++){
	vec c = disks[i].get_c();
	double rmax = std::min(std::min(c.first, c.second), std::min(SCREEN_WIDTH-c.first, SCREEN_HEIGHT-c.second));
		for(int j=0; j < n; j++){
			if (i == j) continue;
			vec dc = sub(disks[i].get_c(), disks[j].get_c());
			double dist = sqrt(dot(dc, dc));
			rmax = std::min(rmax, dist-disks[j].get_r());
		}
	disks[i].set_r(((double) rand() / (double) RAND_MAX)*(rmax-rmin)+rmin);
	}
	
	//initialize collision handling
	//keeps track of soonest collisions per disk
	//ID of walls are n to n+3 starting at left wall and going clockwise
	first_cols = new Col_ID[n+4];
	for(int i=0; i < n+4; i++){
		// inner loop includes walls
		for(int j=0; j < n+4; j++){
			double t_col = col_time(i, j, 0);
			//std::cout << "init: potential col in " << t_col <<
			//	" with " << j << std::endl;
			if (((t_col < first_cols[i].t_col) && (t_col != -1)) || (first_cols[i].t_col == -1)){
				first_cols[i].t_col = t_col;
				first_cols[i].ID = j;
			}
			if (t_col != -1) cols.emplace(Col(i, j, t_col));
		}
	}
	
	for(int i=0; i < n; i++){
		std::cout << "initialization: first collision of disk " <<
			i << " with object " << first_cols[i].ID << " in " <<
			first_cols[i].t_col << std::endl;
		std::cout << "init: disk i radius cx cy vx vy " << i << " " <<
			disks[i].get_r() << " " << disks[i].get_c().first << " " <<
			disks[i].get_c().second << " " << disks[i].get_v().first <<
			" " << disks[i].get_v().second << std::endl;
	}
	
}


Disks::~Disks(){
	delete [] disks;
	delete [] first_cols;
}

void Disks::render_all(SDL_Surface *surface){
	for(int i = 0; i < n; i++){
		disks[i].render(surface);
	}
}

//t_frame is frame time limit
void Disks::update(SDL_Surface *surface, double t_frame){
	double t_remain = TIME + t_frame - resolve_collisions(surface, t_frame);
	move(surface, t_remain);
	TIME += t_frame;
}

bool Disks::invalid_col(Col col){
	int ID1 = col.ID1;
	int ID2 = col.ID2;
	double t_col = col.t_col;
	Col_ID c1 = first_cols[ID1];
	Col_ID c2 = first_cols[ID2];
	bool ans = (t_col == -1) || ((c1.ID != ID2 || c1.t_col != t_col) && (c2.ID != ID1 || c2.t_col != t_col));
	/*
	if (ans) {
		std::cout << "discarding collision between " << ID1 << " and "
			<< ID2 << " at time " << t_col << std::endl;
		std::cout << "    collision of " << ID1 << " should be with " <<
			c1.ID << " at time " << c1.t_col << std::endl;
		std::cout << "    collision of " << ID2 << " should be with " <<
			c2.ID << " at time " << c2.t_col << std::endl;
	}
	*/
	return ans;
}

// t_used is frame time used up so far, t_frame is total frame time
double Disks::resolve_collisions(SDL_Surface *surface, double t_frame){
	double t_cur = TIME;
	while (t_cur < TIME + t_frame){
		Col cur = cols.top();
		while (invalid_col(cur)){
			cols.pop();
			cur = cols.top();
		}
		double t_col = cur.t_col;
		int ID1 = cur.ID1;
		int ID2 = cur.ID2;
		if (t_col > TIME + t_frame) break;
		cols.pop();
		/*
		std::cout << "Col between " << ID1 << " and " << ID2 <<
			" at time " << t_col << ". t_cur is " << t_cur <<
			" and TIME is " << TIME << std::endl;
		*/	
		move(surface, t_col-t_cur);
		update_disks(ID1, ID2);
		update_cols(ID1, ID2, t_col-TIME);
		
		//if (t_cur == t_col) {std::cout << "t_cur == t_col" << std::endl; break;}
		t_cur = t_col;
	}
	
	return t_cur;
}

void Disks::update_disks(int ID1, int ID2){
	if ((ID1 >= n) && (ID2 >= n)) return;
	if ((ID1 < n) && (ID2 < n)){
		update_disks_disk(disks[ID1], disks[ID2]);
		return;
	}
	double vx = disks[ID1].get_v().first;
	double vy = disks[ID1].get_v().second;
	if ((ID2-n) % 2 == 0){
		disks[ID1].set_v(vec(-vx, vy));
	} else {
		disks[ID1].set_v(vec(vx, -vy));
	}
}


void Disks::update_disks_disk(Disk &a, Disk &b){
	vec dc = sub(a.get_c(), b.get_c());
	vec dv = sub(a.get_v(), b.get_v());

	double ar = a.get_r();
	double br = b.get_r();

	double distsq = dot(dc, dc);
	double K = dot(dc, dv);

	K = K / distsq;
	double ma = ar*ar;
	double mb = br*br;
	double mbovertot = mb / (ma + mb);
	double maovertot = ma / (ma + mb);
	a.set_v(sub(a.get_v(),mult(2*mbovertot*K,dc)));
	b.set_v(add(b.get_v(),mult(2*maovertot*K,dc)));
}

//assumes input where ID1 is a disk
void Disks::update_cols(int ID1, int ID2, double t_used){
	first_cols[ID1].t_col = -1;
	first_cols[ID2].t_col = -1;
	double t_col1;
	double t_col2;
	for (int i=0; i<n+4; i++){
		t_col1 = (i==ID1) ? -1 : col_time(ID1, i, t_used);
		t_col2 = (i==ID2) ? -1 : col_time(ID2, i, t_used);
		if (first_cols[i].ID == ID1 || first_cols[i].ID == ID2) first_cols[i].ID = -1;
		if ((t_col1 < first_cols[ID1].t_col && t_col1 != -1) || (first_cols[ID1].t_col == -1)){
			first_cols[ID1].t_col = t_col1;
			first_cols[ID1].ID = i;
		}
		if ((t_col2 < first_cols[ID2].t_col && t_col2 != -1) || (first_cols[ID2].t_col == -1)){
				first_cols[ID2].t_col = t_col2;
				first_cols[ID2].ID = i;
			}
		if (((t_col1 < t_col2) || (t_col2 == -1)) && t_col1 != -1){
			if (t_col1 < first_cols[i].t_col || first_cols[i].t_col == -1 || first_cols[i].ID == ID1){
				/*
				std::cout << "OTHER UPDATE1: " << i << " first col with " <<
					ID1 << " at time " << t_col1 << std::endl;
				std::cout << "          OLD: " << " first col with " <<
					first_cols[i].ID << " at time " << first_cols[i].t_col << std::endl;
				std::cout << "    other col: " << "with ID = " << ID2 << " at time " << t_col2 << std::endl;
				*/
				first_cols[i].t_col = t_col1;
				first_cols[i].ID = ID1;
			}
		} else if (((t_col2 <= t_col1) || (t_col1 == -1)) && t_col2 != -1){
			if (t_col2 < first_cols[i].t_col || first_cols[i].t_col == -1 || first_cols[i].ID == ID2){
			/*
				std::cout << "OTHER UPDATE2: " << i << " first col with " <<
					ID2 << " at time " << t_col2 << std::endl;
				std::cout << "          OLD: " << " first col with " <<
					first_cols[i].ID << " at time " << first_cols[i].t_col << std::endl;
				std::cout << "    other col: " << "with ID = " << ID1 << " at time " << t_col1 << std::endl;
				*/
				first_cols[i].t_col = t_col2;
				first_cols[i].ID = ID2;
			}
		} /*else {
			std::cout << "  REJECTED: " << "ID1, ID2 = " << ID1 << ", " << ID2 <<
				". t_col1, t_col2 = " << t_col1 << ", " << t_col2 << std::endl;
			std::cout << "            i, i ID, i t_col " << i << ", " <<
				first_cols[i].ID << ", " << first_cols[i].t_col << std::endl;
		}*/
		if (t_col1 != -1) cols.emplace(Col(ID1, i, t_col1));
		if (t_col2 != -1) cols.emplace(Col(ID2, i, t_col2));
	}
	/*
	std::cout << "UPDATE: " << ID1 << " first col with " <<
		first_cols[ID1].ID << " at time " << first_cols[ID1].t_col << std::endl;
	std::cout << "        " << ID2 << " first col with " <<
		first_cols[ID2].ID << " at time " << first_cols[ID2].t_col << std::endl;
		*/
	return;
}

//returns -1 if Disks don't collide or are tangential, otherwise time of first collision
// input is ID of colliding objects (can be disk to wall).
// t_used is how much time has passed in the frame
double Disks::col_time(int ID1, int ID2, double t_used){
	if (ID1 > ID2){
		int temp = ID1;
		ID1 = ID2;
		ID2 = temp;
	}
	// wall on wall makes no sense, so we return -1?
	if ((ID1 >= n) && (ID2 >= n)) return -1;
	if ((ID1 < n) && (ID2 < n)){
		Disk a = disks[ID1];
		Disk b = disks[ID2];
		
		return col_time_disk(a, b, t_used);
	}
	Disk a = disks[ID1];
	double r = a.get_r();
	double cx = a.get_c().first;
	double cy = a.get_c().second;
	double vx = a.get_v().first;
	double vy = a.get_v().second;
	
	double ans = -1;
	if ((ID2 == n) && (vx < 0)) ans = (r-cx)/vx;
	else if ((ID2 == n+1) && (vy < 0)) ans = (r-cy)/vy;
	else if ((ID2 == n+2) && (vx > 0)) ans = (SCREEN_WIDTH-r-cx)/vx;
	else if ((ID2 == n+3) && (vy > 0)) ans = (SCREEN_HEIGHT-r-cy)/vy;
	
	return (ans >= 0) ? ans+t_used+TIME : -1;
}


double Disks::col_time_disk(Disk &a, Disk &b, double t_used){
	vec dc = sub(a.get_c(), b.get_c());
	vec dv = sub(a.get_v(), b.get_v());
	double ar = a.get_r();
	double br = b.get_r();
	
	double cdotv = dot(dc, dv);
	double cdistsq = dot(dc, dc);
	double vdistsq = dot(dv, dv);
	double rsumsq = (ar+br)*(ar+br);
	
	double insqrt = cdotv*cdotv-vdistsq*(cdistsq-rsumsq);
	if(insqrt <= 0 || cdistsq < rsumsq || vdistsq == 0) return -1;
	double ans = (-sqrt(insqrt)-cdotv)/vdistsq;
	if (ans < 0) return -1;
	return ans + t_used + TIME;
}


void Disks::move(SDL_Surface *surface, double t){
	for(int i = 0; i < n; i++){
		disks[i].move(surface, t);
	}
}

//Vector operations below

double Disks::dot(vec a, vec b){
	return a.first*b.first+a.second*b.second;
}
vec Disks::add(vec a, vec b){
	vec result(a.first+b.first, a.second+b.second);
	return result;
}
vec Disks::sub(vec a, vec b){
	vec result(a.first-b.first, a.second-b.second);
	return result;
}

vec Disks::mult(double a, vec b){
	vec result(a*b.first, a*b.second);
	return result;
}