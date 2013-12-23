#ifndef HALL_H_
#define HALL_H_

#include "include/Angel.h"

using namespace std;
using namespace Angel;

class hole {
private:
	static GLuint pts;
	static const int num_segments = 50;
	static const float radius = 0.4;
	static vec3 points[];
	static void fill();
public:
	static GLuint program;
	static void init(GLuint prog); // called once before drawing all the holes
	mat4 translation, rotation;
	int i, j;
	hole();
	virtual ~hole();
	void draw();
};

#endif
