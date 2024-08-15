#include "Shader.h"
#include<GL/glew.h>
#include<iostream>
#include<sstream>
#include<fstream>
using namespace std;

///@param filepath filepath to shader source.
///@note both the vertex shader and fragement shader's GLSL code in the same file.

Shader::Shader(const std::string& filepath)
	:m_filepath(filepath), m_RendererID(0)
{
	ShaderProgramSource source = ParseShader(filepath);
	string vertex_shader_src = source.vertex_shader_source;
	string fragment_shader_src = source.fragment_shader_source;
	m_RendererID = CreateShader(vertex_shader_src, fragment_shader_src);
}
///deletes currently bound shaded program
Shader::~Shader()
{
	glDeleteProgram(m_RendererID);
}
///binds generated program
void Shader::Bind() const
{
	glUseProgram(m_RendererID);
}
///unbinds any shader being used.
void Shader::Unbind() const
{
	glUseProgram(0);
}
///@param name name of the uniform in the shader source code. 
///takes four float values as parameters, currently only being used to assign colour values to the fragment shader. 
void Shader::setUniform4f(const std::string& name, float f0, float f1, float f2, float f3)
{
	int location = GetUniformLocation(name);
	glUniform4f(location, f0, f1, f2, f3);
}
///@param matrix takes glm::mat4 object. 
///@param name name of the uniform in the shader source code. 
///currently being used to transform pixel coordinates to glfw co ordinates in the vertex shader. 
void Shader::setUniform4mat4f(const std::string& name, const glm::mat4 matrix)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1 ,GL_FALSE, &matrix[0][0]);
}
///@param name name of the uniform in the shader source code. 
///locates the uniform in the shader and @return the id of the uniform.
unsigned int Shader::GetUniformLocation(const std::string& name)
{
	int location = glGetUniformLocation(m_RendererID, name.c_str());
	if (location == -1) {
		std::cout << "warning: uniform " << name << "doen't exist" << std::endl;
	}
	return location;
}

/// @param vertex_shader_source and @param fragment_shader_source takes and compiles them and attaches them to shader program in the pipeline the deletes vertexShader and fragmentShader.
///@return program_id. 

int Shader::CreateShader(std::string vertex_shader_source, std::string fragment_shader_source) {
	unsigned int program = glCreateProgram();
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertex_shader_source);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragment_shader_source);
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glValidateProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program;
}
///@param type is a enumerator which specifies kind of shader being compiled. 
///@param source a string containing source code to the shader. 

///it will create shader program from the source. 
///@return shader id. 
unsigned int Shader::CompileShader(GLenum type, std::string source) {
	unsigned int shader = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);
	// nullptr in the length parameter means tat the whole string is used as source code for the sahder
	//: ERROR CHECKING CODE:
	int result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)malloc(length * sizeof(char));
		glGetShaderInfoLog(shader, length, &length, message);
		std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader failed to compile" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}
///@param filepath filepath to the source where shader programs reside.

/**it separates vertex_shader and fragment_shader from the source file and return them as strings.  */
ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	ShaderType type = ShaderType::NONE;
	stringstream ss[2];
	string line;
	while (getline(stream, line)) {
		if (line.find("#shader") != string::npos) {
			if (line.find("vertex") != string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("shader") != string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			ss[int(type)] << line << "\n";
		}
	}
	return { ss[int(ShaderType::VERTEX)].str(), ss[int(ShaderType::FRAGMENT)].str() };
}