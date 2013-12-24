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
#include <fstream>
#include <string>

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

int lvl_width = 7;
int lvl_height = 7;
bool falling = 0;
bool win = 0;

int level = 1;

GLfloat cube_width = 1.0;
GLfloat cube_height = 1.0;
GLfloat cube_depth = 1.0;
GLuint uniform_tex_sampler;

vec3 eye(-0.5, 10, 4);
vec3 coi(-0.5, 0, 0);
vec3 up(0, 1, 0);

float xangle = 0;
float zangle = 0;

const int MAX_LVL_WIDTH = 50;
const int MAX_LVL_HEIGHT = 50;
char map[MAX_LVL_HEIGHT][MAX_LVL_WIDTH];

cube * walls[MAX_LVL_HEIGHT][MAX_LVL_WIDTH];
hole * holes[MAX_LVL_HEIGHT][MAX_LVL_WIDTH];
ball * _ball;
base * _base;
//==================
//openGL functions
//==================

void load_level(string filename) {
	ifstream file;
	file.open(filename.c_str());
	file >> lvl_width >> lvl_height;

	for (int i = 0; i < lvl_height; ++i) {
		file >> map[i];
		cout << map[i] << endl;
	}
	file.close();
}

void build_lvl() {
	// notice that everything is shifted by cube_side/2 to ease calculations
	// Handled by centering the camera @ cube_side/2

	// building base
	vec3 upper_left = vec3(-((lvl_width + 1) * cube_width / 2.0),
			-cube_depth / 2.0 - 0.001, -((lvl_height + 1) * cube_height / 2.0));
	vec3 lower_right = vec3((lvl_width - 1) * cube_width / 2.0,
			-cube_depth / 2.0 - 0.001, (lvl_height - 1) * cube_height / 2.0);
	_base = new base(program, 0, upper_left, lower_right);

	// building walls and holes
	for (int i = 0; i < lvl_height; ++i) {
		for (int j = 0; j < lvl_width; ++j) {
			if (map[i][j] == '#') { //wall
				walls[i][j] = new cube();
				walls[i][j]->translation = Translate(
						(-1.0 * lvl_width / 2 + j) * cube_width, 0,
						(-1.0 * lvl_height / 2 + i) * cube_height);
//				cout << i << " " << j << " " << -1.0 * lvl_width / 2 + j << " "
//						<< -1.0 * lvl_height / 2 + i << endl;
			} else if (map[i][j] == 'O' || map[i][j] == 'T') { //hole
				holes[i][j] = new hole();
				holes[i][j]->translation = Translate(
						(-1.0 * lvl_width / 2 + j) * cube_width, 0,
						(-1.0 * lvl_height / 2 + i) * cube_height);
			} else if (map[i][j] == 'S') {
				_ball = new ball(program, 0.3, 1, 1);
				_ball->i = i;
				_ball->j = j;
				_ball->translation = Translate(
						(-1.0 * lvl_width / 2 + j) * cube_width, 0,
						(-1.0 * lvl_height / 2 + i) * cube_height);

			}
		}
	}

	//building ball
}

void init_lvl(int level) {
	if (level == 1)
		load_level("small.map");
	else
		load_level("big.map");

	build_lvl();
	cube::init(program);
	hole::init(program);
}

