/**@file main.cu */
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include<GL/glew.h>
#include<iostream>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "vert_array_quick_bind.h"
#include "Global_Bind_Test.h"
#include "freecam.h"
#include <functional>
#include <fstream>
#include <sstream>
#include "artefact.h"
#include <stb_image/stb_image.h>
#include "cudaGL.h"
#include "cuda_gl_interop.h"
#include <thrust/device_vector.h>
#include "Texture.h"
#include "ray.cuh"
#include "sphere.cuh"
#include "scene.cuh"
#include <float.h>
#include "ray_tracing_camera.cuh"
#include <curand_kernel.h>
#include <glm/gtc/random.hpp>

using namespace glm;
///error checking support
#define gpuCheckErrs(ans) { gpuAssert((ans), __FILE__, __LINE__); }
///call inside this function for CUDA error checking.
inline void gpuAssert(cudaError_t code, const char* file, int line, bool abort = true)
{
	if (code != cudaSuccess)
	{
		fprintf(stderr, "GPUassert: %s %s %d\n", cudaGetErrorString(code), file, line);
		if (abort) exit(code);
	}
}

using namespace std;
///The raster camera, not really needed, but since we are using an unmodified older shader that does need this, we left it in.
freecam primary_cam;
///Wrapper function for the mouse controller
void MouseControlWrapper(GLFWwindow* window, double mouse_x, double mouse_y) {
	primary_cam.mouse_handler(window, mouse_x, mouse_y);
}
///Wrapper class for the scroll controller
void ScrollControlWrapper(GLFWwindow* window, double x_disp, double y_disp) {
	primary_cam.scroll_handler(window, x_disp, y_disp);
}
///A random state uniform from the CuRanD library is returned
#define RANDVEC3 vec3(curand_uniform(local_rand_state),curand_uniform(local_rand_state),curand_uniform(local_rand_state))
///@param local_rand_state A pointer the the CUDA random state
///
///Returns a point inside a unit sphere on the surface of the sphere where the ray
/// intersection takes place.
__device__ vec3 random_in_unit_sphere(curandState* local_rand_state) {
	vec3 p;
	do {
		p = 2.0f * RANDVEC3 - vec3(1, 1, 1);
	} while ((length(p)) * (length(p)) >= 1.0f);
	return p;
}
///@param spheres device pointer to the memory segment on the VRAM where the sphere definitions are present.
///@param dev_ptr dev_ptr device pointer to the GPU memory segment where the scene is to be set up.
///@param count The number of spheres in the scene.
__global__ void make_scene(sphere** spheres, scene** dev_ptr, int count) {
	*dev_ptr = new scene(spheres, count);
}
///@param v The incident ray direction.
///@param n The unit vector of the normal at the intersection point.
///
///Returns the direction of the reflected
/// ray.
/// The __device__ declspec means that this can only be called on the GPU.
__device__ vec3 reflect(vec3 v, vec3 n) {
	return v - 2 * dot(v, n) * n;
}
///@param uv The direction vector of the incident ray.
///@param n  The normal at the point where the ray intersects the sphere.
/// @param etai_over_etat The refractive index of the spheres material.
///
///Returns the direction of the refracted ray
/// the __device__ declspec means that this can only be called on the GPU.
__device__ vec3 refract(vec3 uv, vec3 n, float etai_over_etat) {
	auto cos_theta = dot(-uv, n);
	vec3 r_out_parallel = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_perp = -sqrt(1.0f - (length(r_out_parallel)) * (r_out_parallel)) * n;
	return r_out_parallel + r_out_perp;
}
__device__ vec3 ofset(ray r, vec3 origin) {
	return origin;
}
///@param r The ray with which the the intersection is done.
///@param sky A pointer to the sky texture character array.
///@param su The u co-ordinate of the sky pixel which needs to be projected.
///@param sv the v co-ordinate of the sky pixel which needs to be projected.
///@param sc The device pointer to the memory segment where the scene was set up. 
///@param local_rand_state pointer to the CuRand state associated with current pixel.
///@param depth The maximum recursion depth.
///
///Returns the pixel color by recursively multiplying the attenuated colors, after 10 light bounces.
/// The __device__ declspec means that this function can only be called on the GPU.
__device__ vec3 pix_data3(ray r, unsigned char* sky, int su, int sv, scene** sc, curandState* local_rand_state, int depth) {
	if (depth <= 0)
		return vec3(0.f, 0.f, 0.f);
	sphere_hit_details rec;
	bool hit = (*sc)->hit_full(r, rec);

	if (hit)
	{
		vec3 N = vec3(rec.normal.x, rec.normal.y, rec.normal.z);
		vec3 target = rec.p + rec.normal + random_in_unit_sphere(local_rand_state);
		vec3 bounce_ray_dir;
		vec3 bounce_ray_origin;
		/*if (rec.type == REFLECTIVE) {
			vec3 reflected_ray_dir = glm::normalize(reflect(r.get_direction(), N));
			bounce_ray_dir = reflected_ray_dir;
			bounce_ray_origin = rec.p;
		}*/
		if (rec.type == REFRACTIVE) {
			vec3 refracted_ray_dir = glm::normalize(refract(normalize(r.get_direction()), N, rec.ref_ind));
			bounce_ray_dir = refracted_ray_dir;
			bounce_ray_origin = rec.p;
		}
		if (rec.type == DIFFUSE) {
			vec3 target = rec.p + rec.normal + random_in_unit_sphere(local_rand_state);
			bounce_ray_dir = target - rec.p;
			bounce_ray_origin = rec.p;
		}

		ray scattered = ray(bounce_ray_origin, bounce_ray_dir);
		vec3 albedo = rec.albedo;
		/*if (dot(scattered.get_direction(), rec.normal) > 0) {
			return albedo * pix_data3(scattered, sky, su, sv, sc, local_rand_state, depth - 1);
		}*/
		return albedo * pix_data3(scattered, sky, su, sv, sc, local_rand_state, depth - 1);

		return vec3(0.f, 0.f, 0.f);
		/*return 0.5f * vec3(N.x + 1, N.y + 1, N.z + 1);*/
		//return vec3(0.f, 0.f, 0.5f) * pix_data3(ray(rec.p, target - rec.p), sky, su, sv, sc, local_rand_state, depth -1);
	}

	else
	{
		/*vec3 sky_col;
		int index = sv * 1920 * 3 + su * 3;
		int r = (int)sky[index];
		float rc = (float)((float)r / 255);
		int g = (int)sky[index + 1];
		float gc = (float)((float)g / 255);
		int b = (int)sky[index + 2];
		float bc = (float)((float)b / 255);
		sky_col.x = rc;
		sky_col.y = gc;
		sky_col.z = bc;
		return sky_col;*/
		vec3 unit_direction = glm::normalize(r.get_direction());
		float t = 0.5f * (unit_direction.y + 1.0f);
		return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.9, 0.7, 1.0);
	}
}
///@param pix_buff_loc A device pointer to the segment of memory on the GPU with the OpenGL pixel buffer object.
///@param sky A pointer to the sky texture character array.
///@param max_x The index of the left_most pixel of the frame buffer.
///@param max_y The index of the right most pixel of the frame buffer.
///@param rand_state pointer to the array of CuRand states.
///@param sc Device pointer to the segment of memory containing the scene object.
///
///Populates the pixel buffer with appropriate pixel colors and intensities, by implementing the ray tracing algorithm. MultiSample AntiAliasing is also implemented(Sample Count = 100).
/// The __device__ declspec means that this function can only be called on the GPU.
__global__ void render(unsigned char* pix_buff_loc, int max_x, int max_y, unsigned char* sky, scene** sc, curandState* rand_state) {
	int i = threadIdx.x + blockIdx.x * blockDim.x;
	int j = threadIdx.y + blockIdx.y * blockDim.y;
	if ((i >= max_x) || (j >= max_y)) return;
	int pixel_index = j * max_x * 4 + i * 4;
	curandState local_rand_state = rand_state[(int)pixel_index / 100];
	camera c;
	vec3 col(0, 0, 0);
	float sample_count = 100.f;
	for (int s = 0; s < sample_count; s++) {
		float u = float(i + curand_uniform(&local_rand_state)) / float(max_x);
		float v = float(j + curand_uniform(&local_rand_state)) / float(max_y);
		ray r1 = c.get_ray(u, v);

		col += pix_data3(r1, sky, i, j, sc, &local_rand_state, 10);
	}
	col = col / sample_count;

	unsigned char r = (int)(255 * col.x);
	unsigned char g = (int)(255 * col.y);
	unsigned char b = (int)(255 * col.z);
	//remove the r+1 when not using the texture
	pix_buff_loc[pixel_index + 0] = (int)r + 1;
	pix_buff_loc[pixel_index + 1] = (int)g;
	pix_buff_loc[pixel_index + 2] = (int)b;
	pix_buff_loc[pixel_index + 3] = 255;
}
///@param rand_state Pointer to the GPU memory segment where the CuRand states are to be initailized.
///
///Initializes CUDA random states at the given device pointer.
__global__ void render_init(curandState* rand_state) {
	int index = blockDim.x + threadIdx.x;
	curand_init(1984, index, 0, &rand_state[index]);
}
///@param sph Device pointer to thge GPU memory segment where the spheres are to be initialized.
///@param count The number of spheres in the scene.
///
///Initializes the sphere objects, modify body to add remove, or resize the spheres.
__global__ void add_spheres(sphere** sph, int count) {
	*(sph) = new  sphere(vec3(-.5f, .00005f, -2.5f), .5f, vec3(1.f, 1.f, 1.f), REFRACTIVE, 1.7f);
	*(sph + 1) = new sphere(vec3(.5f, .00005f, -1.5f), .35f, vec3(0.9f, 0.1f, 0.98f), DIFFUSE, 0.f);
	*(sph + 2) = new sphere(vec3(0.f, -100.5f, -1.f), 100.f, vec3(0.15f, 0.996f, 0.15f), DIFFUSE, 0.f);
	*(sph + 3) = new sphere(vec3(1.5f, .00005f, -2.5f), .5f, vec3(0.98f, 0.2f, 0.2f), DIFFUSE, 0.f);
	*(sph + 4) = new sphere(vec3(-1.5f, .00005f, -2.5f), .5f, vec3(0.2f, 0.2f, 0.992f), DIFFUSE, 0.f);
}
///The main driver, sets up the GLFW OpenGL context, then the Textures and Pixel Buffer Object. Also renders the frame buffer to the Context. The Device pointers, shared memory, OpenGL interop are al configured within the body. The thread dispatch is also configured.
/// Effectively the mfunction draws a fullscreen quad onto the context and then texture maps the ray traced pixel buffer which is shared with CUDA onto the Quad.
int main()
{
	cudaSetDevice(0);

	GLFWwindow* window;
	if (!glfwInit())
		return -1;
	window = glfwCreateWindow(1920, 1080, "CUDA project", glfwGetPrimaryMonitor(), NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glewInit();
	float vertices[] = {
		// positions          // colors           // texture coords
		1.f,  1.f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		1.f, -1.f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-1.f, -1.f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-1.f,  1.f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	VertexBuffer vb(vertices, sizeof(vertices));
	IndexBuffer eb(indices, 6);
	VertexArray va;

	va.spec_vertex_size(8);
	va.add_layout_spec(3);
	va.add_layout_spec(3);
	va.add_layout_spec(2);
	va.AddBuffer(vb);
	Texture t;
	int width, height, nrChannels;
	width = 1920;
	height = 1080;
	nrChannels = 4;

	unsigned int pbo;
	glGenBuffers(1, &pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, width * height * nrChannels * sizeof(GLubyte), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
	cudaGraphicsResource* res;
	gpuCheckErrs(cudaGraphicsGLRegisterBuffer(&res, pbo, cudaGraphicsMapFlagsNone));
	gpuCheckErrs(cudaGraphicsMapResources(1, &res, 0));
	unsigned char* out_data;
	size_t num_bytes;
	gpuCheckErrs(cudaGraphicsResourceGetMappedPointer((void**)&out_data, &num_bytes, res));

	int tx = 8;//threads x
	int ty = 8;//threads y
	dim3 blocks(width / tx + 1, height / ty + 1);
	dim3 threads(tx, ty);

	curandState* d_rand_state;
	gpuCheckErrs(cudaMalloc((void**)&d_rand_state, sizeof(curandState)));
	render_init << <512, 108 >> > (d_rand_state);

	//setting up the sky

	int w, h, n;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("res/textures/sky3.jpg", &w, &h, &n, 0);
	unsigned char* sky;
	cudaMalloc(&sky, w * h * 3);
	cudaMemcpy(sky, data, w * h * 3, cudaMemcpyHostToDevice);

	//setting up the rest of the scene

	sphere** spheres;
	cudaMalloc(&spheres, sizeof(sphere*) * 5);
	add_spheres << < 1, 1 >> > (spheres, 5);

	scene** sc;
	cudaMalloc(&sc, sizeof(scene*));
	make_scene << < 1, 1 >> > (spheres, sc, 5);

	vec3 lower_left_corner(-1.6, -0.9, -1.0);
	vec3 horizontal(3.2, 0.0, 0.0);
	vec3 vertical(0.0, 1.8, 0.0);
	vec3 origin(0.0, 0.0, 0.0);
	render << <blocks, threads >> > (out_data, width, height, sky, sc, d_rand_state);
	cudaGraphicsUnmapResources(1, &res);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pbo);
	glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	t.use_pbo(width, height);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	Shader s("res/shaders/tex_basic.shader");
	glfwSetCursorPosCallback(window, MouseControlWrapper);
	glfwSetScrollCallback(window, ScrollControlWrapper);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);

	while (!glfwWindowShouldClose(window))
	{
		primary_cam.input_handler(window);
		glClearColor(0.f, 0.f, 0.f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		s.Bind();
		t.Bind();
		va.Bind();
		eb.Bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		va.Unbind();
		s.Unbind();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}