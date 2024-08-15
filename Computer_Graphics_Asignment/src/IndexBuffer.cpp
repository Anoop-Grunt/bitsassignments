#include "IndexBuffer.h"
#include <GL/glew.h>

///@param data array of indicies
///@param count no of indicies
///this will intialize an buffer and store data into the buffer

IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count)
	:m_count(count)
{
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
}
///this will delete the current buffer
IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_RendererID);
}
///binds current index buffer to GL_ELEMENT_ARRAY_BUFFER target
void IndexBuffer::Bind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}
///unbinds current index buffer.
void IndexBuffer::Unbind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

