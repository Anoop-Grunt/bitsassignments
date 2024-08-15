#include "ray.cuh"
using namespace glm;
/// The camera class for the ray tracer, 
/// Note that this camera is static, because the rendering time for simple framebuffers is around 25 seconds.
class camera {
public:
/// The constructor for the class,
/// the __device__ declspec means that the constructor can be called ony on the GPU
    __device__ camera();
///@param u The u co-ordinate of the pixel, on the projection plane.
///@param v The v co-ordinate of the pixel, on the projection plane.
///
///Returns a ray that passes through u, v on the projection plane. The origin is always the cameras position. 
///NVCC generates the object code which frees up GPU heap memory.
    __device__ ray get_ray(float u, float v);
///The default destructor, 
///NVCC generates the object code which frees up GPU heap memory.
    __device__ ~camera();
/// A glm::vec3 object
/// which stores the projection planes center position.
    vec3 origin;
/// A glm::vec3 object
/// which stores the projection planes lower left position.
    vec3 lower_left_corner;
/// A glm::vec3 object
/// which stores the cameras right direction.
    vec3 horizontal;
/// A glm::vec3 object
/// which stores the cameras up direction.
    vec3 vertical;

};
