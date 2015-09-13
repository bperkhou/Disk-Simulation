#include "all_disks.h"
#include <iostream>
#include <time.h>
#include <map>


Disks::Disks(int num, Uint32 pixel){
	n = num;
	srand(time(NULL));
	circles = new Circle[n];
	for(int i = 0; i < n; i++){
		circles[i].set_color(pixel);
		circles[i].set_c(std::make_pair ((float) (rand() % SCREEN_WIDTH), (float) (rand() % SCREEN_HEIGHT)));
		circles[i].set_v(std::make_pair (((float) rand() / (float) RAND_MAX)*vmax, ((float) rand() / (float) RAND_MAX)*vmax));
		circles[i].set_r(((float) rand() / (float) RAND_MAX)*(rmax - rmin)+rmin);
	}
}


Disks::~Disks(){
	delete [] circles;
}

void Disks::render_all(SDL_Surface *surface){
	for(int i = 0; i < n; i++){
		circles[i].render(surface);
	}
}

void Disks::update(SDL_Surface *surface, float t){
	t = resolve_collisions(surface, t);
	move(surface, t);
}

//returns time remaining in current frame after resolving collisions
float Disks::resolve_collisions(SDL_Surface *surface, float t){
	//std::map<float, std::pair<int, int> > collisions;
	float ctime = col_time(&circles[0], &circles[1]);
	if ((ctime < 0) || (ctime > t)) return t;
	move(surface, ctime);
	col_update(&circles[0], &circles[1]);
	return t-ctime;
	
	/*for(int i = 0; i < n; i++){
		for(int j = 0; j < i; j++){
			ctime = col_time(&circles[i], &circles[j]);
			if(0 <= ctime <= t) collisions.insert(std::pair <float, std::pair<int, int> >(ctime, std::pair <int, int>(i, j)));
		}
	}*/
}

void Disks::col_update(Circle *a, Circle *b){
	std::pair <float, float> dc = sub(a->get_c(), b->get_c());
	std::pair <float, float> dv = sub(a->get_v(), b->get_v());

	float ar = a->get_r();
	float br = b->get_r();

	float distsq = dot(dc, dc);
	float K = dot(dc, dv);

	K = K / distsq;
	float ma = ar*ar;
	float mb = br*br;
	float mbovertot = mb / (ma + mb);
	float maovertot = ma / (ma + mb);
	a->set_v(sub(a->get_v(),mult(2*mbovertot*K,dc)));
	b->set_v(add(b->get_v(),mult(2*maovertot*K,dc)));
}

//returns -1 if circles don't collide or are tangential, otherwise time of first collision
float Disks::col_time(Circle* a, Circle* b){
	std::pair <float, float> dc = sub(a->get_c(), b->get_c());
	std::pair <float, float> dv = sub(a->get_v(), b->get_v());
	float ar = a->get_r();
	float br = b->get_r();
	
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
		circles[i].move(surface, t);
	}
}

//Vector operations below

float Disks::dot(std::pair <float, float> a, std::pair <float, float> b){
	return a.first*b.first+a.second*b.second;
}
std::pair <float, float> Disks::add(std::pair <float, float> a, std::pair <float, float> b){
	std::pair <float, float> result(a.first+b.first, a.second+b.second);
	return result;
}
std::pair <float, float> Disks::sub(std::pair <float, float> a, std::pair <float, float> b){
	std::pair <float, float> result(a.first-b.first, a.second-b.second);
	return result;
}

std::pair <float, float> Disks::mult(float a, std::pair <float, float> b){
	std::pair <float, float> result(a*b.first, a*b.second);
	return result;
}