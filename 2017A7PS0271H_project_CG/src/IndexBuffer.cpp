#include "IndexBuffer.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count)
	:m_count(count)
{
	int index_buffer_relapse_handle;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &index_buffer_relapse_handle);
	glGenBuffers(1, &m_RendererID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
	Unbind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_relapse_handle);
}

IndexBuffer::~IndexBuffer()
{
	//glDeleteBuffers(1, &m_RendererID);
}

void IndexBuffer::Bind()const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

}

void IndexBuffer::Unbind()const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
