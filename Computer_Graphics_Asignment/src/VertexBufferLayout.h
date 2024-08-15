#pragma once
#include <vector>
#include "GL/glew.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
	static unsigned int GetSizeofType(unsigned int type) {
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
		}
	}
};

class VertexBufferLayout
{
public:
    /// intialises m_stride to zero
	VertexBufferLayout()
		:m_stride(0) {}
	
	///
	template<typename T>
	void Push(unsigned int count) {
		static_assert(false);
	}

	template<>
	/// @param count no of permitives per vertex
	///sets normalization to GL_FALSE and type to GL_FLOAT and uses these to calculate m_stride
	void Push<float>(unsigned int count) {
		m_elements.push_back({ GL_FLOAT, count, GL_FALSE});
		m_stride += VertexBufferElement::GetSizeofType(GL_FLOAT) * count;
		printf("");
	}

	template<>
	/// @param count no of permitives per vertex
	///sets normalization to GL_FALSE and type to GL_UNSIGNED_INT and uses these to calculate m_stride
	void Push<unsigned int>(unsigned int count) {
		m_elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_stride += VertexBufferElement::GetSizeofType(GL_UNSIGNED_INT)*count;
	}
	/// @param count no of permitives per vertex
	///sets normalization to GL_FALSE and type to GL_UNSIGNED_BYTE and uses these to calculate m_stride
	template<>
	void Push<unsigned char>(unsigned int count) {
		m_elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_stride += VertexBufferElement::GetSizeofType(GL_UNSIGNED_BYTE) * count;
	}
	///@return structure containing details of the vertices's primitives.
	inline const std::vector <VertexBufferElement> GetElements() const { return m_elements; }
	///@return offset(bytes) to the next vertex in the vertex buffer.
	inline unsigned int GetStride() const{ return m_stride; }

	std::vector<VertexBufferElement> m_elements;///< structure containing details of the vertices's primitives.
	unsigned int m_stride;///< offset(bytes) to the next vertex in the vertex buffer.

};
