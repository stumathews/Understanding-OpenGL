#pragma once

#include "Common.h"
#include <functional>

namespace stu 
{
	/// <summary>
	/// Manages a Texture's lifespan
	/// </summary>
	class Texture
	{
	private:
		unsigned char *image;
		std::string imagePath;
		bool isCreated = false;
		bool isBinded = false;
		bool isLoaded = false;
		GLenum target = GL_TEXTURE_2D;
		GLenum textureUnit = GL_TEXTURE0;
		bool isReleased = false;
		std::function<unsigned char*(std::string filePath, int* width, int* height)> getImageData;
		std::function<bool(unsigned char*)> freeImageData;
		int width, height;
		bool isUsingExternalId = false;
		
		/// <summary>
		/// Texture Id in OpenGL
		/// </summary>
		GLuint Id;
	public:
		

		/// <summary>
		/// Create a texture
		/// </summary>
		/// <param name="getImageData">image loading function</param>
		/// <param name="freeImageData">image freeing function</param>
		Texture(
			std::function<unsigned char* (std::string filePath, int* width, int* height)> getImageData, 
			std::function<bool(unsigned char*)> freeImageData);

		Texture(GLuint existingTextureId);

		/// <summary>
		/// Copy constructor to copy other Textures
		/// </summary>
		/// <param name="other"></param>
		Texture(const Texture& other);


		/// <summary>
		/// Create the texture
		/// </summary>
		/// <param name="imagePath">Path to 2d texture</param>
		/// <param name="autoBind">Should we bind it automatically to GL_TEXTURE_2D</param>
		/// <param name="autoLoad">Should we load and upload the texture to GPU</param>
		/// <returns></returns>
		bool Create(std::string imagePath, bool autoBind = false, bool autoLoad = false);

		/// <summary>
		/// Bind testure to a sampler before binding
		/// </summary>
		/// <param name="shaderId"></param>
		/// <param name="samplerName"></param>
		/// <param name="textureUnit"></param>
		void Use(GLuint shaderId, std::string samplerName, GLuint textureUnit);

		/// <summary>
		/// Set texture parameters and bind
		/// </summary>
		/// <param name="target"></param>
		/// <param name="textureUnit"></param>
		/// <param name="autoLoad"></param>
		/// <returns></returns>
		bool Bind(GLenum target = GL_TEXTURE_2D, GLenum textureUnit = GL_TEXTURE0, bool autoLoad = false);

		
		/// <summary>
		/// Load the image and send it to the GPU
		/// </summary>
		/// <param name="autoRelease"></param>
		/// <returns></returns>
		bool Load(bool autoRelease = true);

		/// <summary>
		/// Unbind texture from texture Id
		/// </summary>
		void Release();

		// Release if we've not released before
		~Texture();

		/// <summary>
		/// Texture Width
		/// </summary>
		/// <returns></returns>
		int GetWidth();


		/// <summary>
		/// Texture Height
		/// </summary>
		/// <returns></returns>
		int GetHeight();


		/// <summary>
		/// Texture Id
		/// </summary>
		/// <returns></returns>
		GLuint GetId();

	};
}

