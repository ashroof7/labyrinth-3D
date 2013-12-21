#include "ball.h"
#include <iostream>
using namespace std;

GLfloat ball::ball_color[] = { 1, 1, 0, 1 };

ball::ball(GLuint program, float rad, int ii, int jj) {
	radius = rad;
	i = ii;
	j = jj;

	//color
	GLuint color;
	glGenBuffers(1, &color);
	glBindBuffer(GL_ARRAY_BUFFER, color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ball_color), ball_color,
			GL_STATIC_DRAW);
	GLint col_attrib = glGetAttribLocation(program, "in_Color");
	glEnableVertexAttribArray(col_attrib);
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

ball::~ball() {
}

void ball::draw() {
	glutSolidSphere(radius, 50, 50);
}
