#include "pch.h"
#include "StuFrameBufferObject.h"
#include "Box.h"

namespace stu
{

	FrameBufferObject::FrameBufferObject(size_t width, size_t height)
	{
		this->width = width;
		this->height = height;
	}

	bool FrameBufferObject::Create()
	{
		bool ok = false;
	
		// Generate the Frame Buffer Object
		glGenFramebuffers(1, &id);
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		// Compile the shaders
		ok = CompileShaders();

		CreateTexture();

		AttachFrameBufferObjects();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Setup the primative we'll be drawing the framebuffer's texture on in the Render()
		CreateTextureFrame();

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
			ok = false;
		}
		
		return ok;
	}

	void FrameBufferObject::AttachFrameBufferObjects()
	{
		frameBufferrRenderObject = std::shared_ptr<FrameBufferRenderObject>(new FrameBufferRenderObject());
		frameBufferrRenderObject->Create(width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBufferrRenderObject->GetId());
	}

	void FrameBufferObject::CreateTexture()
	{
		glGenTextures(1, &textureColourBufferId);
		glBindTexture(GL_TEXTURE_2D, textureColourBufferId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	

		// Attach it to currently bound framebuffer object
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourBufferId, 0);
	}

	void FrameBufferObject::Record(std::function<void(void)> drawScene, bool resetToDefaultAfterRender)
	{
		// Use this framebuffer as primary back buffer and write the scene to it
		glBindFramebuffer(GL_FRAMEBUFFER, id);

		// Clear this frame buffer and prepare for writing
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // we're not using the stencil buffer now
		
		// Write to the FBO
		//glEnable(GL_DEPTH_TEST);	
		drawScene();

		if(resetToDefaultAfterRender)
		{
			// Switch back to the default back buffer
			glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
			//glClearColor(1.0f, 0.0f, 0.0f, 1.0f); 
			//glClear(GL_COLOR_BUFFER_BIT);
		}
	}

	void FrameBufferObject::Render()
	{
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		glDisable(GL_DEPTH_TEST);

			shaderProgram->Use();
			shaderProgram->SetUniform("screenTexture", 1);
	
			glActiveTexture(GL_TEXTURE0 + 1);
			glBindTexture(GL_TEXTURE_2D, textureColourBufferId);
			//box->Render();
			square->Render();
	
			//triangle->Render();

			//glBindTexture(GL_TEXTURE_2D, 0);
		glEnable(GL_DEPTH_TEST);
		//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	}

	GLuint FrameBufferObject::GetId()
	{
		return id;
	}

	GLuint FrameBufferObject::GetTextureId()
	{
		return textureColourBufferId;
	}


	bool FrameBufferObject::CompileShaders()
	{
		shaderProgram = std::make_shared<stu::ShaderProgram>(stu::VertexLayout::PosTexNormCol);
		shaderProgram->CopyAddShader(stu::Shader("resources\\shaders\\framebuffer.vert", "Wih offscreen Vertex Shader", GL_VERTEX_SHADER));
		shaderProgram->CopyAddShader(stu::Shader("resources\\shaders\\framebuffer.frag", "With ofscreen Interim Fragment Shader", GL_FRAGMENT_SHADER));

		if(!shaderProgram->Link())
		{
			std::cout << "Failed to compile interim shader program" << std::endl;	
			return false;
		}
		return true;
	}

	void FrameBufferObject::CreateTextureFrame()
	{
		// Create surface we'll be rendering on to. Note that we are using NDCs.. not world or camera coords.
		auto bottLeft = glm::vec3(-0.5, 0.5, 0.0f);
		auto width = 1.0f, height = 0.5f;

		square = std::shared_ptr<stu::Square>(new stu::Square(bottLeft, width, height, shaderProgram));
		box = std::shared_ptr<stu::Box>(new stu::Box(shaderProgram,
			glm::vec3(0.5,0.5f,0.0f), // a
			glm::vec3(-0.5,0.0,0.0f), // d
			glm::vec3(-0.5,1.0,0.0f), // c
			glm::vec3(0.5,1.0,0.0f) // b
	
		));
		triangle = std::shared_ptr<stu::Triangle>(new stu::Triangle());
	
		box->Create();
		square->Create();

		// Why is the triangle rendering the framebuffer better than the others?
		triangle->SetShaderProgram(shaderProgram);
		triangle->Create();
	}

}
