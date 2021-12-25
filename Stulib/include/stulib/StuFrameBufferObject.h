#pragma once
#include "Common.h"
#include "FrameBufferRenderObject.h"
#include <functional>
#include "Square.h"
#include "Box.h"

namespace stu
{

	class FrameBufferObject
	{
		GLuint id;
		GLuint textureColourBufferId;
		std::shared_ptr<FrameBufferRenderObject> frameBufferrRenderObject;
		std::shared_ptr<stu::Square> square;
		std::shared_ptr<stu::Box> box;
		std::shared_ptr<stu::Triangle> triangle;
		std::shared_ptr<stu::ShaderProgram> shaderProgram;
		size_t width, height;

		/// <summary>
		///  Create the Frame Buffer Render Object
		/// </summary>
		void AttachFrameBufferObjects();

		/// <summary>
		/// Generate texture
		/// </summary>
		void CreateTexture();
		bool CompileShaders();
		void CreateTextureFrame();

	public:

		FrameBufferObject(size_t width, size_t height);

		bool Create();
		

		/// <summary>
		/// Switch to this FBO and run the render function to write to the underlying texture
		/// </summary>
		/// <param name="renderThis"></param>
		/// <param name="resetToDefaultAfterRender"></param>
		void Record(std::function<void(void)> renderThis, bool resetToDefaultAfterRender = false);

		void Render();

		GLuint GetId();

		/// <summary>
		/// Gets the Texture associated with this Frame Buffer Object
		/// </summary>
		/// <returns></returns>
		GLuint GetTextureId();
	};

}