#pragma once

class IndexBuffer {
public:
	unsigned int m_RendererID;///<id of the index buffer used in the rendering pipeline
	unsigned int m_count;///< no of indicies
	IndexBuffer(const unsigned int * data, unsigned int count);
	~IndexBuffer();

	void Bind()const;
	void Unbind()const;
	inline unsigned int getCount()const {
		return m_count;
	}
};