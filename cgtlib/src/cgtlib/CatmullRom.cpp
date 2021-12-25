#include "pch.h"
#include "CatmullRom.h"
#define _USE_MATH_DEFINES
#include <math.h>



CCatmullRom::CCatmullRom()
{
	m_vertexCount = 0;
}

CCatmullRom::~CCatmullRom()
{}

// Perform Catmull Rom spline interpolation between four points, interpolating the space between p1 and p2
glm::vec3 CCatmullRom::Interpolate(glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2, glm::vec3 &p3, float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

	glm::vec3 a = p1;
	glm::vec3 b = 0.5f * (-p0 + p2);
	glm::vec3 c = 0.5f * (2.0f*p0 - 5.0f*p1 + 4.0f*p2 - p3);
	glm::vec3 d = 0.5f * (-p0 + 3.0f*p1 - 3.0f*p2 + p3);

	return a + b*t + c*t2 + d*t3;

}


void CCatmullRom::SetControlPoints()
{
	// Set control points (m_controlPoints) here, or load from disk
	bool useCircle = false;
	auto height1 = 0.5f; // to avoid z-fighting with ground plane
	auto height2 = 10.0f;
	auto height3 = height2;
	auto height = height1;
	auto max_x = 200;

	auto type = 4;

	switch(type)
	{
		case 1: // circle
			m_controlPoints.push_back(glm::vec3(100, height, 0)); //p0
			m_controlPoints.push_back(glm::vec3(71, height, 71)); //p1
			m_controlPoints.push_back(glm::vec3(0, height, 100)); //p2
			m_controlPoints.push_back(glm::vec3(-71, height, 71)); //p3
	
			m_controlPoints.push_back(glm::vec3(-100, height, 0)); //q0
			m_controlPoints.push_back(glm::vec3(-71, height, -71)); //q1
			m_controlPoints.push_back(glm::vec3(0, height, -100)); //q2
			m_controlPoints.push_back(glm::vec3(71, height, -71)); //q3
			break;
		case 2: // default

			m_controlPoints.push_back(glm::vec3(200, height, 0)); //p0
			m_controlPoints.push_back(glm::vec3(51, height, 51)); //p1
			m_controlPoints.push_back(glm::vec3(0, height, 100)); //p2
			m_controlPoints.push_back(glm::vec3(-71, height, 71)); //p3
	
			m_controlPoints.push_back(glm::vec3(-100, height, 0)); //q0
			m_controlPoints.push_back(glm::vec3(-51, height, -51)); //q1
			m_controlPoints.push_back(glm::vec3(0, height, -100)); //q2
			m_controlPoints.push_back(glm::vec3(71, height, -71)); //q3
			break;
		case 4:

			m_controlPoints.push_back(glm::vec3(0, height, -100)); //p0
			m_controlPoints.push_back(glm::vec3(100, height, -150)); //p0
			m_controlPoints.push_back(glm::vec3(200, height, 0)); //p0
			m_controlPoints.push_back(glm::vec3(500, height, 0)); //p0

			m_controlPoints.push_back(glm::vec3(550, height, 150)); //p0
			m_controlPoints.push_back(glm::vec3(500, height, 200)); //p0
			m_controlPoints.push_back(glm::vec3(300, height, 200)); //p0
			m_controlPoints.push_back(glm::vec3(200, height, 200)); //p0
			m_controlPoints.push_back(glm::vec3(150, height+1.0f, 100)); //p0
			break;		
	}

	// Optionally, set upvectors (m_controlUpVectors, one for each control point as well)
}


// Determine lengths along the control points, which is the set of control points forming the closed curve
void CCatmullRom::ComputeLengthsAlongControlPoints()
{
	int M = (int) m_controlPoints.size();

	float fAccumulatedLength = 0.0f;
	m_distances.push_back(fAccumulatedLength);
	for (int i = 1; i < M; i++) {
		fAccumulatedLength += glm::distance(m_controlPoints[i-1], m_controlPoints[i]);
		m_distances.push_back(fAccumulatedLength);
	}

	// Get the distance from the last point to the first
	fAccumulatedLength += glm::distance(m_controlPoints[M-1], m_controlPoints[0]);
	m_distances.push_back(fAccumulatedLength);
}


