#pragma once
#include<sstream>
#include <string>
///An abstraction over the OpenGL specification GLTexture2D,
///Only works with low dynamic range RGB8 type textures.
class Texture
{
public:
///The constructor for
/// the class
	Texture();
///The default
/// destructor.
	~Texture();
/// Binds the associated texture to the GL_TEXTURE_2D slot1.
///Use this before issuing the draw call.
	void Bind();
///Unbinds the texture from the GL_TEXTURE_2D slot and rebinds the originally bound texture
///. Use this after issuing the draw call.
	void Unbind();
///@param path the pointer to the character array containing the sequential RGB8 bytes of the image.
///
///Generates the MipMap from the 
///image
	void addImage(const char* path);
///@param width The width of the texture.
///@param height The height of texture.
///
///In case the texture is to be calculated, then the pixel buffer object needs to hold the texdata,
/// and the texture will read from here. The use_pbo function facilitates exactly that. 
	void use_pbo(int width, int height);
private:

	unsigned int m_RendererID;
	int m_Height;
	int m_Width;
	unsigned char* tex_data;
	int m_NumChannels;
};

