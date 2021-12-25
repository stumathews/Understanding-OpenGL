#pragma once

#include "PerFrameOptions.h"
#include "PerFrame.h"
#include "VertexArrayObject.h"
#include "VertexBufferObj.h"
#include <string>
#include <map>

namespace stu 
{
	/// <summary>
	/// All objects that can be drawn should inherit from Renderable
	/// </summary>
	class Renderable : public IPerFrame
	{
	protected:

		/// <summary>
		/// Each Renderable object has access to its own VAO
		/// </summary>
		VertexArrayObject VAO;

		/// <summary>
		/// Each renderable object has access to its own VBO
		/// </summary>
		VertexBufferObject VBO;

		/// <summary>
		/// Each renderable object has access ot its own EBO
		/// </summary>
		VertexBufferObject EBO;

		bool disableBackFaceCulling = false;
		bool drawNormals = false;
		std::vector<GLfloat> vertexData;
	public:
	
		/// <summary>
		/// Creates a new Renderable
		/// </summary>
		Renderable() 
		{
			perFrameOptions = PerFrameOptions();
			VAO = VertexArrayObject();
			VBO = VertexBufferObject();		
			EBO = VertexBufferObject();
		}

		void UsePerFrameOptions()
		{
			if(perFrameOptions.RenderWireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FILL, GL_LINE);
		}
		
		// Not used?
		PerFrameOptions perFrameOptions;		

		/// <summary>
		/// Let user set if normals should be drawn ot not
		/// </summary>
		/// <param name="yesno"></param>
		void SetDrawNormals(bool yesno);

		/// <summary>
		/// Should this rendrable draw normals(if supported)
		/// </summary>
		/// <param name="yesno">yes or no</param>
		void SetDisableBackFaceCulling(bool yesno);

		/// <summary>
		/// Indates if notmals sould be drawn or not
		/// </summary>
		/// <returns>true if so, false otherwise</returns>
		bool ShouldDrawNormals();

		/// <summary>
		/// Indicates if buck face culling should happen or not
		/// </summary>
		/// <returns>true if so, false otherwise</returns>
		bool ShouldDisableBackFaceCulling();

		/// <summary>
		/// Default number of Vertices in this Renderable
		/// </summary>
		/// <returns>Number of renderable vertices</returns>
		virtual size_t GetNumberOfVertices() { return 3; }

		/// <summary>
		/// Gets the Renderable's VAO
		/// </summary>
		/// <returns></returns>
		VertexArrayObject GetVAO() { return VAO;}

		/// <summary>
		/// Gets the Renderable's VBO
		/// </summary>
		/// <returns></returns>
		VertexBufferObject GetVBO() { return VBO; }

		/// <summary>
		/// Gets all Raw vertex data
		/// </summary>
		/// <returns>All Raw vertex data</returns>
		std::vector<GLfloat> GetVertexData() { return vertexData; }

	};
}

