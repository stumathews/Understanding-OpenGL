#include "pch.h"
#include "Box.h"
#include "Common.h"
#include <iostream>
#include "VertexAttribute.h"
#include "VertexBufferObj.h"
namespace stu
{
	void Box::Create()
	{
		auto PosColTex = [&](){
		
			// Configure the GPU memory to store the veritices in Normalized Device Coordinates
			GLfloat vertices[] = 
			{ 
			     // Format:  position(x, y, z), color(r,g,b), text coords(x,y)
				 0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
				 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
				-0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left 
			};

			GLint indices[] =
			{
				0, 1, 3, // First Triangle
				1, 2, 3 // Second Triangle
			};


			VBO.Create(sizeof(vertices), GL_ARRAY_BUFFER, false, true);
			EBO.Create(sizeof(indices), GL_ELEMENT_ARRAY_BUFFER, false, true);

			VBO.UploadData(vertices);
			EBO.UploadData(indices);

			VertexAttribute positionAttribute;
			positionAttribute.Create(0, GL_FLOAT, 3, 8 * sizeof(GLfloat));
			positionAttribute.Enable((GLvoid*)0);

			VertexAttribute colorAttribute;
			positionAttribute.Create(1, GL_FLOAT, 3, 8 * sizeof(GLfloat));
			positionAttribute.Enable((const void*)(3 * sizeof(GLfloat)));

			VertexAttribute textureCoordinatesAttribute;
			textureCoordinatesAttribute.Create(2, GL_FLOAT, 2,  8 * sizeof(GLfloat));
			textureCoordinatesAttribute.Enable((const void*)(6 * sizeof(GLfloat)));

			EBO.Release();
			VBO.Release();
		};

		auto PosNormColTex = [&](){
		
			// Configure the GPU memory to store the veritices in Normalized Device Coordinates
			GLfloat vertices[] = 
			{ 
				// position(x, y, z), normal(x,y,z), color(r,g,b), text coords(x,y)
				 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right
				 0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
				-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
				-0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left 
			};

			GLint indices[] =
			{
				0, 1, 3, // First Triangle
				1, 2, 3 // Second Triangle
			};


			VBO.Create(sizeof(vertices), GL_ARRAY_BUFFER, false, true);
			EBO.Create(sizeof(indices), GL_ELEMENT_ARRAY_BUFFER, false, true);

			VBO.UploadData(vertices);
			EBO.UploadData(indices);

			// pos, normal, colour, texcoord
			// 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top right

			VertexAttribute positionAttribute;
			positionAttribute.Create(0, GL_FLOAT, 3, 11 * sizeof(GLfloat));
			positionAttribute.Enable((GLvoid*)0);

			VertexAttribute normalAttribute;
			normalAttribute.Create(1, GL_FLOAT, 3, 11 * sizeof(GLfloat));
			normalAttribute.Enable((const void*)(3 * sizeof(GLfloat)));

			VertexAttribute colorAttribute;
			positionAttribute.Create(2, GL_FLOAT, 3, 11 * sizeof(GLfloat));
			positionAttribute.Enable((const void*)(6 * sizeof(GLfloat)));					

			VertexAttribute textureCoordinatesAttribute;
			textureCoordinatesAttribute.Create(3, GL_FLOAT, 2,  11 * sizeof(GLfloat));
			textureCoordinatesAttribute.Enable((const void*)(9 * sizeof(GLfloat)));

			EBO.Release();
			VBO.Release();

		};

		auto PosTextNormCol = [&]()
		{			
		
			GLfloat vertices[] =
			{
				// position(x, y, z),  text coords(x,y) normal(x,y,z), color(r,g,b),
				A.x, A.y, A.z, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, // bottom left
				D.x, D.y, D.z, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, // bottom right
				C.x, C.y, C.z, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,  // top right
				B.x, B.y, B.z, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,   // top left 
			};

			GLint indices[] =
			{
				0, 1, 3, // First Triangle (ADB)
				1, 2, 3 // Second Triangle (DCB)
			};

			VBO.Create(sizeof(vertices), GL_ARRAY_BUFFER, false, true);
			EBO.Create(sizeof(indices), GL_ELEMENT_ARRAY_BUFFER, false, true);

			VBO.UploadData(vertices);
			EBO.UploadData(indices);


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
			


			EBO.Release();
			VBO.Release();

		};

		switch(shaderProgram->vertexLayout)
		{
			case VertexLayout::PosNormColTex:
				VAO.Record(PosNormColTex);
				break;
			case VertexLayout::PosTexNormCol:
				VAO.Record(PosTextNormCol);
				break;
		}		
	}

	Box::Box(std::shared_ptr<stu::ShaderProgram> shaderProgram, glm::vec3 A, glm::vec3 D, glm::vec3 C, glm::vec3 B)
	{
		this->A = A;
		this->D = D;
		this->C = C;
		this->B = B;
		this->shaderProgram = shaderProgram;		
	}

	void Box::Update(float delta) { }

	void Box::Render()
	{		
		VAO.Use();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT , 0);	
		VAO.Release();
	}
}
