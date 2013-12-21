#include "utils.h"
#include "cube.h"

#include <time.h>
#include <iostream>
#include <algorithm>
#include <GL/glew.h>
#include <GL/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

//mouse state variables
int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;


GLfloat screen_width = 600;
GLfloat screen_height = 600;

GLuint program;
GLuint vao;

GLuint P_loc, M_loc, V_loc, W_loc; // locations pointers of P, M, V, W matrices in shader

mat4 I_mat = mat4(1.0); // Identity matrix
mat4 M_mat = mat4(1.0); // Model matrix
mat4 V_mat = mat4(1.0); // View matrix
mat4 W_mat = mat4(1.0); // World matrix (rotation of cube by mouse)
mat4 P_mat = mat4(1.0); // projection matrix

vec3 eye(2,10,-10);

const int lvl_width = 7;
const int lvl_height = 7;

GLuint cube_width = 1;
GLuint cube_height = 1;
GLuint uniform_tex_sampler;

char map[7][7] = {	{'#','#','#','#','#','#','#'},
					{'#','.','.','#','.','.','#'},
					{'#','#','.','#','.','.','#'},
					{'#','.','#','#','#','#','#'},
					{'#','.','.','#','.','.','#'},
					{'#','#','.','.','.','.','#'},
					{'#','#','#','#','#','#','#'}};


cube * walls[lvl_height][lvl_width];

//==================
//openGL functions
//==================

void build_lvl(){
	for (int i = 0; i < lvl_height; ++i) {
		for (int j = 0; j < lvl_width; ++j) {
			if (map[i][j]=='#') {//wall
				walls[i][j] = new cube(program, 0);
				walls[i][j]->translation = translate(vec3((-1.0*lvl_width/2 + j)*cube_width, 0, (-1.0*lvl_height/2 + i)*cube_height));
			}else if(map[i][j] == '.'){	// empty
				//nothing to do here :P
			}
		}
	}
}

void init_buffers() {
	// Initializing  VAOs and VBOs
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}


void init(void) {

	// Load shaders and use the resulting shader program
	program = init_shaders("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	init_buffers();
	build_lvl();

	V_mat = lookAt(eye, vec3(0,0,0), vec3(0,1,0));
	P_mat = perspective(45.0f, 1.0f*screen_width/screen_height, 1.0f, 100.0f);

	// matrices location in shader
	P_loc = glGetUniformLocation(program, "P");
	V_loc = glGetUniformLocation(program, "V");
	M_loc = glGetUniformLocation(program, "M");
	W_loc = glGetUniformLocation(program, "W");

	glUniformMatrix4fv(P_loc, 1, GL_FALSE, value_ptr(P_mat));
	glUniformMatrix4fv(V_loc, 1, GL_FALSE, value_ptr(V_mat));
	glUniformMatrix4fv(M_loc, 1, GL_FALSE, value_ptr(M_mat));
	glUniformMatrix4fv(W_loc, 1, GL_FALSE, value_ptr(W_mat));

	uniform_tex_sampler = glGetUniformLocation(program, "tex_sampler");
	glUniform1i(uniform_tex_sampler, /*GL_TEXTURE*/ 0);


	glClearColor(0.9, 0.9, 0.9, 1.0); // grey background :/ not sure about the color

	glEnable(GL_DEPTH_TEST);// Enable depth test
	glDepthFunc(GL_LESS);// Accept fragment if it closer to the camera than the former one

}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // clear the window//

	glEnableVertexAttribArray(vao);
	glBindVertexArray(vao);

	// update rotation and translation matrices of each object before uploading to shader
	for (int i = 0; i < lvl_height; ++i) {
			for (int j = 0; j < lvl_width; ++j) {
				if (map[i][j]=='#') {//wall
					M_mat = walls[i][j]->translation * walls[i][j]->rotation;
					glUniformMatrix4fv(M_loc, 1, GL_FALSE, value_ptr(M_mat));
					walls[i][j]->draw();
				}else if(map[i][j] =='.'){ // empty
					//nothing to do here :P
				}
			}
		}

	glDisableVertexAttribArray(vao);
	glutSwapBuffers(); // Double buffering
}


void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033:
		cout << "exit" << endl;
		exit(EXIT_SUCCESS);
		break;
	}
}

void keyboard_special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		break;
	case GLUT_KEY_LEFT:
		break;
	case GLUT_KEY_DOWN:
		break;
	case GLUT_KEY_UP:
		break;
	}
}

void onPassiveMotion(int x, int y) {
}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
	P_mat = perspective(45.0f, 1.0f*screen_width/screen_height, 1.0f, 100.0f);
	glUniformMatrix4fv(P_loc, 1, GL_FALSE, value_ptr(P_mat));
}


vec3 get_arcball_vector(int x, int y) {
	//	convert the x,y screen coordinates to [-1,1] coordinates (and reverse y coordinates)
	vec3 P = vec3(1.0*x/screen_width*2 - 1.0, 	1.0*y/screen_height*2 - 1.0, 	0);
	P.y = -P.y;

	float OP_squared = P.x * P.x + P.y * P.y;
	// use Pythagorean theorem to get P.z
	if (OP_squared <= 1*1)
		P.z = sqrt(1*1 - OP_squared);  // Pythagore
	else
		P = normalize(P);  // nearest point

	return P;
}


void onMouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		arcball_on = true;
		last_mx = cur_mx = x;
		last_my = cur_my = y;
	} else {
		arcball_on = false;
	}
}

void onMotion(int x, int y) {
	if (arcball_on) {  // if left button is pressed
		cur_mx = x;
		cur_my = y;

		if (cur_mx != last_mx || cur_my != last_my) {
			vec3 va = get_arcball_vector(last_mx, last_my);
			vec3 vb = get_arcball_vector( cur_mx,  cur_my);

			// dot(va,vb) = ||va||.||vb||.cos(angle)
			float angle = acos(  glm::min(1.0f, 1.0f*dot(va, vb)))*3;

			vec3 axis_in_camera_coord = cross(va, vb);
			mat3 camera2object = inverse(mat3(V_mat * W_mat));
			vec3 axis_in_obj_coord =  camera2object * axis_in_camera_coord;
			W_mat = rotate(W_mat, degrees(angle), axis_in_obj_coord);

			last_mx = cur_mx;
			last_my = cur_my;

			glUniformMatrix4fv(W_loc, 1, false, value_ptr(W_mat));
			glutPostRedisplay();
		}
	}
}


//=========
//main loop
//=========

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(screen_width, screen_height);

	//----------------------------------------
	// If you are using freeglut, the next two lines will check if
	// the code is truly 3.2. Otherwise, comment them out

	//glutInitContextVersion( 3, 2 );
	//glutInitContextProfile( GLUT_CORE_PROFILE );
	//----------------------------------------

	glutCreateWindow("Labyrinth :D");
	glewInit();
	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);
	glutReshapeFunc(onReshape);



	glutMainLoop();
	return 0;
}

