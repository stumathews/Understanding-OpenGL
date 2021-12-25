#include "pch.h"
#include "Triangle.h"
#include "Common.h"
#include "VertexAttribute.h"
#include <iostream>
#include <fstream>
#include "Line.h"
#include "StuTexture.h"

using namespace glm;
namespace stu 
{

	glm::vec3 Triangle::CalculateVertxNormal(glm::vec3 myVertex, glm::vec3 mySurfacesNormal, std::vector<std::shared_ptr<stu::Triangle>> faces)
	{
		std::vector<std::shared_ptr<stu::Triangle>> otherFaces;
		glm::vec3 averageNormal;

		for (auto& other : faces)
		{
			if (other->lv == myVertex) {
				otherFaces.push_back(other);
				averageNormal += other->face_normal;
				continue;
			}
			if (other->rv == myVertex) {
				otherFaces.push_back(other);
				averageNormal += other->face_normal;
				continue;
			}
			if (other->tv == myVertex) {
				otherFaces.push_back(other);
				averageNormal += other->face_normal;
				continue;
			}
		}

		averageNormal /= otherFaces.size();
		return averageNormal;
	}

	// Create a unit triangle by default or any other if values specified.

	Triangle::Triangle(glm::vec3 leftVertex,
		glm::vec3 rightVertex,
		glm::vec3 topVertex, 
		glm::vec3 leftVertexColour, 
		glm::vec3 rightVertexColour, 
		glm::vec3 topVertexColour, 
		glm::vec3 leftVertexNormal,
		glm::vec3 rightVertexNormal, 
		glm::vec3 topVertexNormal,
		glm::vec2 leftVertexTextCoord, 
		glm::vec2 rightVertexTextCoord,
		glm::vec2 topVertexTextCoord)
	{
		SetVertices(
			leftVertex, rightVertex, topVertex, 
			leftVertexColour, rightVertexColour, topVertexColour,
			leftVertexNormal, rightVertexNormal, topVertexNormal, 
			leftVertexTextCoord, rightVertexTextCoord, topVertexTextCoord);

		this->VAO = VertexArrayObject();
		this->VBO = VertexBufferObject();

		UpdateData();
	}

	void Triangle::SetVertices(glm::vec3 leftVertex = glm::vec3( -1.0f, 0.0f, 0.0f ), 
		glm::vec3 rightVertex = glm::vec3( 1.0f, 0.0f, 0.0f ),
		glm::vec3 topVertex = glm::vec3( 0.0f, 1.0f, 0.0f ),
		glm::vec3 leftVertexColour = glm::vec3( 0.0f, 1.0f, 0.0f ),
		glm::vec3 rightVertexColour = glm::vec3( 1.0f, 0.0f, 0.0f ), 
		glm::vec3 topVertexColour = glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3 leftVertexNormal = glm::vec3( 0.0f, 0.0f, 0.0f), 
		glm::vec3 rightVertexNormal = glm::vec3( 0.0f, 0.0f, 0.0f), 
		glm::vec3 topVertexNormal = glm::vec3( 0.0f, 0.0f, 0.0f),
		glm::vec2 leftVertexTextCoord = glm::vec2(0.0f, 0.0f), 
		glm::vec2 rightVertexTextCoord = glm::vec2(1.0f, 0.0f),
		glm::vec2 topVertexTextCoord = glm::vec2(0.5f, 0.5f))
	{
		this->lv = leftVertex;
		this->rv = rightVertex;
		this->tv = topVertex;
		this->lvc = leftVertexColour;
		this->rvc = rightVertexColour;
		this->tvc = topVertexColour;		
		this->lvn = leftVertexNormal;
		this->rvn = rightVertexNormal;
		this->tvn = topVertexNormal;
		this->lvtc = leftVertexTextCoord;
		this->rvtc = rightVertexTextCoord;
		this->tvtc = topVertexTextCoord;
	}

	void Triangle::setTexture(std::shared_ptr<stu::Texture> texture, std::shared_ptr<stu::ShaderProgram> shaderProgram, int sampler)
	{
		this->texture = texture;
		this->useTexture = true;
		this->shaderProgram = shaderProgram;
		this->textureSampler = sampler;
	}