// Return the point (and upvector, if control upvectors provided) based on a distance d along the control polygon
bool CCatmullRom::Sample(float d, glm::vec3 &p, glm::vec3 &up)
{
	if (d < 0)
		return false;

	int M = (int) m_controlPoints.size();
	if (M == 0)
		return false;


	float fTotalLength = m_distances[m_distances.size() - 1];

	// The the current length along the control polygon; handle the case where we've looped around the track
	float fLength = d - (int) (d / fTotalLength) * fTotalLength;

	// Find the current segment
	int j = -1;
	for (int i = 0; i < (int)m_distances.size()-1; i++) {
		if (fLength >= m_distances[i] && fLength < m_distances[i + 1]) {
			j = i; // found it!
			break;
		}
	}

	if (j == -1)
		return false;

	// Interpolate on current segment -- get t
	float fSegmentLength = m_distances[j + 1] - m_distances[j];
	float t = (fLength - m_distances[j]) / fSegmentLength;
	
	// Get the indices of the four points along the control polygon for the current segment
	int iPrev = ((j-1) + M) % M;
	int iCur = j;
	int iNext = (j + 1) % M;
	int iNextNext = (j + 2) % M;

	// Interpolate to get the point (and upvector)
	p = Interpolate(m_controlPoints[iPrev], m_controlPoints[iCur], m_controlPoints[iNext], m_controlPoints[iNextNext], t);
	if (m_controlUpVectors.size() == m_controlPoints.size())
		up = glm::normalize(Interpolate(m_controlUpVectors[iPrev], m_controlUpVectors[iCur], m_controlUpVectors[iNext], m_controlUpVectors[iNextNext], t));

	return true;
}

vector<glm::vec3> CCatmullRom::GetControlPoints()
{
	return m_controlPoints;
}

vector<glm::vec3> CCatmullRom::GetLeftPoints()
{
	return m_leftOffsetPoints;
}

vector<glm::vec3> CCatmullRom::GetRightPoints()
{
	return m_rightOffsetPoints;
}

inline float CCatmullRom::GetWidth() 
{ 
	return width; 
}

// Sample a set of control points using an open Catmull-Rom spline, to produce a set of iNumSamples that are (roughly) equally spaced
void CCatmullRom::UniformlySampleControlPoints(int numSamples)
{
	glm::vec3 p, up;

	// Compute the lengths of each segment along the control polygon, and the total length
	ComputeLengthsAlongControlPoints();
	float fTotalLength = m_distances[m_distances.size() - 1];

	// The spacing will be based on the control polygon
	float fSpacing = fTotalLength / numSamples;

	// Call PointAt to sample the spline, to generate the points
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);

	}


	// Repeat once more for truly equidistant points
	m_controlPoints = m_centrelinePoints;
	m_controlUpVectors = m_centrelineUpVectors;
	m_centrelinePoints.clear();
	m_centrelineUpVectors.clear();
	m_distances.clear();
	ComputeLengthsAlongControlPoints();
	fTotalLength = m_distances[m_distances.size() - 1];
	fSpacing = fTotalLength / numSamples;
	for (int i = 0; i < numSamples; i++) {
		Sample(i * fSpacing, p, up);
		m_centrelinePoints.push_back(p);
		if (m_controlUpVectors.size() > 0)
			m_centrelineUpVectors.push_back(up);
	}


}



void CCatmullRom::CreateCentreline()
{
	// Call Set Control Points
	SetControlPoints();

	// Call UniformlySampleControlPoints with the number of samples required
	UniformlySampleControlPoints(1550);

	// Create a VAO called m_vaoCentreline and a VBO to get the points onto the graphics card

	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vaoCentreline);
	glBindVertexArray(m_vaoCentreline);

	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {
		float t = (float)i / 100.0f;
	
		glm::vec3 v = m_centrelinePoints[i];
		vbo.AddData(&v, sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}	

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei stride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) +sizeof(glm::vec2)));
	
}


