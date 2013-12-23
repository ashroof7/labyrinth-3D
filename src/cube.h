#ifndef CUBE_H_
#define CUBE_H_

#include <GL/glew.h>
#include <GL/glut.h>
#include "include/Angel.h"
#include "wood_texture.c"

using namespace std;
using namespace Angel;

class cube {

private:
        static GLfloat side_vertices[];
        static GLfloat side_colors[];
        static GLfloat tex_coords[];
        static GLuint vao, pos_vbo, col_vbo, tex_coord, texture_id;

public:
        static void init(GLuint program); // called once before drawing all the cubes
        mat4 translation, rotation;
        cube();
        virtual ~cube();
        void draw();
};

#endif /* CUBE_H_ */
