#include "hole.h"
#include <iostream>
using namespace std;
GLuint hole::program = 0, hole::pts = 0;
vec3 hole::points[num_segments];
void hole::init(GLuint prog) {
	fill();
	program = prog;
	//color
	glGenBuffers(1, &pts);
	glBindBuffer(GL_ARRAY_BUFFER, pts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
	// based on the shader variable name
	GLint pos_attrib = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pos_attrib);
	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);
}
void hole::fill() {
	float theta = 2 * 3.1415926 / float(num_segments);
	float tangetial_factor = tanf(theta); //calculate the tangential factor

	float radial_factor = cosf(theta); //calculate the radial factor

	float x = radius; //we start at angle = 0

	float y = 0;
	int index = 0;
	for (int ii = 0; ii < num_segments; ii++) {
		points[index++] = vec3(x, -0.5, y);
		//calculate the tangential vector
		//remember, the radial vector is (x, y)
		//to get the tangential vector we flip those coordinates and negate one of them
		float tx = -y;
		float ty = x;
		//add the tangential vector
		x += tx * tangetial_factor;
		y += ty * tangetial_factor;
		//correct using the radial factor
		x *= radial_factor;
		y *= radial_factor;
	}
}
hole::hole() {
	translation = mat4(1.0);
	rotation = mat4(1.0);
}

hole::~hole() {
}

void hole::draw() {
	glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments);
}
