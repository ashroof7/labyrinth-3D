#include "include/Angel.h"
#include "cube.h"
#include "ball.h"
#include "base.h"
#include "hole.h"

#include <iostream>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <algorithm>

using namespace std;
using namespace Angel;

int TIMERMSECS = 10;

//mouse state variables
int last_mx = 0, last_my = 0, cur_mx = 0, cur_my = 0;
int arcball_on = false;

GLfloat screen_width = 600;
GLfloat screen_height = 600;

GLuint program;
GLuint cube_vao;
GLuint hole_vao;

GLuint P_loc, M_loc, V_loc, W_loc; // locations pointers of P, M, V, W matrices in shader
GLuint A_loc, D_loc, S_loc, Shine_loc;
GLuint mask_loc;
GLuint shape_type_loc;

mat4 I_mat = mat4(1.0); // Identity matrix
mat4 M_mat = mat4(1.0); // Model matrix
mat4 V_mat = mat4(1.0); // View matrix
mat4 W_mat = mat4(1.0); // World matrix (rotation of cube by mouse)
mat4 P_mat = mat4(1.0); // projection matrix


const int lvl_width = 7;
const int lvl_height = 7;

GLfloat cube_width = 1.0;
GLfloat cube_height = 1.0;
GLfloat cube_depth = 1.0;
GLuint uniform_tex_sampler;

vec3 eye(-0.5, 6, 4);

float xangle = 0;
float zangle = 0;

char map[7][7] = { { '#', '#', '#', '#', '#', '#', '#' }, { '#', '.', '.', '#',
		'.', '.', '#' }, { '#', '.', 'O', '#', '.', '.', '#' }, { '#', '.',
		'#', '#', '#', '#', '#' }, { '#', '.', '.', '#', '.', '.', '#' }, {
		'#', '#', '.', '.', 'O', '.', '#' }, { '#', '#', '#', '#', '#', '#',
		'#' } };

cube * walls[lvl_height][lvl_width];
hole * holes[lvl_height][lvl_width];
ball * _ball;
base * _base;
//==================
//openGL functions
//==================


void build_lvl() {
	// notice that everything is shifted by cube_side/2 to ease calculations
	// Handled by centering the camera @ cube_side/2


	// building base
	vec3 upper_left = vec3(-((lvl_width + 1) * cube_width / 2.0), -cube_depth
			/ 2.0 - 0.001, -((lvl_height + 1) * cube_height / 2.0));
	vec3 lower_right = vec3((lvl_width - 1) * cube_width / 2.0, -cube_depth
			/ 2.0 - 0.001, (lvl_height - 1) * cube_height / 2.0);
	_base = new base(program, 0, upper_left, lower_right);

	// building walls and holes
	for (int i = 0; i < lvl_height; ++i) {
		for (int j = 0; j < lvl_width; ++j) {
			if (map[i][j] == '#') { //wall
				walls[i][j] = new cube();
				walls[i][j]->translation = Translate((-1.0 * lvl_width / 2 + j)
						* cube_width, 0, (-1.0 * lvl_height / 2 + i)
						* cube_height);
			} else if (map[i][j] == 'O') { //hole
				holes[i][j] = new hole();
				holes[i][j]->translation = Translate((-1.0 * lvl_width / 2 + j)
						* cube_width, 0, (-1.0 * lvl_height / 2 + i)
						* cube_height);
			}
		}
	}

	//building ball
	_ball = new ball(program, 0.3, 1, 1);
	_ball->translation = Translate((-1.0 * lvl_width / 2 + _ball->i)
			* cube_width, 0, (-1.0 * lvl_height / 2 + _ball->j) * cube_height);
}

void init_objects() {
	cube::init(program);
	hole::init(program);
}

