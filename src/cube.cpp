/*
 * cube.cpp
 *
 *  Created on: Nov 25, 2013
 *      Author: leo
 */

#include "cube.h"
#include <iostream>
using namespace std;

GLfloat cube::side_vertices[] = {
// front base cube
		-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0,
		// back base cube
		-1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, };

GLfloat cube::side_colors[] = {
//		// front base cube
//		0.0, 0.0, 0.0,
//		0.0, 0.0, 0.0,
//		0.0, 0.0, 0.0,
//		0.0, 0.0, 0.0,
//		// back base cube
//		0.0, 0.0, 0.0,
//		0.0, 0.0, 0.0,
//		0.0, 0.0, 0.0,
//		0.0, 0.0, 0.0,

// any mixture for debugging
		// front base cube
		0.0, 1.0, 0.0,  //green
		1.0, 1.0, 0.0,  //yellow
		1.0, 0.5, 0.0, //orange
		1.0, 0.0, 0.0,  //red
		// back base cube
		0.0, 1.0, 0.0,  //green
		0.0, 0.0, 1.0,  //blue
		1.0, 0.5, 0.0, //orange
		0.0, 0.0, 0.0, //black
		};

GLushort cube::base_elements[] = {
// front
		0, 1, 2, 2, 3, 0,
		// top
		3, 2, 6, 6, 7, 3,
		// back
		7, 6, 5, 5, 4, 7,
		// bottom
		4, 5, 1, 1, 0, 4,
		// left
		4, 0, 3, 3, 7, 4,
		// right
		1, 5, 6, 6, 2, 1, };

cube::cube(GLuint program, int offset_index) {
	translation = mat4(1.0);
	rotation = mat4(1.0);

	// base cube elements
	glGenBuffers(1, &base_elements_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, base_elements_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(base_elements), base_elements,
			GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// side vertices
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(side_vertices), side_vertices,
			GL_STATIC_DRAW);

	// based on the shader variable name
	GLint pos_attrib = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(pos_attrib);
	glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// side colors
	glGenBuffers(1, &col_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(side_colors), side_colors,
			GL_STATIC_DRAW);

	// based on the shader variable name
	GLint col_attrib = glGetAttribLocation(program, "in_Color");
	glEnableVertexAttribArray(col_attrib);
	glVertexAttribPointer(col_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// textures
	texture_id = -1;
//	glGenTextures(1, &texture_id);
//	glBindTexture(GL_TEXTURE_2D, texture_id);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//
//	glTexImage2D(GL_TEXTURE_2D, // target
//		       0,  // level, 0 = base, no minimap,
//		       GL_RGB, // internalformat
//		       wood_texture.width,  // width
//		       wood_texture.height,  // height
//		       0,  // border, always 0 in OpenGL ES
//		       GL_RGB,  // format
//		       GL_UNSIGNED_BYTE, // type
//		       wood_texture.pixel_data);
}

cube::cube() {
	col_vbo = pos_vbo = base_elements_ibo = texture_id = -1;
}

cube::~cube() {
	glDeleteBuffers(1, &pos_vbo);
	glDeleteBuffers(1, &col_vbo);
	glDeleteBuffers(1, &base_elements_ibo);
//	glDeleteTextures(1, &texture_id);
}

void cube::draw() {
	//	 draw base using buffer element
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, base_elements_ibo);
	GLint size;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	glDrawElements(GL_TRIANGLES, size / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, texture_id);
//	uniform_wood_texture = glGetUniformLocation(program, "wood_texture");
//	glUniform1i(uniform_wood_texture, /*GL_TEXTURE*/0);
}
