#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

///Free camera class, 
///Camera is capable of moving around the scene, rotations around the yaw, pitch and roll axes, and zoom
///multiple cameras can be used with multiple windows, but Source.cpp needs to be slightly modified
///Oblique viwing angles are not allowed, and hence the shear step is omitted from the normalizing transform
///Also the initial field of view defaults to 45
class  freecam
{
public:
	///Constructor for the class, the initial position defaults to z = 3, x ,y = 0, 
	///and the camera faces towards the origin
	 freecam();
	///The default destructor,
	///Not really meaningful unless multiple cameras are used
	~ freecam();
	///@param window Ponter to the current OpenGL context 
	///
	///Handles keyboard input, use 'WASD' to move around and'12' to roll
	void input_handler(GLFWwindow* window);
	///@param window pointer to the OpenGL context
	///@param mouse_x the normalized x-co ordinate of the mouse cursor
	///@param mouse_y the normalized y-co ordinate of the mouse cursor
	///
	///handles the mouse input, 
	///Use mouse axis movements to pan the camera around, and the scroll wheel to zoom in and out
	void mouse_handler(GLFWwindow* window, double mouse_x, double mouse_y);
	///@param window pointer to the OpenGL context
	///@param x_disp the fov-x parameter
	///@param y_disp the fov-y parameter
	///
	///Handles scroll wheel input
	///Scroll up to zoom in and down to zoom out
	///Zoom is done by reducing the fov angle
	void scroll_handler(GLFWwindow* window, double x_disp, double y_disp);
	///The view matrix for the camera
	glm::mat4 view();
	///The projection matrix for the camera
	glm::mat4 projection();
	///The cameras position, Used for specular lighting
	glm::vec3 position();


private:
	///The world up vector
	glm::vec3 worldUP = glm::vec3(0.f, 1.f, 0.f);
	///The initial camera position
	glm::vec3 CamPos = glm::vec3(0.0f, 0.0f, 12.0f);
	///The front vector of the camera, this sets which way the camera points
	glm::vec3 CamFront = glm::vec3(0.0f, 0.0f, -1.0f);
	///The position of the object which the camera faces
	glm::vec3 Target = glm::vec3(0.0f, 0.0f, 0.0f);
	
	///The time elapsed between frames
	float deltaTime = 0.0f;
	///The absolute time when the previous frame was drawn
	float prevFrameTime = 0.0f;
	///The width of the window(only needed for aspect ratio)
	float win_x = 960;
	///The height of the window(only needed for aspect ratio)
	float win_y = 540;
	///The angle made by the cameras front vector on the x-z plane(planar angle)
	float yaw = -90.0f;
	///The angle made by the cameras front vector with the x-z plane(azimuthal angle)
	float pitch = 0.f;
	///The fov angle
	float field_of_view = 45.f;
	///Initial value of the cameras front vector
	glm::vec3 cf_init;
	///A boolean indicating wether the current mouse movement is the first of a continous movement
	///Used to reset the mouse position when the user stops moving the mouse
	bool firstMouse = true;


	

};

