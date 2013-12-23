#ifndef HOLE_H_
#define HOLE_H_

#include "include/Angel.h"

using namespace std;
using namespace Angel;

class hole {

private:
	static GLuint pts, vao, col_vbo;
	static const int num_segments = 50;
	static const float radius = 0.4;
	static vec3 points[];
	static vec4 colors[];
	static void fill();

public:
	static void init(GLuint program); // called once before drawing all the holes
	mat4 translation, rotation;
	hole();
	virtual ~hole();
	void draw();
};

#endif /* HOLE_H_ */
