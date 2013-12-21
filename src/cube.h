/*
 * cube.h
 *
 *  Created on: Nov 25, 2013
 *      Author: Ashraf Saleh
 */

#ifndef CUBE_H_
#define CUBE_H_

//#include <GL/glew.h>
//#include <GL/glut.h>
#include "include/Angel.h"
#include "wood_texture.c"

using namespace std;
using namespace Angel;

class cube {

private:
	static GLfloat side_vertices[];
	static GLfloat side_colors[];
	static GLfloat tex_coords[];
	static GLushort base_elements[];

public:
	mat4 translation, rotation;
	GLuint pos_vbo, col_vbo, base_elements_ibo, tex_coord, texture_id;
	cube();
	cube(GLuint program, int offset_index);
	virtual ~cube();
	void draw();
};

#endif /* CUBE_H_ */