void init(void) {

	// Load shaders and use the resulting shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);
	build_lvl();
	init_objects();

	V_mat = LookAt(eye, vec3(-0.5, 0, 0), vec3(0, 1, 0));
	P_mat = Perspective(45.0f, 1.0f * screen_width / screen_height, 1.0f,
			100.0f);

	// matrices location in shader
	P_loc = glGetUniformLocation(program, "P");
	V_loc = glGetUniformLocation(program, "V");
	M_loc = glGetUniformLocation(program, "M");
	W_loc = glGetUniformLocation(program, "W");
	A_loc = glGetUniformLocation(program, "AmbientProduct");
	D_loc = glGetUniformLocation(program, "DiffuseProduct");
	S_loc = glGetUniformLocation(program, "SpecularProduct");
	Shine_loc = glGetUniformLocation(program, "Shininess");
	mask_loc = glGetUniformLocation(program, "mask");
	shape_type_loc = glGetUniformLocation(program, "shapeType");

	glUniformMatrix4fv(P_loc, 1, GL_TRUE, P_mat);
	glUniformMatrix4fv(V_loc, 1, GL_TRUE, V_mat);
	glUniformMatrix4fv(M_loc, 1, GL_TRUE, M_mat);
	glUniformMatrix4fv(W_loc, 1, GL_TRUE, W_mat);

	glClearColor(0.0, 0.0, 0.0, 1.0); // black background

	glEnable(GL_DEPTH_TEST); // Enable depth test
	glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one

	uniform_tex_sampler = glGetUniformLocation(program, "tex_sampler");
	glUniform1i(uniform_tex_sampler, /*GL_TEXTURE*/0);

}

void bufferBeforeDrawCube() {
	vec4 ambient = vec4(0.9, 0.9, 0.0, 1.0);
	vec4 diffuse = vec4(0.6, 0.6, 0.0, 0.0);
	vec4 specular = vec4(0.0, 0.0, 0.0, 0.0);
	GLfloat my_shine = 1000;
	GLint my_shape_type = 1;
	glUniform1i(shape_type_loc, my_shape_type);
	glUniform1f(Shine_loc, my_shine);
	glUniform4fv(A_loc, 1, (GLfloat*) &ambient);
	glUniform4fv(D_loc, 1, (GLfloat*) &diffuse);
	glUniform4fv(S_loc, 1, (GLfloat*) &specular);
}
void bufferBeforeDrawBall() {
	vec4 ambient = vec4(0.7, 0.7, 0.7, 0.5);
	vec4 diffuse = vec4(0.7, 0.7, 0.7, 1.0);
	vec4 specular = vec4(1.0, 1.0, 1.0, 0.0);
	GLfloat my_shine = 500;
	GLint my_shape_type = 0;
	glUniform1i(shape_type_loc, my_shape_type);
	glUniform1f(Shine_loc, my_shine);
	glUniform4fv(A_loc, 1, (GLfloat*) &ambient);
	glUniform4fv(D_loc, 1, (GLfloat*) &diffuse);
	glUniform4fv(S_loc, 1, (GLfloat*) &specular);
}
void bufferBeforeDrawHole() {

}
void bufferBeforeDrawSufrace() {
	vec4 ambient = vec4(0.7, 0.7, 0.7, 0.5);
	vec4 diffuse = vec4(0.7, 0.7, 0.7, 1.0);
	vec4 specular = vec4(1.0, 1.0, 1.0, 0.0);
	GLfloat my_shine = 500;
	GLint my_shape_type = 2;
	glUniform1i(shape_type_loc, my_shape_type);
	glUniform1f(Shine_loc, my_shine);
	glUniform4fv(A_loc, 1, (GLfloat*) &ambient);
	glUniform4fv(D_loc, 1, (GLfloat*) &diffuse);
	glUniform4fv(S_loc, 1, (GLfloat*) &specular);
}
int dx[] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
int dy[] = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window//

	bufferBeforeDrawSufrace();
	M_mat = _base->translation * _base->rotation;
	glUniformMatrix4fv(M_loc, 1, GL_TRUE, M_mat);
	_base->draw();

	bufferBeforeDrawCube();
	for (int i = 0; i < lvl_height; ++i) {
		for (int j = 0; j < lvl_width; ++j) {
			if (map[i][j] == '#') { //wall
				GLint mask = 0;
				for (int d = 0; d < 9; d++) {
					int ni = i + dx[d];
					int nj = j + dy[d];
					if (ni >= 0 && nj >= 0 && ni < lvl_height && nj < lvl_width
							&& map[ni][nj] == '#')
						mask |= 1 << d;
				}
				glUniform1i(mask_loc, mask);
				M_mat = walls[i][j]->translation * walls[i][j]->rotation;
				glUniformMatrix4fv(M_loc, 1, GL_TRUE, M_mat);
				walls[i][j]->draw();
			} else if (map[i][j] == '.') { // empty
				//nothing to do here :P
			}
		}
	}

	bufferBeforeDrawBall();

	M_mat = _ball->translation;
	glUniformMatrix4fv(M_loc, 1, GL_TRUE, M_mat);
	_ball->draw();

	glutSwapBuffers(); // Double buffering
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 033: //esc
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
		eye[1] += 0.5;
		glUniformMatrix4fv(V_loc, 1, GL_TRUE, LookAt(eye, vec3(0, 0, 0), vec3(
				0, 1, 0)));
		break;
	case GLUT_KEY_UP:
		eye[1] -= 0.5;
		glUniformMatrix4fv(V_loc, 1, GL_TRUE, LookAt(eye, vec3(0, 0, 0), vec3(
				0, 1, 0)));
		break;
	}
}

