/*
 * utils.h
 *
 *  Created on: Nov 25, 2013
 *      Author: Ashraf Saleh
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <GL/glew.h>

using namespace std;

inline int init_shaders(const char * vshader_file_path,	const char * fshader_file_path) {
	int program;
	int compiled, log_size;


	// Create shaders
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	//read shaders from files
	string vshader_code, fshader_code;
	ifstream fs;
	ostringstream file_contents;

	// reading vshader
	fs.open(vshader_file_path);
	if (!fs.is_open()) {
		perror("couldn't find vshader file");
		exit (EXIT_FAILURE);
	}

	file_contents << fs.rdbuf();
	vshader_code = file_contents.str();
	fs.close();
	file_contents.str("");
	file_contents.clear();


	// reading fshader
	fs.open(fshader_file_path);
	if (!fs.is_open()) {
		perror("couldn't find fshader file");
		exit (EXIT_FAILURE);
	}
	file_contents << fs.rdbuf();
	fshader_code = file_contents.str();
	fs.close();


	// Compile Vertex Shader
	char const * vshader_ptr = vshader_code.c_str();
	glShaderSource(vshader, 1, &vshader_ptr, NULL);
	glCompileShader(vshader);

	// check compilation status
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		cerr << vshader_file_path << " failed to compile:" << endl;
		glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &log_size);
		char* log_message = new char[log_size];
		glGetShaderInfoLog(vshader, log_size, NULL, log_message);
		cerr << log_message << endl;
		delete[] log_message;

		exit (EXIT_FAILURE);
	}


	// Compile Fragment Shader
	char const * fshader_ptr = fshader_code.c_str();
	glShaderSource(fshader, 1, &fshader_ptr, NULL);
	glCompileShader(fshader);

	// check compilation status
	glGetShaderiv(vshader, GL_COMPILE_STATUS, &compiled);
	if (!compiled) {
		cerr << fshader_file_path << " failed to compile:" << endl;
		glGetShaderiv(vshader, GL_INFO_LOG_LENGTH, &log_size);
		char* log_message = new char[log_size];
		glGetShaderInfoLog(vshader, log_size, NULL, log_message);
		cerr << log_message <<endl;
		delete[] log_message;
		exit (EXIT_FAILURE);
	}


	// Link the program
	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);

//	 Check the program
	glGetProgramiv(program, GL_LINK_STATUS, &compiled);
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
	vector<char> program_error_log(max(log_size, int(1)));
	glGetProgramInfoLog(program, log_size, NULL, &program_error_log[0]);
	fprintf(stdout, "%s\n", &program_error_log[0]);

	glDeleteShader(vshader);
	glDeleteShader(fshader);


	return program;
}



#endif /* UTILS_H_ */
