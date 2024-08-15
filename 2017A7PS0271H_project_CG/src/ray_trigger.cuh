

#include "ray.cuh"




using namespace glm;

///A struct returned when a ray intersects with an object in the scene,
/// used to color the pixels appropriately. This class is not used in the final version of the project, because only spheres were used.
struct sphere_hit_details

{
///The parameter t,
/// used to calculate the co-ordinates of the intersection point.
	float t;
///A glm::vec3 object of
/// the intersection point
	vec3 p;
/// A glm::vec3 object
/// of the sphere normal at the point where the ray intersects the sphere.
	vec3 normal;
};
///CLass used to test if a ray intersects a sphere.
/// Contaians methods that allow to check if a given ray intersects a sphere based on the sphere_hit_details provided.
class trigger_test
{
public:
///The constructor for the class, 
///the __device__ declspec__ means that the constructor can only be called on the GPU.
	__device__ trigger_test();
///The default destructor, 
///NVCC generates the object code which frees up heap memory on the GPU.
	__device__ ~trigger_test();


///@param ray The ray object with which the hit test is to be performed
///@param t_min The minimum value for the t parameter, used to avoid negative t values, and to accomodate for floating point errors
///@param t_max The maximum value for the t_prameter, use so that the ray doesn't continue indefinitely.
///@param rec A sphere_hit_details struct object, contains the details of the hit record.
///
///Returns true if the ray hits the sphere,
/// also modifies the value of the hit record appropriately.
	__device__ virtual bool hit(ray r, float t_min, float t_max, sphere_hit_details rec);

private:
};