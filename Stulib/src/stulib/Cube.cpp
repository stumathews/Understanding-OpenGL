#include "pch.h"
#include "Cube.h"
#include "VertexAttribute.h"
namespace stu
{
	void Cube::Create()
	{
		auto rendering_macro_blocks = [&]()
		{	
			auto sizeOfVerticesData = vertices.size() * sizeof(GLfloat);
			auto bufferSize = sizeOfVerticesData;

			// Create a VBO to store vertex data within
			VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
			VBO.Bind();

			// Upload the vertex data
			VBO.UploadData(vertices.data());

	
			int numComponentsPerAttribute = 3;
			int componentType = GL_FLOAT;
			const void *startOfData = 0;
			const int stride = 0; // tightly packed
		
			// Enable and Set the vertex attribute 0
			VertexAttribute positionAttribute;
			positionAttribute.Create(0, componentType, numComponentsPerAttribute,  5 * sizeof(GLfloat));
			positionAttribute.Enable(startOfData);

			/*VertexAttribute colorAttribute;
			positionAttribute.Create(1, GL_FLOAT, 3, 5 * sizeof(GLfloat));
			positionAttribute.Enable((const void*)(3 * sizeof(GLfloat)));*/

			VertexAttribute textureCoordinatesAttribute;
			textureCoordinatesAttribute.Create(2, GL_FLOAT, 2,  5 * sizeof(GLfloat));
			textureCoordinatesAttribute.Enable((const void*)(3 * sizeof(GLfloat)));

			VBO.Release();
		};
		// Use Vertex Array Object and pass in a macro recording
		VAO.Record(rendering_macro_blocks);
	}

	void Cube::Update(float delta)
	{
	}

	void Cube::Render()
	{
		UsePerFrameOptions();
		VAO.Use();
		glDisable(GL_CULL_FACE);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_CULL_FACE);
		VAO.Release();
	}
}