	//temp:
	void Triangle::SetShaderProgram(std::shared_ptr<stu::ShaderProgram> shaderProgram)
	{
		this->shaderProgram = shaderProgram;
	}

	void Triangle::UpdateData(bool useFaceNormalsForAll)
	{
		if (useFaceNormalsForAll)
		{

			/*
				Triangle Vertex Model
			     tv
			     /\
			    /  \
             q /    \  
			  /      \
			 lv      rv
			 ----p-----

			 lv = left vertex
			 rv = right vertex
			 tv = top vertex

			*/
			glm::vec3 p, q;

			p = rv - lv;
			q = tv - lv;

			face_normal = glm::normalize(glm::cross(p, q));

			lvn = face_normal; // left
			rvn = face_normal; // right
			tvn = face_normal; // top
		}

		coords = {
			lvtc.x, lvtc.y, // left texture coordinates
			rvtc.x, rvtc.y, // right texture coordinates
			tvtc.x, tvtc.y // top texture coordinates
		};

		vertices = {
			lv.x, lv.y, lv.z, // left vertex
			rv.x, rv.y, rv.z, // right vertex
			tv.x, tv.y, tv.z  // top vertex
		};

		colours = {
			lvc.x, lvc.y, lvc.z, // left vertex colour
			rvc.x, rvc.y, rvc.z, // right vertex colour
			tvc.x, tvc.y, tvc.z // top vertex colour
		};

		normals = {
			lvn.x, lvn.y, lvn.z, //left vertex normal
			rvn.x, rvn.y, rvn.z, //right vertex normal
			tvn.x, tvn.y, tvn.z, //top vertex normal
		};

		vertices_interleaved =
		{
			lv.x, lv.y, lv.z, lvn.x, lvn.y, lvn.z, lvc.x, lvc.y, lvc.z, lvtc.x, lvtc.y, // left vertex data
			rv.x, rv.y, rv.z, rvn.x, rvn.y, rvn.z, rvc.x, rvc.y, rvc.z, rvtc.x, rvtc.y, // right vertex data
			tv.x, tv.y, tv.z, tvn.x, tvn.y, tvn.z, tvc.x, tvc.y, tvc.z, tvtc.x, tvtc.y, // top vertex data
		};

		vertices_interleaved_mainshader =
		{
			lv.x, lv.y, lv.z, lvtc.x, lvtc.y, lvn.x, lvn.y, lvn.z, lvc.x, lvc.y, lvc.z, // left vertex data
			rv.x, rv.y, rv.z, rvtc.x, rvtc.y, rvtc.y, rvn.y, rvn.z, rvc.x, rvc.y, rvc.z, // right vertex data
			tv.x, tv.y, tv.z, tvtc.x, tvtc.y, tvtc.y, tvn.y, tvn.z, tvc.x, tvc.y, tvc.z, // top vertex data
		};
	}

	void Triangle::UpdateVertexNormals(std::vector<std::shared_ptr<stu::Triangle>> faces)
	{
		lvn = CalculateVertxNormal(lv, face_normal, faces);
		rvn = CalculateVertxNormal(rv, face_normal, faces);
		tvn = CalculateVertxNormal(tv, face_normal, faces);

		UpdateData(false);
	}

	void Triangle::CreateAnd(vec3 translate, vec3 scale, vec3 rotate, float angle)
	{	
		Rotate(rotate.x, rotate.y, rotate.z, angle);
		CreateAnd(translate, scale);	
	}

	std::ostream &operator<<(std::ostream &stream, glm::vec3 &vec)
	{
		stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
		return stream;
	}

	std::ostream &operator<<(std::ostream &stream, glm::vec2 &vec)
	{
		stream << "(" << vec.x << ", " << vec.y  << ")";
		return stream;
	}

