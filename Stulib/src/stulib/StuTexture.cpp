#include "pch.h"
#include "StuTexture.h"

stu::Texture::Texture(std::function<unsigned char* (std::string filePath, int* width, int* height)> getImageData, std::function<bool(unsigned char*)> freeImageData)
{
	width = height = Id = 0;
	image = nullptr;
	this->getImageData = getImageData;
	this->freeImageData = freeImageData;
}

stu::Texture::Texture(GLuint existingTextureId)
{
	this->Id = existingTextureId;
	this->isUsingExternalId = true;
}

bool stu::Texture::Create(std::string imagePath, bool autoBind, bool autoLoad)
{
	this->imagePath = imagePath;

	glGenTextures(1, &Id);

	isCreated = true;
	if (autoBind)
		Bind();
	if (autoLoad)
		Load();
	return isCreated;
}

void stu::Texture::Use(GLuint shaderId, std::string samplerName, GLuint textureUnit)
{
	// Associate the uniform sampler with texture unit
	auto loc = glGetUniformLocation(shaderId, samplerName.c_str());
	glUniform1i(loc, textureUnit);

	// Bind texture to corresponding texture unit
	// Activate a texture unit
	glActiveTexture(GL_TEXTURE0 + textureUnit);

	// Bind the texture to that unit
	// https://learnopengl.com/Getting-started/Textures: After activating a texture unit, a subsequent glBindTexture call will bind that texture to the currently active texture unit.
	glBindTexture(target, Id);

}

bool stu::Texture::Bind(GLenum target, GLenum textureUnit, bool autoLoad)
{
	if (!isCreated)
		return false;
	this->target = target;
	this->textureUnit = textureUnit;

	glBindTexture(target, Id);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	isBinded = true;

	if (isBinded && autoLoad)
		Load();
	return isBinded;
}

stu::Texture::Texture(const Texture& other)
{
	this->Id = 0;
	this->isReleased = false;
	this->target = other.target;
	this->height = other.height;
	this->width = other.width;
	this->Id = 0;
	this->imagePath = other.imagePath;
	this->isBinded = other.isBinded;
	this->isCreated = other.isCreated;
	this->isLoaded = other.isLoaded;
	Load();
}

bool stu::Texture::Load(bool autoRelease)
{
	if (!isCreated || !isBinded)
		return false;

	image = getImageData(imagePath, &width, &height);

	if (image == 0)
		return false;

	glTexImage2D(target, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(target);

	isLoaded = true;

	if (autoRelease)
		Release();
	return true;
}

void stu::Texture::Release()
{
	if (isLoaded)
		freeImageData(image);

	glBindTexture(target, 0);
	isLoaded = false;
}

stu::Texture::~Texture()
{
	if (!isReleased)
		Release();
}

int stu::Texture::GetWidth()
{
	return width;
}

int stu::Texture::GetHeight()
{
	return height;
}

GLuint stu::Texture::GetId()
{
	return Id;
}
