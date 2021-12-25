#include "pch.h"
#include "Square.h"
#include "VertexAttribute.h"
#include "freeimage/FreeImage.h"
#include <iostream>
#include <fstream>

std::ostream &operator<<(std::ostream &stream, glm::vec3 &vec)
{
	stream << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return stream;
}

std::ostream &operator<<(std::ostream &stream, glm::vec2 vec)
{
	stream << "(" << vec.x << ", " << vec.y  << ")";
	return stream;
}

// Private:
stu::Square::Square(stu::Triangle ADB, stu::Triangle DCB)
{

	this->ADB = ADB;
	this->DCB = DCB;
	this->A = ADB.lv;
	this->D = ADB.rv;
	this->C = DCB.rv;
	this->B = ADB.tv;
}

stu::Square::Square(glm::vec3 bottLeft, float width, float height)
{
	A = bottLeft;
	D = glm::vec3(bottLeft.x + width, bottLeft.y, bottLeft.z);
	C = glm::vec3(D.x, D.y + height, D.z);
	B = glm::vec3(A.x, C.y, A.z);

	CreateInternalTriangles(A, D, B, C);
	
	singlePolygon = false;

}

stu::Square::Square(glm::vec3 bottLeft, float width, float height, std::shared_ptr<ShaderProgram> shaderProgram)
	:Square(bottLeft, width, height)
{
	this->shaderProgram = shaderProgram;
	singlePolygon = true;
}

stu::Square::Square(glm::vec3 bottLeft, float width, float height, std::shared_ptr<ShaderProgram> shaderProgram, std::shared_ptr<stu::Texture> texture, int sampler)
	: Square(bottLeft, width, height, shaderProgram)
{
	this->texture = texture;
	this->sampler = sampler;
	this->ShouldUseTexture(true);
}

void stu::Square::CreateInternalTriangles(glm::vec3 &A, glm::vec3 &D, glm::vec3 &B, glm::vec3 &C)
{
	ADB = Triangle(A, D, B);
	DCB = Triangle(D, C, B);
}



stu::Square::Square(stu::Triangle ADB, stu::Triangle DCB, std::shared_ptr<Texture> texture, std::shared_ptr<ShaderProgram> shaderProgram, int sampler )
	:Square(ADB, DCB)
{
	this->singlePolygon = true;
	this->texture = texture;
	this->shaderProgram = shaderProgram;
	this->useTexture = true;	
	this->sampler = sampler;
}

stu::Square::Square(stu::Triangle ADB, stu::Triangle DCB, std::shared_ptr<ShaderProgram> shaderProgram)
	: Square(ADB, DCB)
{
	this->singlePolygon = true;
	this->shaderProgram = shaderProgram;
	this->useTexture = false;
}

void stu::Square::SetTextureCoordinates(glm::vec2 tca, glm::vec2 tcd, glm::vec2 tcc, glm::vec2 tcb)
{
	this->tca = tca;
	this->tcd = tcd;
	this->tcc = tcc;
	this->tcb = tcb;
}

