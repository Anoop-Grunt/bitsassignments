#pragma once
#include "VertexBuffer.h"
#include <vector>
#include "IndexBuffer.h"
///A wrapper class around the glVertexArrayObject type.
///Unlike older versions GPU memory management has to be handled by the programmer
class VertexArray
{

public:
	///The constructor for the class
	///Automatically creates a vertex array object with the first available index
	///, and binds it to the GL_VERTEX_ARRAY target
	VertexArray();
	///The default destructor
	///Note that this doesn't handle gpu memory management
	~VertexArray();
	///@param vb The vertex buffer object whose binding will be stored in the associated vertex array object
	///
	///Binds the vertex buffer to the VAO , and caches the binding
	void AddBuffer(const VertexBuffer vb);
	///Binds the associated vertex array object to the GL_VERTEX_ARRAY target,
	///Use before issuing draw call
	void Bind() const;
	///Unbinds the associated vertex array object
	///Use after the draw call has been issued
	void Unbind() const;
	///@param floats_per_vert_attr The number of float values needed to specify the vertex attribute.
	///
	///Adds another vertex attribute to the layout, the attributes need to be specifed in sequence,
	///and need to be stored in a contiguous manner in the vertex buffer, because offset and stride values are calulated on the fly internally 
	void add_layout_spec(int floats_per_vert_attr);
	///@param floats_per_vert the number of float values need to specify all the vertex attributes
	///
	///Used to calculate the offset, 
	///and stride internally
	void spec_vertex_size(int floats_per_vert);
	///Stores the index of the associated vertex array object
	unsigned int m_rendererID;
	///Used internally while specifying layouts, to calculate the offsets
	unsigned int last_index;
	///Used internally while specifying layouts, to calculate the strides
	unsigned int vertex_size;
	///A vector containing the sizes of all the vertex attributes
	std::vector<int> layouts;
	///The offset to the vertex attribute in the vertex spec
	///Used internally while specifying layouts
	int offset;

};

