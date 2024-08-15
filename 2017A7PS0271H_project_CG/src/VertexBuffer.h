#pragma once
#include "GL/glew.h"
///A wrapper class around the OpenGL glVertexBuffer type.
///
///Earlier version would delete the buffer when the desstructor was called, meaning that 
///subsequent draw calls would need to reload the buffer from CPU memory
/// The current version does not handle GPU memory management,but is much faster, so use accordingly
class VertexBuffer {
private:
	///stores the index of the vertex buffer associated with the object
	unsigned int m_RendererID;
public:
	///constructor for the class 
	///@param data Pointer to the segment of contiguous memory
	///@param size Size in bytes of the vertex set
	VertexBuffer(const void* data, unsigned int size);
	///The default destructor, unlike earlier debug versions this doesn't handle GPU memory management
	/// ,so correctness is traded for speed
	~VertexBuffer();
	///Binds the associated vertex buffer to the GL_ARRAY_BUFFER target
	///Use when drawing with the glDrawArrays calll
	void Bind()const;
	///Unbinds the associated vertex buffer
	///Use after the draw call has been issued
	void Unbind()const;
};