void stu::Square::Create(std::string face)
{
	if(!singlePolygon)
	{
		ADB.Create();
		DCB.Create();
		return;
	}

	//Use 4 points derived from the 2 underlying triangles
	auto c = DCB;
	auto d = ADB;
	auto a = ADB;
	auto b = ADB;

	// Populate the vertex buffer: PosNormColText Layout setup
	auto PosNormColText = [&]()
	{
			
		GLfloat vertices[] = 
		{ 
				// position(x, y, z), normal(x,y,z), color(r,g,b), text coords(x,y)				
				a.lv.x, a.lv.y, a.lv.z, a.lvn.x, a.lvn.y, a.lvn.z, a.lvc.r, a.lvc.g, a.lvc.b, tca.x, tca.y, // bottom left
				d.rv.x, d.rv.y, d.rv.z, d.rvn.x, d.rvn.y, d.rvn.z, d.rvc.r, d.rvc.g, d.rvc.b, tcd.x, tcd.y, // bottom right
				c.rv.x, c.rv.y, c.rv.z, c.rvn.x, c.rvn.y, c.rvn.z, c.rvc.r, c.rvc.g, c.rvc.b, tcc.x, tcc.y,  // top right
				b.tv.x, b.tv.y, b.tv.z, b.tvn.x, b.tvn.y, b.tvn.z, b.tvc.r, b.tvc.g, b.tvc.b, tcb.x, tcb.y  // top left 
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
				
		EBO.Release();
		VBO.Release();

	};

	// Populate the vertex buffer: PosTexNormCol Layout setup
	auto PosTextNormCol = [&]()
	{			
		
		
		GLfloat mainshader_vertices[] =
		{
			// position(x, y, z),  text coords(x,y) normal(x,y,z), color(r,g,b),
			a.lv.x, a.lv.y, a.lv.z, tca.x, tca.y, a.lvn.x, a.lvn.y, a.lvn.z, a.lvc.r, a.lvc.g, a.lvc.b, // bottom left
			d.rv.x, d.rv.y, d.rv.z, tcd.x, tcd.y, d.rvn.x, d.rvn.y, d.rvn.z, d.rvc.r, d.rvc.g, d.rvc.b, // bottom right
			c.rv.x, c.rv.y, c.rv.z, tcc.x, tcc.y, c.rvn.x, c.rvn.y, c.rvn.z, c.rvc.r, c.rvc.g, c.rvc.b,  // top right
			b.tv.x, b.tv.y, b.tv.z, tcb.x, tcb.y, b.tvn.x, b.tvn.y, b.tvn.z, b.tvc.r, b.tvc.g, b.tvc.b,   // top left 
		};

		GLint indices[] =
		{
			0, 1, 3, // First Triangle
			1, 2, 3 // Second Triangle
		};

		VBO.Create(sizeof(mainshader_vertices), GL_ARRAY_BUFFER, false, true);
		EBO.Create(sizeof(indices), GL_ELEMENT_ARRAY_BUFFER, false, true);

		VBO.UploadData(mainshader_vertices);
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
				VAO.Record(PosNormColText);
		break;
		case VertexLayout::PosTexNormCol:
				VAO.Record(PosTextNormCol);
		break;
	}
		
	// Debugging the vertices positions
	if(isUsingDiagnostics)
		PrintVertices(face, a, d, c, b);

	// Construct the normal lines
	CreateNormalLines(a, b, c, d);
}

void stu::Square::PrintVertices(std::string& face, stu::Triangle& a, stu::Triangle& d, stu::Triangle& c, stu::Triangle& b)
{
	std::ofstream output("squareVertices.txt", std::ios::app);

	if (output.is_open())
	{
		if(shaderProgram->vertexLayout == VertexLayout::PosNormColTex)
		{
		/*
			// position(x, y, z), normal(x,y,z), color(r,g,b), text coords(x,y)			

			a.lv.x, a.lv.y, a.lv.z, a.lvn.x, a.lvn.y, a.lvn.z, a.lvc.r, a.lvc.g, a.lvc.b, tca.x, tca.y, // bottom left
			d.rv.x, d.rv.y, d.rv.z, d.rvn.x, d.rvn.y, d.rvn.z, d.rvc.r, d.rvc.g, d.rvc.b, tcd.x, tcd.y, // bottom right
			c.rv.x, c.rv.y, c.rv.z, c.rvn.x, c.rvn.y, c.rvn.z, c.rvc.r, c.rvc.g, c.rvc.b, tcc.x, tcc.y,  // top right
			b.tv.x, b.tv.y, b.tv.z, b.tvn.x, b.tvn.y, b.tvn.z, b.tvc.r, b.tvc.g, b.tvc.b, tcb.x, tcb.y  // top left 
		*/
		output << std::endl << face << " :" << std::endl;


		output << "v " << a.lv << " # A" << std::endl;
		output << "v " << d.rv << " # D" << std::endl;
		output << "v " << c.rv << " # C" << std::endl;
		output << "v " << b.tv << " # B" << std::endl;

		output << "vn " << a.lvn << " # A" << std::endl;
		output << "vn " << d.rvn << " # D" << std::endl;
		output << "vn " << c.rvn << " # C" << std::endl;
		output << "vn " << b.tvn << " # B" << std::endl;

		output << "vt " << glm::vec2(tca.x, tca.y) << " # A" << std::endl;
		output << "vt " << glm::vec2(tcd.x, tcd.y) << " # D" << std::endl;
		output << "vt " << glm::vec2(tcc.x, tcc.y) << " # C" << std::endl;
		output << "vt " << glm::vec2(tcb.x, tcb.y) << " # B" << std::endl;
		}

	}
	output.close();
}



void stu::Square::ShouldUseTexture(bool yesno)
{
	useTexture = yesno;
}

void stu::Square::CreateNormalLines(stu::Triangle& a, stu::Triangle& b, stu::Triangle& c, stu::Triangle& d)
{
	auto factor = 0.1f;
	auto face_normal = a.lvn; // assuming all have the same normal		

	auto Aend = a.lvn * factor;
	auto Bend = b.rvn * factor;
	auto Cend = c.tvn * factor;
	auto Dend = d.tvn * factor;

	Aend = A + face_normal * factor;
	Bend = B + face_normal * factor;
	Cend = C + face_normal * factor;
	Dend = D + face_normal * factor;

	auto centoid = (A + D + B + C) / 4.0f;
	ANormalLine = std::make_shared<stu::Line>(A, Aend);
	BNormalLine = std::make_shared<stu::Line>(D, Dend);
	CNormalLine = std::make_shared<stu::Line>(C, Cend);
	DNormalLine = std::make_shared<stu::Line>(B, Bend);

	centoidNormalLine = std::make_shared<stu::Line>(centoid, centoid + face_normal * factor, glm::vec3(0.0f, 255.0f, 0.0f));

	ANormalLine->Create();
	BNormalLine->Create();
	CNormalLine->Create();
	DNormalLine->Create();
	centoidNormalLine->Create();
}

void stu::Square::Update(float delta)
{

}

void stu::Square::Render()
{
	// Setup texture support
	if(useTexture) 
	{	
		shaderProgram->EnableUseTextureUniform(true);
		texture->Use(shaderProgram->GetId(), shaderProgram->GetSamplerName(), sampler);
	} 
	else
	{
		if(shaderProgram != nullptr)
			shaderProgram->EnableUseTextureUniform(false);
	}

	if(!singlePolygon)
	{
		// Render underlying triangles
		ADB.Render();
		DCB.Render();
	}
	else
	{		
		// Draw Vector Normal Lines
		if(drawNormals)
			RenderNormalLines();

		// Draw Square's Vertices
		VAO.Use();
			glDrawElements(GL_TRIANGLES, GetNumberOfVertices(), GL_UNSIGNED_INT , 0);	
		VAO.Release();

		// Finish up
		if(useTexture)
			texture->Release();
	}
}

size_t stu::Square::GetNumberOfVertices() { return 6; }



void stu::Square::RenderNormalLines()
{
	shaderProgram->EnableUseTextureUniform(false);
	ANormalLine->Render();
	BNormalLine->Render();
	CNormalLine->Render();
	DNormalLine->Render();
	//zeroNormalLine->Render();
	centoidNormalLine->Render();
	shaderProgram->EnableUseTextureUniform(true);
}
