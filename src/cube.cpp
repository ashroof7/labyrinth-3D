#include "cube.h"
#include <iostream>
using namespace std;

GLfloat cube::side_vertices[] = {
//                                // front base cube
//                                -0.5, -0.5,  0.5,
//                                 0.5, -0.5,  0.5,
//                                 0.5,  0.5,  0.5,
//                                -0.5,  0.5,  0.5,
//                                // back base cube
//                                -0.5, -0.5, -0.5,
//                                 0.5, -0.5, -0.5,
//                                 0.5,  0.5, -0.5,
//                                -0.5,  0.5, -0.5,
                // starting the side vertices
                -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5,
                0.5, -0.5, -0.5, -0.5, -0.5,

                -0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5,
                0.5, 0.5, -0.5, -0.5, 0.5,

                -0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5,
                -0.5, -0.5, 0.5, -0.5, 0.5, 0.5,

                0.5, 0.5, 0.5, 0.5, 0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5,
                -0.5, 0.5, 0.5, 0.5, 0.5,

                -0.5, -0.5, -0.5, 0.5, -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, -0.5,
                -0.5, 0.5, -0.5, -0.5, -0.5,

                -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -0.5,
                0.5, 0.5, -0.5, 0.5, -0.5,

};

GLfloat cube::side_colors[] = {
//                                // front base cube
//                                1.0, 1.0, 1.0, 0.0,
//                                1.0, 1.0, 1.0, 0.0,
//                                1.0, 1.0, 1.0, 0.0,
//                                1.0, 1.0, 1.0, 0.0,
//                                // back base cube
//                                1.0, 1.0, 1.0, 0.0,
//                                1.0, 1.0, 1.0, 0.0,
//                                1.0, 1.0, 1.0, 0.0,
//                                1.0, 1.0, 1.0, 0.0,

//                                // any mixture for debugging
//                                // front base cube
//                                0.0, 1.0, 0.0,  //green
//                                1.0, 1.0, 0.0,  //yellow
//                                1.0, 0.5, 0.0, //orange
//                                1.0, 0.0, 0.0,  //red
//                                // back base cube
//                                0.0, 1.0, 0.0,  //green
//                                0.0, 0.0, 1.0,  //blue
//                                1.0, 0.5, 0.0, //orange
//                                0.0, 0.0, 0.0, //black

                1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0,
                1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0,

                1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0,
                1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0,

                1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0,
                1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0,

                1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0,
                1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0,

                1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0,
                1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0,

                1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0,
                1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0,

};

GLfloat cube::tex_coords[] = {
// front base cube
                1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,

                1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,

                1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,

                // back base cube
                1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,

                1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,

                1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0,

};

GLuint  cube::vao = 0,
		cube::pos_vbo=0,
		cube::col_vbo=0,
		cube::tex_coord=0,
		cube::texture_id=0;

void cube::init(GLuint program) {
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

        //  vertices
        glGenBuffers(1, &pos_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(side_vertices), side_vertices,
                        GL_STATIC_DRAW);

        GLint pos_attrib = glGetAttribLocation(program, "vPosition");
        glEnableVertexAttribArray(pos_attrib);
        glVertexAttribPointer(pos_attrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

        //  colors
        glGenBuffers(1, &col_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, col_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(side_colors), side_colors,
                        GL_STATIC_DRAW);

        // based on the shader variable name
        GLint col_attrib = glGetAttribLocation(program, "gl_Color");
        glEnableVertexAttribArray(col_attrib);
        glVertexAttribPointer(col_attrib, 4, GL_FLOAT, GL_FALSE, 0, 0);

        // textures
        glGenBuffers(1, &tex_coord);
        glBindBuffer(GL_ARRAY_BUFFER, tex_coord);
        glBufferData(GL_ARRAY_BUFFER, sizeof(tex_coords), tex_coords,
                        GL_STATIC_DRAW);

        GLint tex_attrib = glGetAttribLocation(program, "in_texcoord");
        glEnableVertexAttribArray(tex_attrib);
        glVertexAttribPointer(tex_attrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glGenTextures(1, &texture_id);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, wood_image.width, wood_image.height,
                        0, GL_RGB, GL_UNSIGNED_BYTE, wood_image.pixel_data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glBindVertexArray(vao);
}

cube::cube() {
        translation = mat4(1.0);
        rotation = mat4(1.0);
}


cube::~cube() {
        glDeleteBuffers(1, &pos_vbo);
        glDeleteBuffers(1, &col_vbo);
        glDeleteTextures(1, &texture_id);
}

void cube::draw() {
		glEnableVertexAttribArray(vao);
		glBindVertexArray(vao);

		// draw base using buffer element
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);
        glDisableVertexAttribArray(vao);

}
