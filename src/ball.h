#ifndef BALL_H_
#define BALL_H_

#include "include/Angel.h"

using namespace std;
using namespace Angel;

class ball {
private:
	float radius;
	static GLfloat ball_color[];
	static GLuint vao;

public:
	mat4 translation, rotation;
	int i, j;
	ball(GLuint program,float rad, int ii, int jj);
	virtual ~ball();
	void draw();
};

#endif
