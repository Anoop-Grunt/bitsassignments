using namespace std;

#include<GL/glew.h>
#include<iostream>
#include "GLFW/glfw3.h"
#include<sstream>
#include<fstream>
#include<string>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "BresenhamLIne.h"
#include "Bresenham_Circle.h"
#include <unordered_set>
#include "TreeBuild.h"
struct Node {
	int data;
	float prelim_x = 0;
	int depth = 0;
	float Mod = 0;
	bool isLeftChild;
	int offset = 0;
	struct Node* left, * right;
};
//float depth_latest[100] = {};
//struct Node* newNode(char data, bool isLeftChild, int depth)
//{
//	struct Node* node = new Node;
//	node->data = data;
//	node->depth = depth;
//	node->isLeftChild = isLeftChild;
//	node->left = NULL;
//	node->right = NULL;
//	return (node);
//}
//void postorder(struct Node* head)
//{
//	
//	struct Node* temp = head;
//	unordered_set<Node*> visited;
//	while (temp && visited.find(temp) == visited.end()) {
//
//		// Visited left subtree 
//		if (temp->left &&
//			visited.find(temp->left) == visited.end())
//			temp = temp->left;
//
//		// Visited right subtree 
//		else if (temp->right &&
//			visited.find(temp->right) == visited.end())
//			temp = temp->right;
//
//		// Print node 
//		else {
//			
//			if(temp->left == NULL){
//				//This means that the node is a leaf node
//				if (temp->isLeftChild) 
//				{
//					//this means the leaf is a left child
//					temp->prelim_x = 0;
//					depth_latest[temp->depth] = temp->prelim_x;
//					
//				}
//				else
//				{
//					//this means the leaf is a right child
//					temp->prelim_x = depth_latest[temp->depth] + 1;
//					
//				}
//			}
//			else
//			{
//				//This means that the node is a parent to atleast one node
//				if (temp->isLeftChild) {
//					//this means node is a left parent
//					if (temp->right == NULL) {
//						//This means that the node is a parent to a single child
//						temp->prelim_x = temp->left->prelim_x;
//						depth_latest[temp->depth] = temp->prelim_x;
//					}
//					else {
//						//This means that there are two children
//						temp->prelim_x = (temp->left->prelim_x + temp->right->prelim_x)/2;
//						depth_latest[temp->depth] = temp->prelim_x;
//						
//					}
//				}
//				else {
//					//This means the node is a right parent
//					temp->prelim_x = depth_latest[temp->depth] + 1;
//					if (temp->right == NULL) {
//						//This means that the node is a parent to a single child
//						float desired_x = temp->left->prelim_x;
//						temp->Mod = temp->prelim_x - desired_x;
//					}
//					else {
//						//This means that there are two children
//						float desired_x = (temp->left->prelim_x + temp->right->prelim_x) / 2;
//						temp->Mod = temp->prelim_x - desired_x;
//						
//					}
//					
//				}
//			}
//			BresenCircle c8(50*temp->prelim_x+ 320, 480 - (temp->depth+1)*50, 20);
//			c8.draw2();
//			printf("%c has prelim x of %f and mod of %f\n", temp->data, temp->prelim_x, temp->Mod);
//			visited.insert(temp);
//			temp = head;
//		}
//	}
//}
//struct  ShaderProgramSource
//{
//	string vertex_shader_source;
//	string fragment_shader_source;
//};
//
//static ShaderProgramSource ParseShader(const std::string& filepath) {
//	std::ifstream stream(filepath);
//
//	enum class ShaderType
//	{
//		NONE = -1, VERTEX = 0, FRAGMENT = 1
//	};
//	ShaderType type = ShaderType::NONE;
//	stringstream ss[2];
//	string line;
//	while (getline(stream, line)) {
//		if (line.find("#shader") != string::npos) {
//			if (line.find("vertex") != string::npos)
//				type = ShaderType::VERTEX;
//			else if (line.find("shader") != string::npos)
//				type = ShaderType::FRAGMENT;
//		}
//		else
//		{
//			ss[int(type)] << line << "\n";
//		}
//	}
//	return { ss[int(ShaderType::VERTEX)].str(), ss[int(ShaderType::FRAGMENT)].str() };
//}
//
//unsigned int CompileShader(GLenum type, std::string source) {
//	unsigned int shader = glCreateShader(type);
//	const char* src = source.c_str();
//	glShaderSource(shader, 1, &src, nullptr);
//	glCompileShader(shader);
//	// nullptr in the length parameter means tat the whole string is used as source code for the sahder
//	//: ERROR CHECKING CODE:
//	int result;
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
//	if (result == GL_FALSE) {
//		int length;
//		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
//		char* message = (char*)malloc(length * sizeof(char));
//		glGetShaderInfoLog(shader, length, &length, message);
//		std::cout << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader failed to compile" << endl;
//		std::cout << message << std::endl;
//		glDeleteShader(shader);
//		return 0;
//	}
//	return shader;
//}
//
//static int CreateShader(std::string vertex_shader_source, std::string fragment_shader_source) {
//	unsigned int program = glCreateProgram();
//	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertex_shader_source);
//	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragment_shader_source);
//	glAttachShader(program, vertexShader);
//	glAttachShader(program, fragmentShader);
//	glLinkProgram(program);
//	glValidateProgram(program);
//	glDeleteShader(vertexShader);
//	glDeleteShader(fragmentShader);
//	return program;
//}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glewInit();
	cout << glGetString(GL_VERSION) << endl;
	unsigned int positions[] = {
		100, 100,
		300, 100,
		300, 300,
		100, 300
	};
	unsigned int indices[] = {
	0, 1, 2,
	2, 0, 3
	};
	unsigned int indices2[] = {
	0, 1, 2,
	2, 1, 3
	};
	//unsigned int vertex_array_object;
	//glGenVertexArrays(1,&vertex_array_object);
	//glBindVertexArray(vertex_array_object);
	VertexArray va;
	VertexBuffer vb(&positions, 8 * sizeof(unsigned int));
	VertexBufferLayout layout;
	layout.Push<unsigned int>(2);
	/*va.AddBuffer(vb, layout);
	va.Bind();*/

	
	int i = 0;
	const auto& elements = layout.GetElements();
	const auto& element = elements[i];
	unsigned int offset = 0;
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
	


		
	IndexBuffer ib(indices, 6);
	glm::mat4 proj = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
	Shader shader("res/shaders/Basic.shader");
	shader.Bind();
	shader.setUniform4f("u_color", 0.85, 0.0, 1.0, 1.0);
	shader.setUniform4mat4f("u_MVP", proj);

	
	
	/*unsigned int location = glGetUniformLocation(shader, "u_color");*/
	float red_value = 0.5f;
	float red_increment = 0.05f;



	/*struct Node* root = newNode('H', true, 0);
	root->left = newNode('D', true, 1);
	root->left->left = newNode('C', true, 2);
	root->left->left->left = newNode('A', true, 3);
	root->left->left->right = newNode('B', false, 3);
	root->right = newNode('G', false, 1);
	root->right->right = newNode('F', false, 2);
	root->right->left = newNode('E', true, 2);*/

	TreeNode g('G', NULL);
	TreeNode e('E', &g);
	TreeNode f('F', &g);
	g.addLeftChild(&e);
	g.addRightChild(&f);
	TreeNode c('C', &e);
	TreeNode d('D', &e);
	e.addLeftChild(&c);
	e.addRightChild(&d);
	TreeNode a('A', &c);
	TreeNode b('B', &c);
	c.addLeftChild(&a);
	c.addRightChild(&b);
	TreeNode k('K', &b);
	TreeNode l('L', &b);
	b.addLeftChild(&k);
	b.addRightChild(&l);
	TreeNode m('M', &f);
	TreeNode n('N', &f);
	f.addLeftChild(&m);
	f.addRightChild(&n);
	TreeNode x('X', &m);
	TreeNode y('Y', &m);
	m.addLeftChild(&x);
	m.addRightChild(&y);

	std::cout << "Here" << std::endl;



	//postorder(root);



	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);
		/*glCall(ib.Bind());*/
		shader.Bind();	
		if (red_value >= 1.0f)
		{
			red_increment = -0.05f;
		}
		if (red_value <= 0.0f) {
			red_increment = 0.05f;
		}	
		shader.setUniform4f("u_color", red_value, 0.0, 1.0, 1.0);
		red_value += red_increment;
		/*glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);*/
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		plot_tree(&g, 50);
	/*	BresenLine l1( 500, 350, 370, 280);
		l1.draw2();
		BresenLine l2( 550 ,300, 300, 200);
		l2.draw2();
		BresenLine l3(0, 0, 640, 480);
		l3.draw2();
		BresenCircle c1(320, 240, 200);
		c1.draw2();
		BresenCircle c2(250, 150, 50);
		c2.draw2();
		BresenCircle c4(100, 200, 45);
		c4.draw();*/
		/*BresenCircle c1(75+ 320, 480- 50, 20 );
		BresenCircle c2(25+320, 480-100, 20);
		BresenCircle c3(25*5 + 320, 480 - 100, 20);
		BresenCircle c4(25 + 320, 480 -150, 20 );
		BresenCircle c5(100 + 320, 480 -150, 20);
		BresenCircle c6(150 + 320, 480 - 150, 20);
		BresenCircle c7(0 + 320, 480 - 200, 20);
		BresenCircle c8(50 + 320, 480 - 200, 20);
		c1.draw2();
		c2.draw2();
		c3.draw2();
		c4.draw2();
		c5.draw2();
		c6.draw2();
		c7.draw2();
		c8.draw2();*/
		/*postorder(root);*/
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}
	/*glDeleteProgram(shader);*/
	glfwTerminate();
	return 0;
}