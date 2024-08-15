#pragma once
#include "ray.cuh"
using namespace glm;

///An enumeration that is used to specify the type of the material.
enum mat_type {REFLECTIVE, REFRACTIVE, DIFFUSE };


struct sphere_hit_details

{
    float t;
    vec3 p;
    vec3 normal;
///A boolean representing is the intersection point is
/// inside or outside the sphere, this is required for refraction.
    bool front_face;
///Orients the normal
/// outwards always, 
///the __device__ declspec maintains that the function is compiled only for the GPU.
    __device__ inline void orient_normal(ray r, const vec3 point_out) {
        front_face = dot(r.get_direction(), point_out) < 0;
        normal = front_face ? point_out : -point_out;
    }
///A glm::vec3 object that holds the diffuse color,
/// or the matte color of the sphere.
    vec3 albedo;
/// A mat_type enum,
/// that is used to identify how the light ray bounces off the sphere.
    mat_type type;
/// The refrractive index
/// of the material.
    float ref_ind;
    
};


/// Objects of this class represent spheres in the scene, 
///The spheres are intersected by rays and return their color values.
class sphere
{
public:
///@param center A glm::vec3 specifying the center of the sphere.
///@param r A float value specifying the radius of the sphere.
///@param albedo A glm::vec3 specifying the diffuse color of the material, the format is RGB8.
///@param m The material type.
///@param ref_index The refractive index of the material.
///
/// The contructor for the class,
/// the __device__ declspec means that the construcor can only be called on the GPU.
    __device__ sphere(vec3 center, float r, vec3 albedo, mat_type m, float ref_index);
/// The default destructor, 
///NVCC generates the object code which frees up GPU heap memory.
    __device__ ~sphere();
///@param ray The ray object with which the hit test is to be performed
///@param t_min The minimum value for the t parameter, used to avoid negative t values, and to accomodate for floating point errors
///@param t_max The maximum value for the t_prameter, use so that the ray doesn't continue indefinitely.
///@param rec A sphere_hit_details struct object, contains the details of the hit record.
///
///Returns true if the ray hits the sphere,
/// also modifies the value of the hit record appropriately.
    __device__ bool hit(ray r, float tmin, float tmax, sphere_hit_details& rec);




///A glm::vec3 that stores the center of the 
///sphere 
    vec3 center;
///A glm::vec3 that stores the center of the 
///sphere 
    float radius;
///A glm::vec3 that stores the matte color of the 
///sphere 
    vec3 albedo;
///A glm::vec3 that stores the material type of the 
///sphere 
    mat_type type;
///A glm::vec3 that stores the refractive index of the 
///sphere material
    float ri;
};





