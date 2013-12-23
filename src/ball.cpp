#include "ball.h"
#include <iostream>
using namespace std;

GLfloat ball::ball_color[] = { 0.5, 0.5, 0, 1 };
GLuint ball::vao = 0;

ball::ball(GLuint program, float rad, int ii, int jj) {
	radius = rad;
	i = ii;
	j = jj;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//color
	GLuint color;
	glGenBuffers(1, &color);
	glBindBuffer(GL_ARRAY_BUFFER, color);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ball_color), ball_color,
			GL_STATIC_DRAW);

	GLint col_attrib = glGetAttribLocation(program, "gl_Color");
	glEnableVertexAttribArray(col_attrib);
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(vao);
}

ball::~ball() {
}

void ball::draw() {

	glEnableVertexAttribArray(vao);
	glBindVertexArray(vao);
	glutSolidSphere(radius, 50, 50);
	glBindVertexArray(0);
	glDisableVertexAttribArray(vao);


}
