#include "hole.h"
#include <iostream>

using namespace std;

vec3 hole::points[num_segments];
vec4 hole::colors[num_segments];

GLuint 	hole::vao = 0,
		hole::pts = 0,
		hole::col_vbo = 0;

void hole::init(GLuint program) {

	fill();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &pts);
	glBindBuffer(GL_ARRAY_BUFFER, pts);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	GLint pos_attrib = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pos_attrib);
	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);


	// colors
	glGenBuffers(1, &col_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLint col_attrib = glGetAttribLocation(program, "gl_Color");
	glEnableVertexAttribArray(col_attrib);
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
}

void hole::fill() {
	float theta = 2 * 3.1415926 / float(num_segments);
	float tangetial_factor = tanf(theta); //calculate the tangential factor

	float radial_factor = cosf(theta); //calculate the radial factor

	float x = radius; //we start at angle = 0

	float y = 0;
	int index = 0;
	for (int ii = 0; ii < num_segments; ii++) {
		colors[index]   = vec4(0.0f, 0.0f, 0.0f, 1.0f);
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
	glDeleteBuffers(1, &pts);
}

void hole::draw() {
	glEnableVertexAttribArray(vao);
	glBindVertexArray(vao);

	glDrawArrays(GL_TRIANGLE_FAN, 0, num_segments);

	glBindVertexArray(0);
	glDisableVertexAttribArray(vao);

}
