#include "pch.h"
#include "InstancedRenderer.h"
#include "Shader.h"
#include "VertexAttribute.h"

namespace stu
{

	InstancedRenderer::InstancedRenderer(std::shared_ptr<GameObject> instance, std::vector<glm::vec3> instanceColours)
	{
		this->shaderProgram = shaderProgram;
		this->instance = instance;
		this->instanceColours = instanceColours;
		this->instanceMatrices = { glm::mat4(1), glm::mat4(1), glm::mat4(1)};

	}

	void InstancedRenderer::UpdateInstancePositions(std::vector<glm::mat4> instanceMatrices)
	{
		instanceMatrices = instanceMatrices;
	}

	
	void InstancedRenderer::Create()
	{
		CompileShaders();

		std::vector<GLfloat> VBOData;
		
		// Extract Renderables from underlying Game Object
		// and accumulate vertex data as single entity
		for(auto &renderable : instance->GetRenderables())
		{
			auto numberVerticesInPrimative = renderable->GetNumberOfVertices();
			if( numberVerticesInPrimative != 3) // use only tringles
				continue;

			totalNumberOfVertices += numberVerticesInPrimative;

			auto primativeVertexData = renderable->GetVertexData();
			VBOData.insert(VBOData.end(), primativeVertexData.begin(), primativeVertexData.end());
		}
			
		// Define the VBO for the InstancedRenderer

		auto posNormColTex_Interleaved = [&]()
		{	
			auto sizeOfVerticesData = VBOData.size() * sizeof(GLfloat);
			auto bufferSize = sizeOfVerticesData;

			// Create a VBO to store vertex data within
			VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
			VBO.Bind();

			// Upload the vertex data
			VBO.UploadData(VBOData.data());
	
			int numComponentsPerAttribute = 3;
			int componentType = GL_FLOAT;
			const void *startOfData = 0;
	
			VertexAttribute positionAttribute;
			positionAttribute.Create(0, GL_FLOAT, 3, 11 * sizeof(GLfloat));
			positionAttribute.Enable((GLvoid*)0);

			VertexAttribute normalAttribute;
			normalAttribute.Create(1, GL_FLOAT, 3, 11 * sizeof(GLfloat));
			normalAttribute.Enable((const void*)(3 * sizeof(GLfloat)));

			VertexAttribute colorAttribute;
			colorAttribute.Create(2, GL_FLOAT, 3, 11 * sizeof(GLfloat));
			colorAttribute.Enable((const void*)(6 * sizeof(GLfloat)));					

			VertexAttribute textureCoordinatesAttribute;
			textureCoordinatesAttribute.Create(3, GL_FLOAT, 2,  11 * sizeof(GLfloat));
			textureCoordinatesAttribute.Enable((const void*)(9 * sizeof(GLfloat)));

			VBO.Release();		

		};

		auto posTexNormCol_Interleaved = [&]()
		{	
			auto sizeOfVerticesData = VBOData.size() * sizeof(GLfloat);
			auto bufferSize = sizeOfVerticesData;

			// Create a VBO to store vertex data within
			VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
			VBO.Bind();

			// Upload the vertex data
			VBO.UploadData(VBOData.data());
	
			int numComponentsPerAttribute = 3;
			int componentType = GL_FLOAT;
			const void *startOfData = 0;
	
			VertexAttribute positionAttribute;
			positionAttribute.Create(0, GL_FLOAT, 3, 11 * sizeof(GLfloat));
			positionAttribute.Enable((GLvoid*)0);

			VertexAttribute textureCoordinatesAttribute;
			textureCoordinatesAttribute.Create(1, GL_FLOAT, 2,  11 * sizeof(GLfloat));
			textureCoordinatesAttribute.Enable((const void*)(3 * sizeof(GLfloat)));

			VertexAttribute normalAttribute;
			normalAttribute.Create(2, GL_FLOAT, 3, 11 * sizeof(GLfloat));
			normalAttribute.Enable((const void*)(5 * sizeof(GLfloat)));

			VertexAttribute colorAttribute;
			colorAttribute.Create(3, GL_FLOAT, 3, 11 * sizeof(GLfloat));
			colorAttribute.Enable((const void*)(8 * sizeof(GLfloat)));					

			

			VBO.Release();	
		};
		
		// Use Vertex Array Object and pass in a macro recording
		switch(shaderProgram->vertexLayout)
		{
			case VertexLayout::PosNormColTex:
					VAO.Record(posNormColTex_Interleaved);
					break;
			case VertexLayout::PosTexNormCol:
					VAO.Record(posTexNormCol_Interleaved);
					break;
		}

		// Append the last Vertex Attribute - colour
		// Append more...using a diffirent VBO but the VertexArrayBuffer will just use this pointer instead of above VBO
			

		instanceColoursArrayVBO.Create(instanceColours.size() * sizeof(glm::vec3), GL_ARRAY_BUFFER, false);
		instanceColoursArrayVBO.Bind();
		instanceColoursArrayVBO.UploadData(instanceColours.data());

		instanceViewMatricesArrayVBO.Create(instanceMatrices.size() * sizeof(glm::mat4), GL_ARRAY_BUFFER, false);
		instanceViewMatricesArrayVBO.Bind();
		instanceViewMatricesArrayVBO.UploadData(instanceMatrices.data());		
		
		VAO.Record([&]()
		{			
			// vec3 - colour
			instanceColoursArrayVBO.Bind();
			std::size_t vec3_size = sizeof(glm::vec3);
	
			auto stride = vec3_size;
			VertexAttribute instanceColourVertexAttribute;
			instanceColourVertexAttribute.Create(4, GL_FLOAT, 3, stride);
			instanceColourVertexAttribute.Enable((void*)0);

			glVertexAttribDivisor(4, 1);

			// Mat4:
			instanceViewMatricesArrayVBO.Bind();
			
			std::size_t vec4_size = sizeof(glm::vec4);
			stride = sizeof(glm::mat4);

			VertexAttribute instanceMatricesAttribute1;
			instanceMatricesAttribute1.Create(5, GL_FLOAT, 4, stride);
			instanceMatricesAttribute1.Enable((void*)0);

			VertexAttribute instanceMatricesAttribute2;
			instanceMatricesAttribute2.Create(6, GL_FLOAT, 4, stride);
			instanceMatricesAttribute2.Enable( (void *)(vec4_size * 1));

			VertexAttribute instanceMatricesAttribute3;
			instanceMatricesAttribute3.Create(7, GL_FLOAT, 4, stride);
			instanceMatricesAttribute3.Enable( (void *)(vec4_size * 2));

			VertexAttribute instanceMatricesAttribute4;
			instanceMatricesAttribute4.Create(8, GL_FLOAT, 4, stride);
			instanceMatricesAttribute4.Enable( (void *)(vec4_size * 3));

			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);
			glVertexAttribDivisor(8, 1);

		});
		
		instanceColoursArrayVBO.Release();
		instanceViewMatricesArrayVBO.Release();
		

		isCreated = true;

	}


	void InstancedRenderer::Render()
	{
		auto amount = (GLsizei) instanceColours.size();
		VAO.Use();
			glDrawArraysInstanced(GL_TRIANGLES, 0, GetNumberOfVertices(), amount);
		VAO.Release();

	}

	size_t InstancedRenderer::GetNumberOfVertices()
	{
		return totalNumberOfVertices;
	}
	
	bool InstancedRenderer::CompileShaders()
	{
		shaderProgram = std::make_shared<stu::ShaderProgram>(stu::VertexLayout::PosTexNormCol);
		shaderProgram->CopyAddShader(stu::Shader("stu\\instance.vert", "With instnace Vertex Shader", GL_VERTEX_SHADER));
		shaderProgram->CopyAddShader(stu::Shader("stu\\instance.frag", "With instance Interim Fragment Shader", GL_FRAGMENT_SHADER));

		if (!shaderProgram->Link())
		{
			std::cout << "Failed to compile interim shader program" << std::endl;
			return false;
		}
		return true;
	}
}