	void Triangle::CreateNormalLines()
	{
	
		auto centoid = (lv + rv + tv) / 3.0f;
		lvNormalLine = std::make_shared<stu::Line>(lv, lv);
		rvNormalLine = std::make_shared<stu::Line>(rv, rv);
		tvNormalLine = std::make_shared<stu::Line>(tv, tv);
		zeroNormalLine = std::make_shared<stu::Line>(glm::vec3(0.0, 0.0, 0.0), face_normal);	
		centoidNormalLine = std::make_shared<stu::Line>(centoid, centoid + face_normal, glm::vec3(0.0f, 255.0f, 0.0f));
		
		UpdateNormalLines();
		
		lvNormalLine->Create();
		rvNormalLine->Create();
		tvNormalLine->Create();
		zeroNormalLine->Create();
		centoidNormalLine->Create();

		isNormalLinesCreated = true;
	}

	void Triangle::Create(std::string face = "no face")
	{	
		// Its possible that something like stu::Square might try to call Create() on its underlying triangle even if the caller
		// has already done this. Lets ensure either case works and its not created twice in the latter case.
		if(isCreated)
			return;
										
		// Records the steps nessesary to setup data data in vertex before rendering
		auto posCol_Blocks = [&]()
		{	
			auto sizeOfVerticesData = vertices.size() * sizeof(GLfloat);
			auto sizeOfColourData = colours.size() * sizeof(GLfloat);
			auto bufferSize = sizeOfVerticesData + sizeOfColourData;

			// Create a VBO to store vertex data within
			VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
			VBO.Bind();

			// Upload the vertex data
			VBO.UploadPartionedData(0, sizeOfVerticesData, vertices.data());
			VBO.UploadPartionedData(sizeOfVerticesData, sizeOfColourData, colours.data());

	
			int numComponentsPerAttribute = 3;
			int componentType = GL_FLOAT;
			const void *startOfData = 0;
			const int stride = 0; // tightly packed
		
			// Enable and Set the vertex attribute 0
			VertexAttribute positionAttribute;
			positionAttribute.Create(0, componentType, numComponentsPerAttribute, stride);
			positionAttribute.Enable(startOfData);

			// Enable and Set the vertex attribute 1
			startOfData = (const void*)(sizeof(vertices));
			VertexAttribute colourAttribute;
			colourAttribute.Create(1, componentType, numComponentsPerAttribute, stride);
			colourAttribute.Enable(startOfData);
			VBO.Release();
		};

		auto posNormCol_Blocks = [&]()
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

		auto posNormColTex_Blocks = [&]()
		{	
			auto sizeOfVerticesData = vertices.size() * sizeof(GLfloat);
			auto sizeOfNormalsData = normals.size() * sizeof(GLfloat);
			auto sizeOfColourData = colours.size() * sizeof(GLfloat);
			auto sizeOfCoordsData = coords.size() * sizeof(GLfloat);
	
			auto bufferSize = sizeOfVerticesData + sizeOfNormalsData + sizeOfColourData + sizeOfCoordsData;

			// Create a VBO to store vertex data within
			VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
			VBO.Bind();

			// Upload the vertex data
			VBO.UploadPartionedData(0, sizeOfVerticesData, vertices.data());
			VBO.UploadPartionedData(sizeOfVerticesData, sizeOfNormalsData, normals.data());
			VBO.UploadPartionedData(sizeOfVerticesData + sizeOfNormalsData, sizeOfColourData, colours.data());
			VBO.UploadPartionedData(sizeOfVerticesData + sizeOfNormalsData + sizeOfColourData, sizeOfCoordsData, coords.data());

	
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

			// Enable and Set the vertex attribute 3 - coords
			startOfData = (const void*)(sizeOfVerticesData + sizeOfNormalsData + sizeOfColourData);
			VertexAttribute textureCoordinatesAttribute;
			textureCoordinatesAttribute.Create(3, GL_FLOAT, 2, stride);
			textureCoordinatesAttribute.Enable(startOfData);

			VBO.Release();
		};

		auto posCol_Interleaved = [&]()
		{	
			auto sizeOfVerticesData = vertices_interleaved.size() * sizeof(GLfloat);
			auto bufferSize = sizeOfVerticesData;

			// Create a VBO to store vertex data within
			VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
			VBO.Bind();

			// Upload the vertex data
			VBO.UploadData(vertices_interleaved.data());
	
			int numComponentsPerAttribute = 3;
			int componentType = GL_FLOAT;
			const void *startOfData = 0;
		
			// Enable and Set the vertex attribute 0 - vertices positions
			VertexAttribute positionAttribute;
			positionAttribute.Create(0, GL_FLOAT, 3, 6 * sizeof(GLfloat));
			positionAttribute.Enable(startOfData);

			// Enable and Set the vertex attribute 1 - vertices colours
			VertexAttribute colourAttribute;
			colourAttribute.Create(1, GL_FLOAT, 3, 6 * sizeof(GLfloat));
			colourAttribute.Enable((const void*)(3 * sizeof(GLfloat)));

			VBO.Release();
		};

		auto posNormCol_Interleaved = [&]()
		{	
			auto sizeOfVerticesData = vertices_interleaved.size() * sizeof(GLfloat);
			auto bufferSize = sizeOfVerticesData;

			// Create a VBO to store vertex data within
			VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
			VBO.Bind();

			// Upload the vertex data
			VBO.UploadData(vertices_interleaved.data());

	
			int numComponentsPerAttribute = 3;
			int componentType = GL_FLOAT;
			const void *startOfData = 0;
		
			// Enable and Set the vertex attribute 0 - vertices positions
			VertexAttribute positionAttribute;
			positionAttribute.Create(0, GL_FLOAT, 3, 9 * sizeof(GLfloat));
			positionAttribute.Enable(startOfData);

			// Enable and Set the vertex attribute 1 - vertices normals
			VertexAttribute normalAttribute;
			normalAttribute.Create(1, GL_FLOAT, 3, 9 * sizeof(GLfloat));
			normalAttribute.Enable((const void*)(3 * sizeof(GLfloat)));

			// Enable and Set the vertex attribute 2 - vertices colours
			VertexAttribute colourAttribute;
			normalAttribute.Create(2, GL_FLOAT, 3, 9 * sizeof(GLfloat));
			normalAttribute.Enable((const void*)(6 * sizeof(GLfloat)));

			VBO.Release();
		};

		auto posNormColTex_Interleaved = [&]()
		{	
			auto sizeOfVerticesData = vertices_interleaved.size() * sizeof(GLfloat);
			auto bufferSize = sizeOfVerticesData;
			vertexData = vertices_interleaved;

			// Create a VBO to store vertex data within
			VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
			VBO.Bind();

			// Upload the vertex data
			VBO.UploadData(vertexData.data());
	
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
			auto sizeOfVerticesData = vertices_interleaved_mainshader.size() * sizeof(GLfloat);
			auto bufferSize = sizeOfVerticesData;
			vertexData = vertices_interleaved_mainshader;

			// Create a VBO to store vertex data within
			VBO.Create(bufferSize, GL_ARRAY_BUFFER, true);
			VBO.Bind();

			// Upload the vertex data
			VBO.UploadData(vertexData.data());
	
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
					
		if(drawNormals)
			CreateNormalLines();

		if(isUsingDeiganostics)
			PrintVertices(face);
		
		isCreated = true;
	}

	void Triangle::PrintVertices(std::string& face)
	{
		std::ofstream output("example.txt", std::ios::app);
		if (output.is_open())
		{
			output << std::endl << face << " :" << std::endl;
			output << "v " << lv << " # left" << std::endl;
			output << "v " << rv << " # right" << std::endl;
			output << "v " << tv << " # top" << std::endl;
			output << "vn " << lvn << " # left" << std::endl;
			output << "vn " << rvn << " # right" << std::endl;
			output << "vn " << tvn << " # top" << std::endl;
			output << "vt" << lvtc << " # left" << std::endl;
			output << "vt" << rvtc << " # right" << std::endl;
			output << "vt" << tvtc << " # top" << std::endl;

		}
		output.close();
	}

	void Triangle::RenderNormals()
	{
		shaderProgram->EnableUseTextureUniform(false);
		if(drawNormals && isNormalLinesCreated)
		{
			lvNormalLine->Render();
			rvNormalLine->Render();
			tvNormalLine->Render();
			centoidNormalLine->Render();
		}
		shaderProgram->EnableUseTextureUniform(true);
	}




	// Inherited via IPerFrame


	/// <summary>
	/// Set Diagnostics mode/debugging
	/// </summary>
	/// <param name="yesno"></param>

	void Triangle::SetDiganostics(bool yesno)
	{
		this->isUsingDeiganostics = yesno;
	}

	void Triangle::Create()
	{
		Create("default face"); 
	}

	void Triangle::Update(float delta)
	{
		UpdateNormalLines();
	}


	void Triangle::Render()
	{
		if(useTexture) 
		{
			shaderProgram->EnableUseTextureUniform(true);
			texture->Use(shaderProgram->GetId(), shaderProgram->GetSamplerName(), textureSampler);
		} else 
		{
			if(shaderProgram)
				shaderProgram->EnableUseTextureUniform(false);
		}

		if(ShouldDrawNormals())
			RenderNormals();

		// Just in case
		if(!isCreated)
			return;

		if(ShouldDisableBackFaceCulling())
			glDisable(GL_CULL_FACE);


		UsePerFrameOptions();
		VAO.Use();
		glDrawArrays(GL_TRIANGLES, 0, GetNumberOfVertices());
		VAO.Release();

		// disabled it if we enabled it 
		if(ShouldDisableBackFaceCulling())
			glEnable(GL_CULL_FACE);
	
	}

	void Triangle::UpdateNormalLines()
	{
		if(!drawNormals || !isNormalLinesCreated)
			return;

		auto factor = 0.1f;
		auto centoid = (lv + rv + tv) / 3.0f;
		
		bool useFacesAverage = false;
			
		auto lvend = lvn * factor;
		auto rvend = rvn * factor;		
		auto tvend = tvn * factor;

		if(!useFacesAverage)
		{
			lvend = lv + face_normal * factor;
			rvend = rv + face_normal * factor;
			tvend = tv + face_normal * factor;
		}
		else
		{
			lvend = lv + glm::vec3(lv.x, lv.y, face_normal.z);
			rvend = rv + glm::vec3(rv.x, rv.y, face_normal.z);
			tvend = tv + glm::vec3(tv.x, tv.y, face_normal.z);
		}

		
		lvNormalLine->Set(lv, lvend);
		rvNormalLine->Set(rv, rvend);
		tvNormalLine->Set(tv, tvend);
		zeroNormalLine->Set(glm::vec3(0.0, 0.0, 0.0), face_normal);
		centoidNormalLine->Set(centoid, centoid + face_normal * factor, glm::vec3(0.0f, 255.0f, 0.0f));
	}

	

	void Triangle::Translate(float x, float y, float z)
	{
		mat4 trans = glm::translate(mat4(1), glm::vec3(x, y, z));

		//this->lv = trans * vec4(this->lv, 1); // Note need to use vec4 with w = 1
		//this->rv = trans * vec4(this->rv, 1);
		//this->tv = trans * vec4(this->tv, 1);
		UpdateData();

	}

	void Triangle::Scale(float x, float y, float z)
	{
		glm::mat4 scale = glm::scale(glm::mat4(1), vec3(x, y, z));
		//this->lv = scale * vec4(this->lv, 1); // Note need to use vec4 with w = 1
		//this->rv = scale * vec4(this->rv, 1);
		//this->tv = scale * vec4(this->tv, 1);
		UpdateData();
	}

	void Triangle::Rotate(float x, float y, float z, float deg)
	{
		glm::mat4 rot = glm::rotate(glm::mat4(1), glm::radians(deg), vec3(x, y, z));
		//this->lv = rot * vec4(this->lv, 1); // Note need to use vec4 with w = 1
		//this->rv = rot * vec4(this->rv, 1);
		//this->tv = rot * vec4(this->tv, 1);
		UpdateData();
	}

	void Triangle::CreateAnd(vec3 translate, vec3 scale)
	{
		Translate(translate.x, translate.y, translate.z);
		Scale(scale.x, scale.y, scale.z);	
		Create("default face");
	}
}
