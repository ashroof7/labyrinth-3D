/*
 * base.h
 *
 *  Created on: Dec 22, 2013
 *      Author: leo
 */

#ifndef BASE_H_
#define BASE_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include "include/Angel.h"
#include "wood_texture.c"

using namespace std;
using namespace Angel;

class base {

private:
	 GLfloat vertices[12];
	static GLfloat colors[];
	static GLfloat tex_coords[];

public:
	mat4 translation, rotation;
	GLuint pos_vbo, col_vbo, tex_coord, texture_id, vao;
	base();
	base(GLuint program, GLuint offset_index, const vec3 upper_left, const vec3 lower_right );
	virtual ~base();
	void draw();
};

#endif /* BASE_H_ */
