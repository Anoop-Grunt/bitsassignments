/**@file ray.cuh */
#pragma once
#include <cuda.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"


#define GLM_FORCE_CUDA
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


///Objects of the ray class signify rays of light emitted by the camera.
/// The rays are supposed to hit objects and return the corresponding color value.
class ray
{
public:
///@param direction The direction vector of the ray, needs to be a glm::vec3 object.
///@param origin The starting point of the ray, needs to be a glm::vec3 object.
///
///The constructor for the class,  takes the origin and direction, as parameters. ray objects are created on the GPU heap as specified by the __device__ declspec. 
	__device__ ray(glm::vec3 origin, glm::vec3 direction);
///The default destructor for the class, NVCC generates the object code which frees heap memory.
	__device__ ~ray();
///Returns a glm::vec3 object, whose x,y,z fields are the co-ordinates of the origin of the ray
	__device__ glm::vec3 get_origin();
///Returns a glm::vec3 object, whose x,y,z fields are the components of the unit vector in the rays direction
	__device__ glm::vec3 get_direction();
///Returns a glm::vec3 object, whose x,y,z fields are the co-ordinates of the point on the ray at parameter t,
/// the point is calculated as __origin + t*direction__ 
	__device__ glm::vec3 get_point_at_t(float t);



///glm::vec3 object which stores the origin of
///the ray
	glm::vec3 origin;
///glm::vec3 object which stores the direction of
///the ray
	glm::vec3 direction;
};

