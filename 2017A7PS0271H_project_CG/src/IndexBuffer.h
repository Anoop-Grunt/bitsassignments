#pragma once
///Wrapper class around the glElementBuffer type.
///Creates an element buffer with the given indices,the index of the element buffer is automatically generated 
class IndexBuffer {
private:
	///Stores the number indices in the associated element buffer
	unsigned int m_count;
public:
	///Stores the index of the associated element buffer
	unsigned int m_RendererID;
	///@param data pointer to the segment of contiguous memory of indices
	///@param count Number of indices 
	///
	/// The constructor for the class
	IndexBuffer(const unsigned int * data, unsigned int count);
	/// The default destructor, 
	///unlike older versions this doesn't handle GPU memory management, so correctness is traded for speed
	~IndexBuffer();
	/// Binds the associated index buffer to the glElementArrayBuffer target
	///Use when drawing with glDrawElements
	void Bind()const;
	/// Unbinds the associated index buffer
	/// Use after the draw call hgas been issued
	void Unbind()const;
	/// returns the number of indices in the associated index buffer
	///Generally used only for debugging
	inline unsigned int getCount()const {
		return m_count;
	}
};