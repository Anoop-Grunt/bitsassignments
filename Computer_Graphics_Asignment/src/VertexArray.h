#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
public:
    ///generates vertex array object with the given vertex buffer and specified layout
	VertexArray();
	///deletes current vertex array object
	~VertexArray();
	///@param vb vertex buffer whose layout will be specified
	///@param layout the vertex buffer layout
	void AddBuffer(const VertexBuffer vb, const VertexBufferLayout layout);
	///binds vertex array object
	///@note if vertex array is bound there is no need to bind vertex buffer
	void Bind() const;
	///unbinds the current vertex array object
	void Unbind() const;
	unsigned int m_rendererID;///< id of the Vertex Array object to be used in the rendering pipeline.
};
