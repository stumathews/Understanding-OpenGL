#pragma once
#include "PerFrame.h"
#include "Renderable.h"
#include "Common.h"
#include "VertexArrayObject.h"
#include "VertexBufferObj.h"
#include <vector>
#include "ShaderProgram.h"
#include "Line.h"
#include "StuTexture.h"

namespace stu
{
	
	/*
		Triangle

		  tv
		 /  \ 
		/    \
	   lv -- rv


	   left vertex, right vertex, top vertex

	*/
	class Triangle : public Renderable
	{
	private:
		glm::vec3 CalculateVertxNormal(glm::vec3 myVertex, glm::vec3 mySurfacesNormal, std::vector<std::shared_ptr<stu::Triangle>> faces);
		
		// Texture support
		std::shared_ptr<stu::Texture> texture;
		std::shared_ptr<stu::ShaderProgram> shaderProgram;
		int textureSampler = 0;

		// Options
		bool useTexture = false; // unless setTexture() is called
		bool isNormalLinesCreated = false;
		bool isUsingDeiganostics = false;
	public:

		// Create a unit triangle by default or any other if values specified.
		Triangle(glm::vec3 leftVertex = glm::vec3( -1.0f, 0.0f, 0.0f ), 
				 glm::vec3 rightVertex = glm::vec3( 1.0f, 0.0f, 0.0f ), 
			     glm::vec3 topVertex = glm::vec3( 0.0f, 1.0f, 0.0f ),
				 //colour 
				 glm::vec3 leftVertexColour = glm::vec3( 0.0f, 1.0f, 0.0f ), 
				 glm::vec3 rightVertexColour = glm::vec3( 1.0f, 0.0f, 0.0f ),
			     glm::vec3 topVertexColour = glm::vec3( 0.0f, 0.0f, 1.0f ),
				 // normal 
				 glm::vec3 leftVertexNormal = glm::vec3( 0.0f, 0.0f, 0.0f),
			     glm::vec3 rightVertexNormal = glm::vec3( 0.0f, 0.0f, 0.0f), 
			     glm::vec3 topVertexNormal = glm::vec3( 0.0f, 0.0f, 0.0f),
			     // text
			     glm::vec2 leftVertexTextCoord = glm::vec2(0.0f, 0.0f), 
			     glm::vec2 rightVertexTextCoord = glm::vec2(1.0f, 0.0f), 
			     glm::vec2 topVertexTextCoord = glm::vec2(0.5f, 0.5f));

		void SetVertices(glm::vec3 leftVertex, glm::vec3 rightVertex, glm::vec3 topVertex, 
						 glm::vec3 leftVertexColour, glm::vec3 rightVertexColour, glm::vec3 topVertexColour, 
						 glm::vec3 leftVertexNormal, glm::vec3 rightVertexNormal, glm::vec3 topVertexNormal, 
			             glm::vec2 leftVertexTextCoord, glm::vec2 rightVertexTextCoord, glm::vec2 topVertexTextCoord);
		
		// Specify the texture to use and the shader program
		void setTexture(
			std::shared_ptr<stu::Texture> texture,
			std::shared_ptr<stu::ShaderProgram> shaderProgram, int sampler = 0);

		// temp:
		void SetShaderProgram(std::shared_ptr<stu::ShaderProgram> shaderProgram);
		bool isCreated = false;

		// Vertex attributes
		
		glm::vec3 lvn = glm::vec3( 0.0f, 1.0f, 0.0f ); //left vertex normal
		glm::vec3 rvn = glm::vec3( 1.0f, 0.0f, 0.0f ); //right vertex normal
		glm::vec3 tvn = glm::vec3(0.0f, 0.0f, 1.0f ); // top vertex normal

		glm::vec3 lv = glm::vec3( -1.0f, 0.0f, 0.0f );  //left vertex
		glm::vec3 rv = glm::vec3( 1.0f, 0.0f, 0.0f );   // right vertex
		glm::vec3 tv = glm::vec3( 0.0f, 1.0f, 0.0f );   // top vertex

		glm::vec3 lvc = glm::vec3( 0.0f, 1.0f, 0.0f ); //left vertex color: Green
		glm::vec3 rvc = glm::vec3( 1.0f, 0.0f, 0.0f ); //right vertex color: Red
		glm::vec3 tvc = glm::vec3( 0.0f, 0.0f, 1.0f ); // top vertex color: Blue

		glm::vec2 lvtc = glm::vec2(0.0f ,0.0f); // left vertex texture coordinate
		glm::vec2 rvtc = glm::vec2(1.0f ,0.0f);  // right vertex texture coordinate
		glm::vec2 tvtc = glm::vec2(0.5f ,0.5f);  // top vertex texture coordinate

		// normal drawing
		std::shared_ptr<stu::Line> lvNormalLine;
		std::shared_ptr<stu::Line> rvNormalLine;
		std::shared_ptr<stu::Line> tvNormalLine;
		std::shared_ptr<stu::Line> zeroNormalLine;
		std::shared_ptr<stu::Line> centoidNormalLine;

		// Update internal data structures if we know some aspects have been changed.
		// Ie update the face normals if we've changed the normal values etc
		void UpdateData(bool useFaceNormalsForAll = true);

		// Storage for vertex attributes to support both interleaved and homogenous layouts
		std::vector<GLfloat> vertices;
		std::vector<GLfloat> colours;
		std::vector<GLfloat> normals;
		std::vector<GLfloat> coords;
		std::vector<GLfloat> vertices_interleaved;
		std::vector<GLfloat> vertices_interleaved_mainshader;
		
		glm::vec3 face_normal;
	
		// Utility functions to modify in-place the vertices at creation time 
		void CreateAnd(glm::vec3 scale, glm::vec3 translate, glm::vec3 rotate, float angle);
		void CreateAnd(glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3 translate = glm::vec3(0.0f, 0.0f, 0.0f));
		
		// Translate the vertices in-place
		void Translate(float x, float y, float z);
		void Scale(float x, float y, float z);
		void Rotate(float x, float y, float z, float degrees);
				
		/// <summary>
		/// Main creation of the VBO
		/// </summary>
		/// <param name="face"></param>
		void Create(std::string face);

		/// <summary>
		/// Diagnostics
		/// </summary>
		/// <param name="face"></param>
		void PrintVertices(std::string& face);

		/// <summary>
		/// Construct Normals lines in VBO
		/// </summary>
		void CreateNormalLines();

		/// <summary>
		/// Update normals lines if normals have changed
		/// </summary>
		void UpdateNormalLines();

		/// <summary>
		/// Update normals if normals with respect to the faces of other surfaces (neighbours)
		/// </summary>
		/// <param name="faces"></param>
		void UpdateVertexNormals(std::vector<std::shared_ptr<stu::Triangle>> faces);

		/// <summary>
		/// Draw the normals on the triangle
		/// </summary>
		void RenderNormals();

		/// <summary>
		/// Set Diagnostics mode/debugging
		/// </summary>
		/// <param name="yesno"></param>
		void SetDiganostics(bool yesno);

		// Inherited via IPerFrame
		virtual void Create() override;
		virtual void Update(float delta) override;
		virtual void Render() override;
	};
}