void init(void) {

	// Load shaders and use the resulting shader program
	program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(program);

	init_lvl(level);

	V_mat = LookAt(eye, coi, up);
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
//	vec4 ambient = vec4(0.8, 0.8, 0.8, 1.0);
//	vec4 diffuse = vec4(0.2, 0.2, 0.2, 1.0);
//	vec4 specular = vec4(5.0, 5.0, 5.0, 0.0);
	vec4 ambient = vec4(0.8, 0.8, 0.8, 1.0);
	vec4 diffuse = vec4(0.2, 0.2, 0.2, 1.0);
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
	vec4 ambient = vec4(0.6, 0.6, 0.6, 1.0);
	vec4 diffuse = vec4(0.25, 0.25, 0.25, 1.0);
	vec4 specular = vec4(0.3, 0.3, 0.3, 1.0);
	GLfloat my_shine = 20;
	GLint my_shape_type = 0;
	glUniform1i(shape_type_loc, my_shape_type);
	glUniform1f(Shine_loc, my_shine);
	glUniform4fv(A_loc, 1, (GLfloat*) &ambient);
	glUniform4fv(D_loc, 1, (GLfloat*) &diffuse);
	glUniform4fv(S_loc, 1, (GLfloat*) &specular);
}
void bufferBeforeDrawHole() {
	vec4 ambient = vec4(0.0, 0.0, 0.0, 0.5);
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
	vec4 specular = vec4(1.0, 1.0, 1.0, 0.0);
	GLfloat my_shine = 5000;
	GLint my_shape_type = 3;
	glUniform1i(shape_type_loc, my_shape_type);
	glUniform1f(Shine_loc, my_shine);
	glUniform4fv(A_loc, 1, (GLfloat*) &ambient);
	glUniform4fv(D_loc, 1, (GLfloat*) &diffuse);
	glUniform4fv(S_loc, 1, (GLfloat*) &specular);
}
void bufferBeforeDrawTargetHole() {
	vec4 ambient = vec4(0.0, 0.6, 0.0, 0.5);
	vec4 diffuse = vec4(0.0, 0.6, 0.2, 1.0);
	vec4 specular = vec4(1.0, 1.0, 1.0, 0.0);
	GLfloat my_shine = 5000;
	GLint my_shape_type = 3;
	glUniform1i(shape_type_loc, my_shape_type);
	glUniform1f(Shine_loc, my_shine);
	glUniform4fv(A_loc, 1, (GLfloat*) &ambient);
	glUniform4fv(D_loc, 1, (GLfloat*) &diffuse);
	glUniform4fv(S_loc, 1, (GLfloat*) &specular);
}
void bufferBeforeDrawSufrace() {
	vec4 ambient = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 diffuse = vec4(0.0, 0.0, 0.0, 1.0);
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
	bufferBeforeDrawHole();
	for (int i = 0; i < lvl_height; i++) {
		for (int j = 0; j < lvl_width; j++) {
			if (map[i][j] == 'O') { //hole
				M_mat = holes[i][j]->translation * holes[i][j]->rotation;
				glUniformMatrix4fv(M_loc, 1, GL_TRUE, M_mat);
				holes[i][j]->draw();
			}
		}
	}
	bufferBeforeDrawTargetHole();
	for (int i = 0; i < lvl_height; i++) {
		for (int j = 0; j < lvl_width; j++) {
			if (map[i][j] == 'T') {
				M_mat = holes[i][j]->translation * holes[i][j]->rotation;
				glUniformMatrix4fv(M_loc, 1, GL_TRUE, M_mat);
				holes[i][j]->draw();
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
	case 'a':
		eye.x -= 0.5;
		coi.x -= 0.5;
		glUniformMatrix4fv(V_loc, 1, GL_TRUE, LookAt(eye, coi, up));
		break;
	case 'd':
		eye.x += 0.5;
		coi.x += 0.5;
		glUniformMatrix4fv(V_loc, 1, GL_TRUE, LookAt(eye, coi, up));
		break;
	case 's':
		eye.z += 0.5;
		coi.z += 0.5;
		glUniformMatrix4fv(V_loc, 1, GL_TRUE, LookAt(eye, coi, up));
		break;
	case 'w':
		eye.z -= 0.5;
		coi.z -= 0.5;
		glUniformMatrix4fv(V_loc, 1, GL_TRUE, LookAt(eye, coi, up));
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
		eye.y += 0.5;
		glUniformMatrix4fv(V_loc, 1, GL_TRUE, LookAt(eye, coi, up));
		break;
	case GLUT_KEY_UP:
		eye.y -= 0.5;
		glUniformMatrix4fv(V_loc, 1, GL_TRUE, LookAt(eye, coi, up));
		break;
	}
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
	vec3 P = vec3(1.0 * x / screen_width * 2 - 1.0,
			1.0 * y / screen_height * 2 - 1.0, 0);
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
				zangle -= (last_mx - cur_mx) / 5.0;
			if (abs(xangle) > 30)
				xangle += (last_my - cur_my) / 5.0;

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
int falling_cnt;
int win_cnt;
#define magic 0.19
float dist(float x1, float y1, float x2, float y2) {
	float a = (x1 - x2) * (x1 - x2);
	float b = (y1 - y2) * (y1 - y2);
	return sqrt(a + b);
}
void ball_fall_check() {
	vec4 pos = _ball->translation * vec3(0, 0, 0);
	int i = (int) (pos[2] + lvl_height / 2.0 + 0.5);
	int j = (int) (pos[0] + lvl_width / 2.0 + 0.5);
//	cout << pos[2] << " " << pos[0] << endl;
	if (map[i][j] == 'O' || map[i][j] == 'T') {
		if (dist(pos[0], pos[2], j - lvl_width / 2.0, i - lvl_height / 2.0)
				< _ball->radius) {
			if (map[i][j] == 'O') {
				falling = true;
				falling_cnt = 1000 / TIMERMSECS;
			} else if (map[i][j] == 'T') {
				win = true;
				win_cnt = 4 * 1000 / TIMERMSECS;
			}
		}
	}
}

void reset() {
	if (win && level == 1)
		init_lvl(++level);

	eye = vec3(-0.5, 10, 4);
	coi = vec3(-0.5, 0, 0);
	up = vec3(0, 1, 0);
	glUniformMatrix4fv(V_loc, 1, GL_TRUE, LookAt(eye, coi, up));

	falling = win = false;
	xangle = zangle = speedx = speedz = 0;
	W_mat = RotateX(0) * RotateZ(0);
	_ball->translation = Translate(
			(-1.0 * lvl_width / 2 + _ball->j) * cube_width, 0,
			(-1.0 * lvl_height / 2 + _ball->i) * cube_height);
	glUniformMatrix4fv(W_loc, 1, GL_TRUE, W_mat);
}

void animate(int n) {
	if (falling) {
		falling_cnt--;
		_ball->translation *= Translate(0, -0.1, 0);
		display();
		glutTimerFunc(TIMERMSECS, animate, 0);
		if (falling_cnt == 0)
			reset();
		return;
	}
	if (win) {
		win_cnt--;
		_ball->translation *= Translate(0, +0.05, 0);
		W_mat *= RotateZ(1) * RotateX(1);
		glUniformMatrix4fv(W_loc, 1, GL_TRUE, W_mat);
		display();
		glutTimerFunc(TIMERMSECS, animate, 0);
		if (win_cnt == 0)
			reset();
		return;
	}
	ball_fall_check();
	mat4 tmp = _ball->translation;
	vec4 pos = tmp * vec3(0, 0, 0); //FIXME why do u need multiplication ?? 3shan yetla3lak vec4 badal maheya mat4
	tmp *= Translate(speedz, 0, 0);
	pos = tmp * vec3(0, 0, 0);
	pos[0] += lvl_width / 2.0;
	pos[2] += lvl_height / 2.0;
	if (map[(int) ceil(pos[2] - magic)][(int) floor(pos[0] + magic)] == '#'
			|| map[(int) floor(pos[2] + magic)][(int) ceil(pos[0] - magic)]
					== '#'
			|| map[(int) floor(pos[2] + magic)][(int) floor(pos[0] + magic)]
					== '#'
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
	pos[2] += lvl_height / 2.0;
	pos[0] += lvl_width / 2.0;
	if (map[(int) ceil(pos[2] - magic)][(int) floor(pos[0] + magic)] == '#'
			|| map[(int) floor(pos[2] + magic)][(int) ceil(pos[0] - magic)]
					== '#'
			|| map[(int) floor(pos[2] + magic)][(int) floor(pos[0] + magic)]
					== '#'
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
	glutInitDisplayMode(
			GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(screen_width, screen_height);

	//----------------------------------------
	// If you are using freeglut, the next two lines will check if
	// the code is truly 3.2. Otherwise, comment them out

	//glutInitContextVersion( 3, 2 );
	//glutInitContextProfile( GLUT_CORE_PROFILE );
	//----------------------------------------

	glutCreateWindow("Labyrinth3D");
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

