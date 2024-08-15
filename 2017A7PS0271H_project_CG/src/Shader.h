#pragma once
#include <string>
#include "GL/glew.h"
#include "glm/mat4x4.hpp"


///A structure storing string pointers to the source code of the vertex shader, and fragment shader
/// Use to select shader before issuing draw calls, do not forget to set uniforms
struct  ShaderProgramSource
{
	std::string vertex_shader_source;
	std::string fragment_shader_source;
};
///A wrapper class around the glShaderProgram type
///Only suppoerts vertex and fragment shaders
class Shader
{
public:
	///The constructor for the class.
	///The shaders need to be in a single file, the vertex shader source should start with #vertex shader, and fragment shader needs to start with #fragment shader
	///@param filepath a string specifying the path to the file containing the source codes for the shaders
	Shader(const std::string& filepath);
	///The default destructor
	~Shader();
	///A wrapper around the glUseProgram function
	void Bind() const;
	///Unbinds the shader program
	void Unbind() const;

	//set uniforms
	///@param name name of the uniform in the shader source
	///@param f1 the x component of the vec4
	///@param f2 the y component of the vec4
	///@param f3 the z component the vec4
	///@param f4 the w component of the vec4
	///
	///Used to send a uniform of vec4 to the shader program
	void setUniform4f(const std::string& name, float f1, float f2, float f3, float f4);
	///@param name name of the uniform in the shader source
	///@param matrix a glm::mat4 object
	///
	///Used to send a mat4 uniform vaeiable to the shader
	void setUniform4mat4f(const std::string& name, const glm::mat4 matrix);
	///@param name name of the uniform in the shader soruce
	///@param f1 the float value of the uniform variable
	///
	///Used to send a float uniform to the shader
	void setUniformfloat(const std::string& name, const float f1);
	
private:
	///Stores the index of the shader program
	unsigned int m_RendererID;
	//uniforms cahcing
	///@param name name of the uniform in the shader soruce
	///
	///a wrapper around glGetUniformLocation,
	///returns index to the uniform in the shader program
	unsigned int GetUniformLocation(const std::string& name);
	///The path to the shader source code
	const std::string m_filepath;
	///@param vertex_shader_source a C_string of the vertex shader source code
	///@param fragment_shader_source a C_string of the fragment shade source code
	///
	///returns index to the compiled shader program
	int CreateShader(std::string vertex_shader_source, std::string fragment_shader_source);
	///@param type enumerator specifying the shader type, must be one of VERTEX_SHADER, or FRAGMENT_SHADER
	///@param source a C_string of the specific shader's source code
	///
	///Compiles the two shader source codes and returns a index to a shader program
	unsigned int CompileShader(GLenum type, std::string source);
	///@param filepath a string specifying the path to the file containing the source code
	///
	/// retrieves the vertex and fragment shadersource codes from the file
	ShaderProgramSource ParseShader(const std::string& filepath);
};


