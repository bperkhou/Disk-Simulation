#include "all_disks.h"
#include <iostream>
#include <time.h>
#include <map>
#include <stdlib.h>
#include <math.h>



//initialize centers first, then pick random radii that don't cause intersections
Disks::Disks(int num, Uint32 pixel){
	n = num;
	srand(time(NULL));
	disks = new Disk[n];
	for(int i = 0; i < n; i++){
		disks[i].set_color(pixel);
		disks[i].set_c(vec((float) (rand() % SCREEN_WIDTH), (float) (rand() % SCREEN_HEIGHT)));
		disks[i].set_v(vec(((float) rand() / (float) RAND_MAX)*(vmax-vmin)+vmin, ((float) rand() / (float) RAND_MAX)*(vmax-vmin)+vmin));
		disks[i].set_r(0);
	}
	for(int i = 0; i < n; i++){
	vec c = disks[i].get_c();
	float rmax = std::min(std::min(c.first, c.second), std::min(SCREEN_WIDTH-c.first, SCREEN_HEIGHT-c.second));
		for(int j = 0; j < n; j++){
			if (i == j) continue;
			vec dc = sub(disks[i].get_c(), disks[j].get_c());
			float dist = sqrt(dot(dc, dc));
			rmax = std::min(rmax, dist-disks[j].get_r());
		}
	disks[i].set_r(((float) rand() / (float) RAND_MAX)*(rmax-rmin)+rmin);
	}
}


Disks::~Disks(){
	delete [] disks;
}

void Disks::render_all(SDL_Surface *surface){
	for(int i = 0; i < n; i++){
		disks[i].render(surface);
	}
}

void Disks::update(SDL_Surface *surface, float t){
	float t_remain = resolve_collisions(surface, t);
	//t==t_remain precisely when no more collisions in frame
	while (t != t_remain){
		t = t_remain;
		t_remain = resolve_collisions(surface, t);
	}
	move(surface, t_remain);
}

//returns time remaining in current frame after resolving collisions
float Disks::resolve_collisions(SDL_Surface *surface, float t){
	//std::map<float, std::pair<int, int> > collisions;
	
	//when n=2, the below code works
	
	float ctime = col_time(disks[0], disks[1]);
	if ((ctime < 0) || (ctime > t)) return t;
	move(surface, ctime);
	col_update(disks[0], disks[1]);
	return t-ctime;
	
	
	/*for(int i = 0; i < n; i++){
		for(int j = 0; j < i; j++){
			ctime = col_time(disks[i], disks[j]);
			if(0 <= ctime <= t) collisions.insert(std::pair <float, std::pair<int, int> >(ctime, std::pair <int, int>(i, j)));
		}
	}*/
}

void Disks::col_update(Disk &a, Disk &b){
	vec dc = sub(a.get_c(), b.get_c());
	vec dv = sub(a.get_v(), b.get_v());

	float ar = a.get_r();
	float br = b.get_r();

	float distsq = dot(dc, dc);
	float K = dot(dc, dv);

	K = K / distsq;
	float ma = ar*ar;
	float mb = br*br;
	float mbovertot = mb / (ma + mb);
	float maovertot = ma / (ma + mb);
	a.set_v(sub(a.get_v(),mult(2*mbovertot*K,dc)));
	b.set_v(add(b.get_v(),mult(2*maovertot*K,dc)));
}

//returns -1 if Disks don't collide or are tangential, otherwise time of first collision
float Disks::col_time(Disk &a, Disk &b){
	vec dc = sub(a.get_c(), b.get_c());
	vec dv = sub(a.get_v(), b.get_v());
	float ar = a.get_r();
	float br = b.get_r();
	
	float cdotv = dot(dc, dv);
	float cdistsq = dot(dc, dc);
	float vdistsq = dot(dv, dv);
	float rsumsq = (ar+br)*(ar+br);
	
	float insqrt = cdotv*cdotv-vdistsq*(cdistsq-rsumsq);
	if(insqrt <= 0 || cdistsq < rsumsq || vdistsq == 0) return -1;
	float ans = (-sqrt(insqrt)-cdotv)/vdistsq;
	return ans;
}


void Disks::move(SDL_Surface *surface, float t){
	for(int i = 0; i < n; i++){
		disks[i].move(surface, t);
	}
}

//Vector operations below

float Disks::dot(vec a, vec b){
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

vec Disks::mult(float a, vec b){
	vec result(a*b.first, a*b.second);
	return result;
}