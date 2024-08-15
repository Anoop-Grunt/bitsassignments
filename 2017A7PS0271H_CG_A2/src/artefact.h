#pragma once
#include<string.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"
#include "Global_Bind_Test.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include  <map>

///Any object in the scene, objects are loaded in using assimp
///Objects can only have materials, not textures, and only the indices, vertices, normals, specular, ambient, diffuse lighting coeffs, and the aplha blending strength are read from the .obj files
class scene_artefact
{
public:
	///@param path the path to the .obj file
	///
	///The constructor for the class, loads in the model and sets up the meshes(with VAO's VBO's and EBO's)
	scene_artefact(std::string const& path);
	///The default destructor.
	///Note that this version doesn't handle GPU memory management, and the programmer needs to delete the VAO's that aren't being used to avoid memory overflow in the VRAM
	~scene_artefact();
	///@param shader The appropriate shader object
	///@param model The model matrix of the object(the object's transform), needs to be a glm::mat4
	///
	///Draws the object on screen, we consider this method legacy, and dont recommend using this unless GPU memory is unavailable
	void illustrate(Shader shader, glm::mat4 model);
	///@param v vector of all the vertices
	///@param i vector of all the indices
	///
	///returns a copy of the mesh_prototype object created in the solve_child method, the reason we use this is because when we push a mesh_prototype object into the meshes vector it is stored by reference, and when the object goes out of the destructor is called, resulting in us havinmg to fetch the data from the RAM again
	///To avoid that we create a copy of the class, and since OpenGl bindings are universal the VAO's are not los, 
	///Also the copy is not created by calling the constructor again, but by replicating memory, so we don't need to worry about extra VAO's
	mesh_prototype cook(std::vector<float> v, std::vector<unsigned int> i);
	///@param shader The appropriate shader object
	///@param model The model matrix of the object(the object's transform), needs to be a glm::mat4
	///
	///The recommended way to draw objects on the screen, this is significantly faster than the other method
	///, but the programmer needs to delete VAO's that are not going to be reused to avoid overflow.
	void quick_illustrate_unstable(Shader shader, glm::mat4 model);
private:
	///A vector containg all the meshes of the object
	std::vector<mesh_prototype> meshes;
	///A map with key value as the alpha, and  the mesh index as the value
	std::map<float, unsigned int>  blend_indices_approximate;
	///A vector  containing all the vertex data of the meshes, the first indx here corresponds to the mesh index
	std::vector<std::vector<float>> vertex_data;
	///A vector containg all the indices of the meshes, the first index here corresponds to the mesh index       
	std::vector<std::vector<unsigned int>> index_data;
	///A vector containing the diffuse strenghts of the materials, the index corresponds to the mesh index
	std::vector<glm::vec4> diffs;
	///A vector containing the ambient strengths of the materials, the index corresponds to the mesh index
	std::vector<glm::vec4> ambs;
	///A vector containing the specualt strengths of the materials, the index here corresponds to the mesh index
	std::vector<glm::vec4> specs;
	///A vector containing the aplha values of the materials, the index here corresponds to the 
	std::vector<float> alphas;
	///A vector containing the shininess values of the materials, the index here corresponds to the mesh index 
	std::vector<float> shines;
	///A parameter internally used for alpha blending
	int blend_control = 0;
	///@param node pointer to the child node
	///@param scene pointer to the scene object
	/// 
	///recursively obtains the meshes an materials from the assimp tree structure
	void solve_child(aiNode* node,const aiScene* scene);
};