void onPassiveMotion(int x, int y) {
}

void onReshape(int width, int height) {
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
	P_mat = Perspective(45.0f, 1.0f * screen_width / screen_height, 1.0f,
			100.0f);
	glUniformMatrix4fv(P_loc, 1, GL_FALSE, P_mat);
}

vec3 get_arcball_vector(int x, int y) { // what is the purpose of this method?
	//        convert the x,y screen coordinates to [-1,1] coordinates (and reverse y coordinates)
	vec3 P = vec3(1.0 * x / screen_width * 2 - 1.0, 1.0 * y / screen_height * 2
			- 1.0, 0);
	P.y = -P.y;

	float OP_squared = P.x * P.x + P.y * P.y;
	// use Pythagorean theorem to get P.z
	if (OP_squared <= 1 * 1)
		P.z = sqrt(1 * 1 - OP_squared); // Pythagore
	else
		P = normalize(P); // nearest point

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
	if (arcball_on) { // if left button is pressed
		cur_mx = x;
		cur_my = y;

		// YES x and z are switched with y and x
		// don't ask :D
		if (cur_mx != last_mx || cur_my != last_my) {
			zangle += (last_mx - cur_mx) / 5.0;
			xangle += -(last_my - cur_my) / 5.0;
			if (abs(zangle) > 30)
				zangle -= (last_mx - cur_mx) / 3.0;
			if (abs(xangle) > 30)
				xangle += (last_my - cur_my) / 3.0;

			W_mat = RotateX(xangle) * RotateZ(zangle);
			last_mx = cur_mx;
			last_my = cur_my;

			glUniformMatrix4fv(W_loc, 1, GL_TRUE, W_mat);
			glutPostRedisplay();
		}
	}
}
float speedz = 0;
float speedx = 0;

#define magic 0.19

void animate(int n) {
	mat4 tmp = _ball->translation;
	vec4 pos = tmp * vec3(0, 0, 0);
	tmp *= Translate(speedz, 0, 0);
	pos = tmp * vec3(0, 0, 0);
	pos[0] += lvl_width / 2.0;
	pos[2] += lvl_height / 2.0;
	//        cout << speedx << " "<< speedz << endl;
	if (map[(int) ceil(pos[2] - magic)][(int) floor(pos[0] + magic)] == '#'
			|| map[(int) floor(pos[2] + magic)][(int) ceil(pos[0] - magic)]
					== '#' || map[(int) floor(pos[2] + magic)][(int) floor(
			pos[0] + magic)] == '#'
			|| map[(int) ceil(pos[2] - magic)][(int) ceil(pos[0] - magic)]
					== '#') {
		tmp = _ball->translation;
		speedz = 0;
	} else {
		speedz += -zangle / 5000.0;
	}
	mat4 tt = tmp;
	tmp *= Translate(0, 0, speedx);
	pos = tmp * vec3(0, 0, 0);
	pos[0] += lvl_height / 2.0;
	pos[2] += lvl_width / 2.0;
	if (map[(int) ceil(pos[2] - magic)][(int) floor(pos[0] + magic)] == '#'
			|| map[(int) floor(pos[2] + magic)][(int) ceil(pos[0] - magic)]
					== '#' || map[(int) floor(pos[2] + magic)][(int) floor(
			pos[0] + magic)] == '#'
			|| map[(int) ceil(pos[2] - magic)][(int) ceil(pos[0] - magic)]
					== '#') {
		tmp = tt;
		speedx = 0;
	} else {
		speedx += xangle / 5000.0;
	}
	_ball->translation = tmp;
	display();
	glutTimerFunc(TIMERMSECS, animate, 0);
}

//=========
//main loop
//=========

int main(int argc, char **argv) {

	glutInit(&argc, argv);
	glEnable(GL_DEPTH_TEST);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
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
	glutTimerFunc(TIMERMSECS, animate, 0);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutMouseFunc(onMouse);
	glutMotionFunc(onMotion);
	glutReshapeFunc(onReshape);

	glutMainLoop();
	return 0;
}

