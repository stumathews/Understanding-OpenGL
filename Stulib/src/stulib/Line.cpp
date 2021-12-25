#include "pch.h"
#include "Line.h"
#include "PerFrame.h"
#include "Renderable.h"
#include "Common.h"
#include "VertexArrayObject.h"
#include "VertexBufferObj.h"
#include <vector>
#include "VertexAttribute.h"


void stu::Line::Set(glm::vec3 start, glm::vec3 end, glm::vec3 colour) 
{ 	
	Initialize(start, end, colour);
}

stu::Line::Line(glm::vec3 start, glm::vec3 end, glm::vec3 color)
{
	Initialize(start, end, color);
}

void stu::Line::SetColour(glm::vec3 &color)
{
	// start and end colours for the line
	colours = { color.r, color.g, color.b, color.r, color.g, color.b };
}

void stu::Line::Create()
{
	auto posNormColBlocks = [&]()
	{	
		auto sizeOfVerticesData = vertices.size() * sizeof(GLfloat);
		auto sizeOfNormalsData = normals.size() * sizeof(GLfloat);
		auto sizeOfColourData = colours.size() * sizeof(GLfloat);
	
		auto bufferSize = sizeOfVerticesData + sizeOfNormalsData + sizeOfColourData;

		// Create a VBO to store vertex data within
		VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
		VBO.Bind();

		// Upload the vertex data
		VBO.UploadPartionedData(0, sizeOfVerticesData, vertices.data());
		VBO.UploadPartionedData(sizeOfVerticesData, sizeOfNormalsData, normals.data());
		VBO.UploadPartionedData(sizeOfVerticesData + sizeOfNormalsData, sizeOfColourData, colours.data());

	
		int numComponentsPerAttribute = 3;
		int componentType = GL_FLOAT;
		const void *startOfData = 0;
		const int stride = 0; // tightly packed
		
		// Enable and Set the vertex attribute 0 - vertices
		VertexAttribute positionAttribute;
		positionAttribute.Create(0, componentType, numComponentsPerAttribute, stride);
		positionAttribute.Enable(startOfData);

		// Enable and Set the vertex attribute 1 - normals
		startOfData = (const void*)(sizeOfVerticesData);
		VertexAttribute normalAttribute;
		normalAttribute.Create(1, componentType, numComponentsPerAttribute, stride);
		normalAttribute.Enable(startOfData);

		// Enable and Set the vertex attribute 2 - colours
		startOfData = (const void*)(sizeOfVerticesData + sizeOfNormalsData);
		VertexAttribute colourAttribute;
		colourAttribute.Create(2, componentType, numComponentsPerAttribute, stride);
		colourAttribute.Enable(startOfData);
		VBO.Release();
	};

	// Use Vertex Array Object and pass in a macro recording
	VAO.Record(posNormColBlocks);
}

void stu::Line::Initialize(glm::vec3& start, glm::vec3& end, glm::vec3 color)
{
	// Store where the line will begin and end
	vertices.clear();
	vertices.push_back(start.x);
	vertices.push_back(start.y);
	vertices.push_back(start.z);

	vertices.push_back(end.x);
	vertices.push_back(end.y);
	vertices.push_back(end.z);

	// start and end colours
	SetColour(color);
}


void stu::Line::Render()
{		
	UsePerFrameOptions();
	glLineWidth(5);
	VAO.Use();
		glDrawArrays(GL_LINES, 0, 3);
	VAO.Release();
	glLineWidth(1);
}

void stu::Line::Update(float delta)
{
}