void CCatmullRom::CreateOffsetCurves()
{
	// Compute the offset curves, one left, and one right.  Store the points in m_leftOffsetPoints and m_rightOffsetPoints respectively
	auto width = GetWidth();
	auto sizeCenterPoints = m_centrelinePoints.size();
	for(int i = 0; i < m_centrelinePoints.size(); i ++)
	{
		glm::vec3 p = m_centrelinePoints[i];
		Sample(i, p);

		auto nextIndex = i + 1;
		glm::vec3 pNext;
		Sample(nextIndex, pNext);

		auto T = glm::normalize(pNext - p);
		auto y = glm::vec3(0.0f, 1.0f, 0.0);
		auto N = glm::normalize(glm::cross(T, y));
		auto B = glm::normalize(glm::cross(N, T));
		
		auto leftPoint = p - (width/2)* N;
		auto rightPoint = p + (width/2)* N;

		m_leftOffsetPoints.push_back(leftPoint);
		m_rightOffsetPoints.push_back(rightPoint);
	}

	// Generate two VAOs called m_vaoLeftOffsetCurve and m_vaoRightOffsetCurve, each with a VBO, and get the offset curve points on the graphics card
	// Note it is possible to only use one VAO / VBO with all the points instead.

	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vaoOffsetCurve);
	glBindVertexArray(m_vaoOffsetCurve);

	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {
		float t = (float)i / 100.0f;
	
		glm::vec3 l = m_leftOffsetPoints[i];
		glm::vec3 r = m_rightOffsetPoints[i];
		vbo.AddData(&l, sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
		vbo.AddData(&r, sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}	

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei stride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) +sizeof(glm::vec2)));

}


void CCatmullRom::CreateTrack()
{
	
	
	// Use VAO to store state associated with vertices
	glGenVertexArrays(1, &m_vaoTrack);
	glBindVertexArray(m_vaoTrack);

	// Create a VBO
	CVertexBufferObject vbo;
	vbo.Create();
	vbo.Bind();

	glm::vec2 texCoord(0.0f, 0.0f);
	glm::vec3 normal(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < m_centrelinePoints.size(); i++) {
		float t = (float)i / 100.0f;
	
		glm::vec3 l = m_leftOffsetPoints[i];
		
		vbo.AddData(&l, sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));

		glm::vec3 r = m_rightOffsetPoints[i];
		vbo.AddData(&r, sizeof(glm::vec3));
		vbo.AddData(&texCoord, sizeof(glm::vec2));
		vbo.AddData(&normal, sizeof(glm::vec3));
	}	

	// Upload the VBO to the GPU
	vbo.UploadDataToGPU(GL_STATIC_DRAW);

	// Set the vertex attribute locations
	GLsizei stride = 2*sizeof(glm::vec3)+sizeof(glm::vec2);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	// Texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)sizeof(glm::vec3));
	// Normal vectors
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(glm::vec3) +sizeof(glm::vec2)));

}


void CCatmullRom::RenderCentreline()
{
	// Bind the VAO m_vaoCentreline and render it
	glBindVertexArray(m_vaoCentreline);
	glLineWidth(2);
	//glDrawArrays(GL_POINTS, 0, m_centrelinePoints.size());
	glDrawArrays(GL_LINE_LOOP, 0, m_centrelinePoints.size());

}

void CCatmullRom::RenderOffsetCurves()
{
	// Bind the VAO m_vaoLeftOffsetCurve and render it

	// Bind the VAO m_vaoRightOffsetCurve and render it

	glBindVertexArray(m_vaoOffsetCurve);
	glLineWidth(2);
	glPointSize(5);
	std::vector<glm::vec3> points;
	for(int i = 0; i < m_leftOffsetPoints.size();i++)
	{
		points.push_back(m_leftOffsetPoints[i]);
		points.push_back(m_rightOffsetPoints[i]);
	}

	glDrawArrays(GL_POINTS, 0, points.size());
}


void CCatmullRom::RenderTrack()
{
	// Bind the VAO m_vaoTrack and render it
	glBindVertexArray(m_vaoTrack);
	glLineWidth(2);
	glPointSize(5);
	std::vector<glm::vec3> vertices;
	for(size_t i = 0; i < m_centrelinePoints.size(); i++)
	{
		auto left = m_leftOffsetPoints[i];
		auto right = m_rightOffsetPoints[i];		
		vertices.push_back(left);
		vertices.push_back(right);
		if(i + 1 < m_centrelinePoints.size())
		{
			auto top = m_leftOffsetPoints[i + 1];
			vertices.push_back(top);
		}
	}
	//repeat first two points
	vertices.push_back(m_leftOffsetPoints[0]);
	vertices.push_back(m_leftOffsetPoints[0]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
}

int CCatmullRom::CurrentLap(float d)
{

	return (int)(d / m_distances.back());

}

glm::vec3 CCatmullRom::_dummy_vector(0.0f, 0.0f, 0.0f);