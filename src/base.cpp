#include "base.h"
#include <iostream>
using namespace std;


GLfloat base::colors[] = {
		1.0, 1.0, 1.0, 0.0,
		1.0, 1.0, 1.0, 0.0,
		1.0, 1.0, 1.0, 0.0,
		1.0, 1.0, 1.0, 0.0,
};

GLfloat base::tex_coords[] ={
				1.0, 1.0,
				1.0, 0.0,
				0.0, 0.0,
				0.0, 1.0,
};

base::base(GLuint program, GLuint offset_index, const vec3 upper_left, const vec3 lower_right) {
	// construct rectangle vertices
	vertices[0] = upper_left.x;
	vertices[1] = upper_left.y;
	vertices[2] = upper_left.z;

	vertices[3] = upper_left.x;
	vertices[4] = upper_left.y;
	vertices[5] = lower_right.z;

	vertices[6] = lower_right.x;
	vertices[7] = upper_left.y;
	vertices[8] = upper_left.z;

	vertices[9] = lower_right.x;
	vertices[10] = upper_left.y;
	vertices[11] = lower_right.z;


	translation = mat4(1.0);
	rotation = mat4(1.0);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//vertices
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// based on the shader variable name
	GLint pos_attrib = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pos_attrib);
	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// colors
	glGenBuffers(1, &col_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	GLint col_attrib = glGetAttribLocation(program, "in_Color");
	glEnableVertexAttribArray(col_attrib);
	glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// textures
	glGenBuffers(1, &tex_coord);
	glBindBuffer(GL_ARRAY_BUFFER, tex_coord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords,GL_STATIC_DRAW);

	GLint tex_attrib = glGetAttribLocation(program, "in_texcoord");
	glEnableVertexAttribArray(tex_attrib);
	glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wood_image.width, wood_image.height,
			0, GL_RGB, GL_UNSIGNED_BYTE, wood_image.pixel_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);


}

base::base() {
	vao = col_vbo = pos_vbo  = texture_id = tex_coord = -1;
}

base::~base() {
	glDeleteBuffers(1, &pos_vbo);
	glDeleteBuffers(1, &col_vbo);
	glDeleteTextures(1, &texture_id);
}

void base::draw() {
	glEnableVertexAttribArray(vao);
	glBindVertexArray(vao);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glDrawArrays(GL_TRIANGLE_STRIP, 0,4);

	glBindVertexArray(0);
	glDisableVertexAttribArray(vao);
}
