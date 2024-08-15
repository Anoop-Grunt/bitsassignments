#pragma once
#include "sphere.cuh"

///Generally only one object of this class exists on the GPU at any one instant
///, the object contains pointers to all the spheres in the scene. 
class scene
{
public:
///@param device_pointer The pointer to the memory segment on the GPU that was allocated Using CudaMalloc,
///Please make sure that the memory allocated is the same as count*sizeof(sphere)
/// @param count the number of spheres in the scene.
///
///The constructor for the class,
/// the __device__ declspec means that the constructor can only be called on the GPU.
	__device__ scene(sphere** device_pointer, int count);
///The default for the class,
/// NVCC generates the object code which frees the GPU heap memory.
	__device__ ~scene();
///@param r The ray with which to perform the hit test.
///@param rec The address of the hit record which is to be updated if the ray succesfully hits a sphere
///
///Returns True if the ray hits any  object in the scene, also 
///modifies the hit record appropriately.
	__device__ bool hit_full(ray r, sphere_hit_details& rec);
/// A device pointer to the
/// memory segment where all the spheres have been defined.
	sphere** spheres;
///The number of spheres
/// in the scene
	int s_count;
};

