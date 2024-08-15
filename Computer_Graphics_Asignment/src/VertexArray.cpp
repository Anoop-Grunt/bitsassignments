#include "VertexArray.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_rendererID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_rendererID);
}

void VertexArray::AddBuffer(const VertexBuffer vb, const VertexBufferLayout layout)
{
	/*Bind();*/
	/*vb.Bind();*/
	/*auto keyword is used when we want the type to be deduced from the initializer*/
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size();i++) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(),(const void*) offset);
		GLint test = 0;
		glGetVertexAttribiv(i,GL_VERTEX_ATTRIB_ARRAY_ENABLED, &test);
		printf("\nvertex attrib array status is : %d", test);
		offset += element.count * VertexBufferElement::GetSizeofType(element.type);
	}

}

void VertexArray::Bind() const
{
	GLint test = 0;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &test);
	glBindVertexArray(m_rendererID);
	int rendCheck;
	glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &test);
	glGetVertexArrayiv(m_rendererID, GL_ELEMENT_ARRAY_BUFFER_BINDING, &rendCheck);
	glBindVertexArray(test);
	printf("\nvertex array binding status is : %d", test);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}
