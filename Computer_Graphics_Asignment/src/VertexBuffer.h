#pragma once

class VertexBuffer {
public:
	unsigned int m_RendererID;///< id of the vertexBuffer


	///@param data an array of vertices 
	///@param size size of the vertex buffer
	///allocates memory in the VRAM

	VertexBuffer(const void* data, unsigned int size);

	///destroy current vertex buffer
	~VertexBuffer();
	///binds vertexbuffer to the GL_ARRAY_BUFFER
	void Bind()const;
	///unbinds current vertexbuffer
	void Unbind()const;
};