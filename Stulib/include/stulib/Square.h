#pragma once
#include "Triangle.h"
#include "ShaderProgram.h"

namespace stu
{
	/*
		A square with texture support. Can merge to Triangles to form one single polygon ie a square:
			B---C
			| \ | 
			A---D
	*/
	class Square: public Renderable
	{
	private:		
		Square(stu::Triangle ADB, stu::Triangle DCB);

		// Draw a normal line from each vertex.
		std::shared_ptr<stu::Line> ANormalLine;
		std::shared_ptr<stu::Line> BNormalLine;
		std::shared_ptr<stu::Line> CNormalLine;
		std::shared_ptr<stu::Line> DNormalLine;

		// Draw a normal line in the center of the square.
		std::shared_ptr<stu::Line> centoidNormalLine;

		bool isUsingDiagnostics = false;

	public:

		// A square is composed of two underlying triangles
		Triangle ADB;
		Triangle DCB;

		/* The basis of our square of that of the 4 vertices points ADBC:
		
			Square Vertex Model:

			B---C
			| \ | 
			A---D

		*/
		glm::vec3 A;
		glm::vec3 D;
		glm::vec3 C;
		glm::vec3 B;

		// Texture co-ordinates for each vertex a, d, c and b
		glm::vec2 tca = glm::vec2(0.0f, 0.0f); // texture coorindate for A
		glm::vec2 tcd = glm::vec2(1.0f, 0.0f); // texture coorindate for D
		glm::vec2 tcc = glm::vec2(1.0f, 1.0f); // texture coorindate for C
		glm::vec2 tcb = glm::vec2(0.0f, 1.0f); // texture coorindate for B

		

		/// <summary>
		/// The texture to be stetched over the square's 4 vertices
		/// </summary>
		std::shared_ptr<Texture> texture;

		int sampler = 0;

		/// <summary>
		/// The shader program for this Squre
		/// </summary>
		std::shared_ptr<ShaderProgram> shaderProgram;

		// Create a square using an initial point at bottom left i.e A in ADCB
		Square(glm::vec3 bottLeft, float width, float height);

		Square(glm::vec3 bottLeft, float width, float height, std::shared_ptr<ShaderProgram> shaderProgram);

		Square(glm::vec3 bottLeft, float width, float height, std::shared_ptr<ShaderProgram> shaderProgram, std::shared_ptr<stu::Texture> texture, int sampler = 0);

		void CreateInternalTriangles(glm::vec3 &A, glm::vec3 &D, glm::vec3 &B, glm::vec3 &C);
				
		// Create a square using two existing triangles, effectively merging their shared vertices
		Square(stu::Triangle ADB, stu::Triangle DCB, std::shared_ptr<Texture> texture, std::shared_ptr<ShaderProgram> shaderProgram, int sampler = 0);

		// Create square without a texture
		Square(stu::Triangle ADB, stu::Triangle DCB, std::shared_ptr<ShaderProgram> shaderProgram);

		// Set ecture co-ordinates maually to something other than the defaults. tc = (t)exture (c)oordinate
		void SetTextureCoordinates(
			/*texture coordiante for A */
			glm::vec2 tca = glm::vec2(0.0f, 0.0f),
			/*texture coordiante for D */
			glm::vec2 tcd = glm::vec2(1.0f, 0.0f), 
			/*texture coordiante for C */
			glm::vec2 tcc = glm::vec2(1.0f, 1.0f),
			/*texture coordiante for B */
			glm::vec2 tcb = glm::vec2(0.0f, 1.0f));

		// Debugging 
		void PrintVertices(std::string& face, stu::Triangle& a, stu::Triangle& d, stu::Triangle& c, stu::Triangle& b);

		/// <summary>
		/// Should we render our associated texture or not?
		/// </summary>
		/// <param name="yesno"></param>
		void ShouldUseTexture(bool yesno);

		/// <summary>
		/// Construct the normal lines
		/// </summary>
		/// <param name="a"></param>
		/// <param name="b"></param>
		/// <param name="c"></param>
		/// <param name="d"></param>
		void CreateNormalLines(stu::Triangle& a, stu::Triangle& b, stu::Triangle& c, stu::Triangle& d);

		// Create square vertices. Provide a name for this square/face
		void Create(std::string face);

		// Inherited via Renderable
		virtual void Create() override { Create("default face"); }		
		virtual void Update(float delta) override;
		virtual void Render() override;
		virtual size_t GetNumberOfVertices() override;


		// Drawing code for rendering the normal lines
		void RenderNormalLines();

		// Draw each triangle sperately? By default we dont don't do that and just use the triangles vertices
		bool singlePolygon = false;

		// Texture support
		bool useTexture = false;
	};
}