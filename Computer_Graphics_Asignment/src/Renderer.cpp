#include "Renderer.h"
#include <iostream>
using namespace std;

/// this function will run untill the no error is detected
void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}


///this function will print the *error type*, *function name*, *file name*, *line number* and return **false** if error occurs
///and  return **true** if error doesn't occur
bool GLLogCall(const char* funct, const char* file, int line) {
	while (GLenum error = glGetError()) {
		cout << "OpenGL error (" << error << ")" << "function :" << funct << " file :" << file << " line : " << line << endl;
		return false;
	}
	return true;
}