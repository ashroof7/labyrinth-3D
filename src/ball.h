#ifndef BALL_H_
#define BALL_H_

#include "include/Angel.h"

using namespace std;
using namespace Angel;

class ball {
private:
	static GLfloat ball_color[];
	static GLuint vao;

public:
	float radius;
	mat4 translation, rotation;
	int i, j;
	ball(GLuint program,float rad, int ii, int jj);
	virtual ~ball();
	void draw();
};

#endif
