#include "pch.h"
#include "CatmullRom.h"
using namespace glm;

namespace stu
{
CCatmullRom::CCatmullRom()
{
}

CCatmullRom::~CCatmullRom()
{
}

glm::vec3 CCatmullRom::Interpolate(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3, float t)
{
	const vec3 a = p1;
	const vec3 b = 0.5f * (p2 - p0);
	const vec3 c = 0.5f * ((2.0f * p0) - (5.0f *p1) + (4.0f * p2) - p2);
	const vec3 d = 0.5f * (-p0 + (3.0f * p1) - (3.0f * p2) + p3);
	
	// X(t) = a + bt + ct^2 + dt^3  (This is the cubic spline equasion)
	auto X = [&](const float t)
	{		
		return a + (b * t) + (c * pow(t,2.0f) + (d * pow(t, 3.0f)));
	};
	return X(t);
}

// Calculates derivative of cubic splite equasion, which is the vector pointing at the next point to interpolate
glm::vec3 CCatmullRom::DirectionAtT(float t, glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3)
{
	const vec3 a = p1;
	const vec3 b = 0.5f * (p2 - p0);
	const vec3 c = 0.5f * ((2.0f * p0) - (5.0f *p1) + (4.0f * p2) - p2);
	const vec3 d = 0.5f * (-p0 + (3.0f * p1) - (3.0f * p2) + p3);

	// This is the derivative of the cubic spline equasion
	auto Xprime = [&](const float t) -> vec3
	{
		return b + (2.0f * c * t) + (3.0f * d * pow(t, 2.0f));
	};
	return Xprime(t);
}

//void CCatmullRom::CreatePath(glm::vec3& p0, glm::vec3& p1, glm::vec3& p2, glm::vec3& p3)
//{
//	// Use VAO to store state associated with vertices
//	glGenVertexArrays(1, &m_vao);
//	glBindVertexArray(m_vao);
//
//	// Create a VBO
//	CVertexBufferObject vbo;
//	vbo.Create();
//	vbo.Bind();
//
//	glm::vec2 texCoord(0.0f, 0.0f);
//	glm::vec3 normal(0.0f, 1.0f, 0.0f);
//	for (unsigned int i = 0; i < 100; i++) {
//		float t = (float)i / 100.0f;
//	
//		glm::vec3 v = Interpolate(p0, p1, p2, p3, t);
//		vbo.AddData(&v, sizeof(glm::vec3));
//		vbo.AddData(&texCoord, sizeof(glm::vec2));
//		vbo.AddData(&normal, sizeof(glm::vec3));
//	}
//
//	// Upload the VBO to the GPU
//	vbo.UploadDataToGPU(GL_STATIC_DRAW);
//
//	// Set the vertex attribute locations
//	GLsizei stride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);
//
//	// Vertex positions
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
//	// Texture coordinates
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
//	// Normal vectors
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) +sizeof(glm::vec2)));
//}
//
//void CCatmullRom::RenderPath()
//{
//	// vind the vao to point the pseudo GL_ARRAY_BUFFER to use our VBO with our vertices in it.
//	glBindVertexArray(m_vao);
//	glLineWidth(2);
//	glDrawArrays(GL_LINE_STRIP, 0, 100);
//}
}
