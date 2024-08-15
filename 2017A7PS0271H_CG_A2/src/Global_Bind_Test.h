#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "vert_array_quick_bind.h"
#include "GL/glew.h"
///Class associated with polygonal meshes.
///called prototype because meshes can contain vertices of only one layout :{3floats for position, 3floats for normal }
class mesh_prototype
{
public:
	///@param vertices pointer to the contiguous segment of memory where the vertices are stored
	///@param indices pointer to the contiguous segment of memory where the indices are stored
	///@param vbsize the size of the memory segment where the vertices are stored
	///@param ibsize the size number of indices in the mesh
	///Creates a vertex buffer, an index bvuffer and a vertex array object with default layout.
	///The layout cant be changed so meshes with texture uv's etc will not work properly
	mesh_prototype(float* vertices,unsigned int * indices, unsigned int vbsize, unsigned int ibsize);
	///The default destrcutor,
	/// note that in this version the associated VBO, VAO, EBO are not deleted, the programmer himself has to handle the GPU memory management
	~mesh_prototype();
	///Draws the mesh, the model matrix can't be specified here,
	/// it needs to be specified in the scene_artefact class
	void draw();
private:
	///The vertex array object associated with the mesh
	VertexArray va;
	///Index to the elemnt array associated with the mesh
	unsigned int ibHandle;
	///The number of indices in the mesh
	unsigned int ibsize;
	///Pointer to the segment of CPU memory containing the indices 
	unsigned int* indices;
	///IndexBuffer object associated with the mesh
	IndexBuffer ib;
};




