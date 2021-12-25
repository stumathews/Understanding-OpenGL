/*
	This is not used.
*/
#include "pch.h"
#include "Shapes.h"
#include "glm/detail/type_vec.hpp"
#include "stulib/SquareGrid.h"
#include "freeimage/FreeImage.h"
#include "stulib/common.h"
#include "stulib/PerFrame.h"
#include "stulib/Line.h"
#include "stulib/GameObject.h"
#include "stulib/StuTexture.h"
using namespace std;

Shapes::Shapes()
{
}

void Shapes::TextureTriangles(glm::vec3& initialPoint, bool draw, vector<shared_ptr<stu::IPerFrame>> &scene, shared_ptr<stu::ShaderProgram> withInterimShaderProrgam)
{
	if(!draw) return;

	
	FIBITMAP* dib(0);
	auto getImageData = [&](std::string filePath, int* width, int* height) -> unsigned char*
	{		
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		

		fif = FreeImage_GetFileType(filePath.c_str(), 0); // Check the file signature and deduce its format

		if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
			fif = FreeImage_GetFIFFromFilename(filePath.c_str());
	
		if(fif == FIF_UNKNOWN) // If still unknown, return failure
			return nullptr;

		if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
			dib = FreeImage_Load(fif, filePath.c_str());

		if(!dib) {
			char message[1024];
			sprintf_s(message, "Cannot load image\n%s\n", filePath.c_str());
			//MessageBox(NULL, message, "Error", MB_ICONERROR);
			return nullptr;
		}

		BYTE* pData = FreeImage_GetBits(dib); // Retrieve the image data

		// If somehow one of these failed (they shouldn't), return failure
		if (pData == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
			return nullptr;
	

		GLenum format;
		int bada = FreeImage_GetBPP(dib);
		if(FreeImage_GetBPP(dib) == 32)format = GL_BGRA;
		if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
		if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;

		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
			
		return pData;
	};

	auto freeImageData = [&](unsigned char* image) -> bool
	{
		FreeImage_Unload(dib);
		return true;
	};

	auto grassPath = "D:\\repos\\Computer Graphics\\Labs\\Lab4\\Demo\\OpenGLTemplate\\resources\\textures\\grassfloor01.jpg";
	auto wallPath = "D:\\repos\\Computer Graphics\\Labs\\Lab4\\Demo\\OpenGLTemplate\\resources\\textures\\wall.jpg";
	auto roofTilePath = "D:\\repos\\Computer Graphics\\Labs\\Lab4\\Demo\\OpenGLTemplate\\resources\\textures\\roof.jpg";

	auto grassTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
	auto wallTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);	
	auto roofTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
	
	wallTexture->Create(wallPath, true, true);
	grassTexture->Create(grassPath, true, true);
	roofTexture->Create(roofTilePath, true, true);

	glm::vec3 A = initialPoint;
	glm::vec3 B(-0.5f, 0.5f, 0.0f);
	glm::vec3 C(0.5f, 0.5f, 0.0f);
	glm::vec3 D(0.5f, -0.5f, 0.0f);
	glm::vec3 E(-0.7f, 0.5f, 0.0f);
	glm::vec3 F(0.7f, 0.5f, 0.0f);
	glm::vec3 G(0.0f, 1.2f, 0.0f);
	glm::vec3 E_ = glm::vec3(E.x, E.y, -1.0);
	glm::vec3 F_ = glm::vec3(F.x, F.y, -1.0);
	glm::vec3 G_ = glm::vec3(G.x, G.y, -1.0);

	glm::vec3 A_(-0.5f, -0.5f, -1.0f);
	glm::vec3 B_(-0.5f, 0.5f, -1.0f);
	glm::vec3 C_(0.5f, 0.5f, -1.0f);
	glm::vec3 D_(0.5f, -0.5f, -1.0f);
	glm::vec3 J = glm::vec3(E.x, E.y, E.z + 0.2f);
	glm::vec3 I = glm::vec3(G.x, G.y, G.z + 0.2f);
	glm::vec3 J_ = glm::vec3(E_.x, E_.y, E_.z - 0.2f);
	glm::vec3 I_ = glm::vec3(G_.x, G_.y, G_.z - 0.2f);
	glm::vec3 H = glm::vec3(F.x, F.y, F.z + 0.2f);
	glm::vec3 H_ = glm::vec3(F_.x, F_.y, F_.z - 0.2f);

	glm::vec3 K_ = D_; // SBott
	glm::vec3 L_ = A_;
	glm::vec3 M_ = D;
	glm::vec3 N_ = A;
	glm::vec3 R_ = B_; // Left wall
	glm::vec3 Q_ = B;
	glm::vec3 P_ = A_;
	glm::vec3 O_ = A;
	glm::vec3 Q = C; // Right wall
	glm::vec3 R = C_;

	glm::vec3 P = D_;
	glm::vec3 O = D;
	glm::vec3 S = F_; // Roof mouth (ceil)
	glm::vec3 T = E_;
	glm::vec3 U = E;
	glm::vec3 V = F;

	shared_ptr<stu::IPerFrame> ADB(new stu::Triangle(A, D, B));
	reinterpret_pointer_cast<stu::Triangle>(ADB)->setTexture(wallTexture, withInterimShaderProrgam);
	reinterpret_pointer_cast<stu::Triangle>(ADB)->Create();
	shared_ptr<stu::IPerFrame> DCB(new stu::Triangle(D, C, B));
	reinterpret_pointer_cast<stu::Triangle>(DCB)->Create();

	shared_ptr<stu::IPerFrame> D_A_C_(new stu::Triangle(D_, A_, C_));
	reinterpret_pointer_cast<stu::Triangle>(D_A_C_)->Create();
	shared_ptr<stu::IPerFrame> A_B_C_(new stu::Triangle(A_, B_, C_));
	reinterpret_pointer_cast<stu::Triangle>(A_B_C_)->Create();

	shared_ptr<stu::IPerFrame> EFG(new stu::Triangle(E, F, G));
	reinterpret_pointer_cast<stu::Triangle>(EFG)->setTexture(roofTexture, withInterimShaderProrgam);
	reinterpret_pointer_cast<stu::Triangle>(EFG)->Create();
	shared_ptr<stu::IPerFrame> F_E_G_(new stu::Triangle(F_, E_, G_));
	reinterpret_pointer_cast<stu::Triangle>(F_E_G_)->Create();

	//Roofs

	shared_ptr<stu::IPerFrame> J_JI_(new stu::Triangle(J_, J, I_));
	reinterpret_pointer_cast<stu::Triangle>(J_JI_)->Create();

	shared_ptr<stu::IPerFrame> JII_(new stu::Triangle(J, I, I_));
	reinterpret_pointer_cast<stu::Triangle>(JII_)->Create();

	shared_ptr<stu::IPerFrame> HH_I(new stu::Triangle(H, H_, I));
	reinterpret_pointer_cast<stu::Triangle>(HH_I)->Create();

	shared_ptr<stu::IPerFrame> H_I_I(new stu::Triangle(H_, I_, I));
	reinterpret_pointer_cast<stu::Triangle>(H_I_I)->Create();

	//SBOT
	shared_ptr<stu::IPerFrame> M_N_K_(new stu::Triangle(M_, N_, K_));
	reinterpret_pointer_cast<stu::Triangle>(M_N_K_)->Create();

	shared_ptr<stu::IPerFrame> N_L_K_(new stu::Triangle(N_, L_, K_));
	reinterpret_pointer_cast<stu::Triangle>(N_L_K_)->Create();

	// left wall

	shared_ptr<stu::IPerFrame> P_O_R_(new stu::Triangle(P_, O_, R_));
	reinterpret_pointer_cast<stu::Triangle>(P_O_R_)->Create();

	shared_ptr<stu::IPerFrame> O_Q_R_(new stu::Triangle(O_, Q_, R_));
	reinterpret_pointer_cast<stu::Triangle>(O_Q_R_)->Create();

	// right wall
	shared_ptr<stu::IPerFrame> OPQ(new stu::Triangle(O, P, Q));
	reinterpret_pointer_cast<stu::Triangle>(OPQ)->Create();

	shared_ptr<stu::IPerFrame> PRQ(new stu::Triangle(P, R, Q));
	reinterpret_pointer_cast<stu::Triangle>(PRQ)->Create();

	// roof mouth
	shared_ptr<stu::IPerFrame> VUS(new stu::Triangle(V, U, S));
	reinterpret_pointer_cast<stu::Triangle>(VUS)->Create();

	shared_ptr<stu::IPerFrame> UTS(new stu::Triangle(U, T, S));
	reinterpret_pointer_cast<stu::Triangle>(UTS)->Create();

	// front wall
	scene.push_back(ADB);
	scene.push_back(DCB);

	// back wall
	scene.push_back(D_A_C_);
	scene.push_back(A_B_C_);

	//font triangle
	scene.push_back(EFG);

	// back triange
	scene.push_back(F_E_G_);

	// roofs- left
	scene.push_back(J_JI_);
	scene.push_back(JII_);

	// roofs- right
	scene.push_back(HH_I);
	scene.push_back(H_I_I);


	// bott
	scene.push_back(M_N_K_);
	scene.push_back(N_L_K_);


	// left wall
	scene.push_back(P_O_R_);
	scene.push_back(O_Q_R_);

	// right wall
	scene.push_back(OPQ);
	scene.push_back(PRQ);


	// roof mouth
	scene.push_back(VUS);
	scene.push_back(UTS);

	// add door

	// add two windows

	// calculate normals

	bool drawNormals = false;
	if (drawNormals)
	{
		vector<shared_ptr<stu::Triangle>> faces;

		for (auto& renderable : scene)
		{
			auto tri = dynamic_pointer_cast<stu::Triangle>(renderable);
			faces.push_back(tri);
		}

		int count = 0;
		for (const auto& face : faces)
		{
			//if(count == 1) continue;
			face->UpdateVertexNormals(faces);
			auto factor = 0.1f;

			auto lvs = face->lv;
			auto lve = face->lvn * 0.1f;

			auto rvs = face->rv;
			auto rve = face->rvn * 0.1f;

			auto tvs = face->tv;
			auto tve = face->tvn * 0.1f;
			std::shared_ptr<stu::Line> lvline(new stu::Line(lvs, lve));
			std::shared_ptr<stu::Line> rvline(new stu::Line(rvs, rve));
			std::shared_ptr<stu::Line> tvline(new stu::Line(tvs, tve));

			lvline->Create();
			rvline->Create();
			tvline->Create();
			scene.push_back(lvline);
			scene.push_back(rvline);
			scene.push_back(tvline);
			count++;
		}
	}
}

void Shapes::CreateCube(glm::vec3& A, bool draw, std::vector<shared_ptr<stu::IPerFrame>> &scene, std::shared_ptr<stu::ShaderProgram> withInterimShaderProrgam)
{
if(!draw)
		return;
	glm::vec3 red(255.0f, 0.0f, 0.0f);

	glm::vec3 B(-0.5f, 0.5f, 0.0f);
	glm::vec3 C(0.5f, 0.5f, 0.0f);
	glm::vec3 D(0.5f, -0.5f, 0.0f);
	glm::vec3 E(-0.7f, 0.5f, 0.0f);
	glm::vec3 F(0.7f, 0.5f, 0.0f);
	glm::vec3 G(0.0f, 1.2f, 0.0f);
	glm::vec3 E_ = glm::vec3(E.x, E.y, -1.0);
	glm::vec3 F_ = glm::vec3(F.x, F.y, -1.0);
	glm::vec3 G_ = glm::vec3(G.x, G.y, -1.0);

	glm::vec3 A_(-0.5f, -0.5f, -1.0f);
	glm::vec3 B_(-0.5f, 0.5f, -1.0f);
	glm::vec3 C_(0.5f, 0.5f, -1.0f);
	glm::vec3 D_(0.5f, -0.5f, -1.0f);
	glm::vec3 J = glm::vec3(E.x, E.y, E.z + 0.2f);
	glm::vec3 I = glm::vec3(G.x, G.y, G.z + 0.2f);
	glm::vec3 J_ = glm::vec3(E_.x, E_.y, E_.z - 0.2f);
	glm::vec3 I_ = glm::vec3(G_.x, G_.y, G_.z - 0.2f);
	glm::vec3 H = glm::vec3(F.x, F.y, F.z + 0.2f);
	glm::vec3 H_ = glm::vec3(F_.x, F_.y, F_.z - 0.2f);

	glm::vec3 K_ = D_; // SBott
	glm::vec3 L_ = A_;
	glm::vec3 M_ = D;
	glm::vec3 N_ = A;
	glm::vec3 R_ = B_; // Left wall
	glm::vec3 Q_ = B;
	glm::vec3 P_ = A_;
	glm::vec3 O_ = A;
	glm::vec3 Q = C; // Right wall
	glm::vec3 R = C_;

	glm::vec3 P = D_;
	glm::vec3 O = D;
	glm::vec3 S = F_; // Roof mouth (ceil)
	glm::vec3 T = E_;
	glm::vec3 U = E;
	glm::vec3 V = F;

	glm::vec3 K = R;
	glm::vec3 L = R_;
	glm::vec3 M = C;
	glm::vec3 N = B;

	// square front
	shared_ptr<stu::Triangle> ADB(new stu::Triangle(A, D, B));
	shared_ptr<stu::Triangle> DCB(new stu::Triangle(D, C, B));

	// square back
	shared_ptr<stu::Triangle> D_A_C_(new stu::Triangle(D_, A_, C_));
	shared_ptr<stu::Triangle> A_B_C_(new stu::Triangle(A_, B_, C_));



	//SBOT
	shared_ptr<stu::Triangle> M_N_K_(new stu::Triangle(M_, N_, K_));

	shared_ptr<stu::Triangle> N_L_K_(new stu::Triangle(N_, L_, K_));

	// left wall

	shared_ptr<stu::Triangle> P_O_R_(new stu::Triangle(P_, O_, R_));

	shared_ptr<stu::Triangle> O_Q_R_(new stu::Triangle(O_, Q_, R_));

	// right wall
	shared_ptr<stu::Triangle> OPQ(new stu::Triangle(O, P, Q));

	shared_ptr<stu::Triangle> PRQ(new stu::Triangle(P, R, Q));

	//STOP
	shared_ptr<stu::Triangle> KLM(new stu::Triangle(K,L,M));
	shared_ptr<stu::Triangle> LNM(new stu::Triangle(L,N, M));
	
	

	FIBITMAP* dib(0);
	auto getImageData = [&](std::string filePath, int* width, int* height) -> unsigned char*
	{		
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		

		fif = FreeImage_GetFileType(filePath.c_str(), 0); // Check the file signature and deduce its format

		if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
			fif = FreeImage_GetFIFFromFilename(filePath.c_str());
	
		if(fif == FIF_UNKNOWN) // If still unknown, return failure
			return nullptr;

		if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
			dib = FreeImage_Load(fif, filePath.c_str());

		if(!dib) {
			char message[1024];
			sprintf_s(message, "Cannot load image\n%s\n", filePath.c_str());
			//MessageBox(NULL, message, "Error", MB_ICONERROR);
			return nullptr;
		}

		BYTE* pData = FreeImage_GetBits(dib); // Retrieve the image data

		// If somehow one of these failed (they shouldn't), return failure
		if (pData == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
			return nullptr;
	

		GLenum format;
		int bada = FreeImage_GetBPP(dib);
		if(FreeImage_GetBPP(dib) == 32)format = GL_BGRA;
		if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
		if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;

		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
			
		return pData;
	};

	auto freeImageData = [&](unsigned char* image) -> bool
	{
		FreeImage_Unload(dib);
		return true;
	};

	auto grassPath = "D:\\repos\\Computer Graphics\\Labs\\Lab4\\Demo\\OpenGLTemplate\\resources\\textures\\grassfloor01.jpg";
	auto wallPath = "D:\\repos\\Computer Graphics\\Labs\\Lab4\\Demo\\OpenGLTemplate\\resources\\textures\\wall.jpg";
	auto roofTilePath = "D:\\repos\\Computer Graphics\\Labs\\Lab4\\Demo\\OpenGLTemplate\\resources\\textures\\roof.jpg";

	auto grassTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
	auto wallTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);	
	auto roofTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
	
	wallTexture->Create(wallPath, true, true);
	grassTexture->Create(grassPath, true, true);
	roofTexture->Create(roofTilePath, true, true);
	
	auto SF = make_shared<stu::Square>(*ADB, *DCB, wallTexture, withInterimShaderProrgam);	
	auto SB = make_shared<stu::Square>(*D_A_C_, *A_B_C_, wallTexture, withInterimShaderProrgam);
	
	auto SBOTT = make_shared<stu::Square>(*M_N_K_, *N_L_K_, wallTexture, withInterimShaderProrgam);
	auto RW = make_shared<stu::Square>(*OPQ, *PRQ, wallTexture, withInterimShaderProrgam);
	auto LW = make_shared<stu::Square>(*P_O_R_, *O_Q_R_, wallTexture, withInterimShaderProrgam);
	auto STOP = make_shared<stu::Square>(*KLM, *LNM, wallTexture, withInterimShaderProrgam);
	
		
	SF->Create("SF");
	SB->Create("SB");	
	SBOTT->Create("SBOTT");
	RW->Create("RW");
	LW->Create("LW");
	STOP->Create("STOP");

	scene.push_back(LW);
	scene.push_back(RW);
	scene.push_back(SBOTT);
	scene.push_back(SF);
	scene.push_back(SB);
	scene.push_back(STOP);
}

void Shapes::MergeTriangles(glm::vec3& A, bool draw, std::vector<shared_ptr<stu::IPerFrame>> &scene, std::shared_ptr<stu::ShaderProgram> withInterimShaderProrgam)
{
	if(!draw)
		return;
	glm::vec3 red(255.0f, 0.0f, 0.0f);

	glm::vec3 B(-0.5f, 0.5f, 0.0f);
	glm::vec3 C(0.5f, 0.5f, 0.0f);
	glm::vec3 D(0.5f, -0.5f, 0.0f);
	glm::vec3 E(-0.7f, 0.5f, 0.0f);
	glm::vec3 F(0.7f, 0.5f, 0.0f);
	glm::vec3 G(0.0f, 1.2f, 0.0f);
	glm::vec3 E_ = glm::vec3(E.x, E.y, -1.0);
	glm::vec3 F_ = glm::vec3(F.x, F.y, -1.0);
	glm::vec3 G_ = glm::vec3(G.x, G.y, -1.0);

	glm::vec3 A_(-0.5f, -0.5f, -1.0f);
	glm::vec3 B_(-0.5f, 0.5f, -1.0f);
	glm::vec3 C_(0.5f, 0.5f, -1.0f);
	glm::vec3 D_(0.5f, -0.5f, -1.0f);
	glm::vec3 J = glm::vec3(E.x, E.y, E.z + 0.2f);
	glm::vec3 I = glm::vec3(G.x, G.y, G.z + 0.2f);
	glm::vec3 J_ = glm::vec3(E_.x, E_.y, E_.z - 0.2f);
	glm::vec3 I_ = glm::vec3(G_.x, G_.y, G_.z - 0.2f);
	glm::vec3 H = glm::vec3(F.x, F.y, F.z + 0.2f);
	glm::vec3 H_ = glm::vec3(F_.x, F_.y, F_.z - 0.2f);

	glm::vec3 K_ = D_; // SBott
	glm::vec3 L_ = A_;
	glm::vec3 M_ = D;
	glm::vec3 N_ = A;
	glm::vec3 R_ = B_; // Left wall
	glm::vec3 Q_ = B;
	glm::vec3 P_ = A_;
	glm::vec3 O_ = A;
	glm::vec3 Q = C; // Right wall
	glm::vec3 R = C_;

	glm::vec3 P = D_;
	glm::vec3 O = D;
	glm::vec3 S = F_; // Roof mouth (ceil)
	glm::vec3 T = E_;
	glm::vec3 U = E;
	glm::vec3 V = F;

	// square front
	shared_ptr<stu::Triangle> ADB(new stu::Triangle(A, D, B));
	shared_ptr<stu::Triangle> DCB(new stu::Triangle(D, C, B));

	// square back
	shared_ptr<stu::Triangle> D_A_C_(new stu::Triangle(D_, A_, C_));
	shared_ptr<stu::Triangle> A_B_C_(new stu::Triangle(A_, B_, C_));

	// triangle front
	shared_ptr<stu::Triangle> EFG(new stu::Triangle(E, F, G, red, red, red));

	// triangle back
	shared_ptr<stu::Triangle> F_E_G_(new stu::Triangle(F_, E_, G_, red, red, red));

	//Roofs

	shared_ptr<stu::Triangle> J_JI_(new stu::Triangle(J_, J, I_, red, red, red));

	shared_ptr<stu::Triangle> JII_(new stu::Triangle(J, I, I_, red, red, red));

	shared_ptr<stu::Triangle> HH_I(new stu::Triangle(H, H_, I, red, red, red));

	shared_ptr<stu::Triangle> H_I_I(new stu::Triangle(H_, I_, I, red, red, red));

	//SBOT
	shared_ptr<stu::Triangle> M_N_K_(new stu::Triangle(M_, N_, K_));

	shared_ptr<stu::Triangle> N_L_K_(new stu::Triangle(N_, L_, K_));

	// left wall

	shared_ptr<stu::Triangle> P_O_R_(new stu::Triangle(P_, O_, R_));

	shared_ptr<stu::Triangle> O_Q_R_(new stu::Triangle(O_, Q_, R_));

	// right wall
	shared_ptr<stu::Triangle> OPQ(new stu::Triangle(O, P, Q));

	shared_ptr<stu::Triangle> PRQ(new stu::Triangle(P, R, Q));

	// roof mouth
	shared_ptr<stu::Triangle> VUS(new stu::Triangle(V, U, S));

	shared_ptr<stu::Triangle> UTS(new stu::Triangle(U, T, S));
	
	
	

	FIBITMAP* dib(0);
	auto getImageData = [&](std::string filePath, int* width, int* height) -> unsigned char*
	{		
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		

		fif = FreeImage_GetFileType(filePath.c_str(), 0); // Check the file signature and deduce its format

		if(fif == FIF_UNKNOWN) // If still unknown, try to guess the file format from the file extension
			fif = FreeImage_GetFIFFromFilename(filePath.c_str());
	
		if(fif == FIF_UNKNOWN) // If still unknown, return failure
			return nullptr;

		if(FreeImage_FIFSupportsReading(fif)) // Check if the plugin has reading capabilities and load the file
			dib = FreeImage_Load(fif, filePath.c_str());

		if(!dib) {
			char message[1024];
			sprintf_s(message, "Cannot load image\n%s\n", filePath.c_str());
			//MessageBox(NULL, message, "Error", MB_ICONERROR);
			return nullptr;
		}

		BYTE* pData = FreeImage_GetBits(dib); // Retrieve the image data

		// If somehow one of these failed (they shouldn't), return failure
		if (pData == NULL || FreeImage_GetWidth(dib) == 0 || FreeImage_GetHeight(dib) == 0)
			return nullptr;
	

		GLenum format;
		int bada = FreeImage_GetBPP(dib);
		if(FreeImage_GetBPP(dib) == 32)format = GL_BGRA;
		if(FreeImage_GetBPP(dib) == 24)format = GL_BGR;
		if(FreeImage_GetBPP(dib) == 8)format = GL_LUMINANCE;

		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);
			
		return pData;
	};

	auto freeImageData = [&](unsigned char* image) -> bool
	{
		FreeImage_Unload(dib);
		return true;
	};

	auto grassPath = "D:\\repos\\Computer Graphics\\Labs\\Lab4\\Demo\\OpenGLTemplate\\resources\\textures\\grassfloor01.jpg";
	auto wallPath = "D:\\repos\\Computer Graphics\\Labs\\Lab4\\Demo\\OpenGLTemplate\\resources\\textures\\wall.jpg";
	auto roofTilePath = "D:\\repos\\Computer Graphics\\Labs\\Lab4\\Demo\\OpenGLTemplate\\resources\\textures\\roof.jpg";

	auto grassTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
	auto wallTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);	
	auto roofTexture = std::make_shared<stu::Texture>(getImageData, freeImageData);
	
	wallTexture->Create(wallPath, true, true);
	grassTexture->Create(grassPath, true, true);
	roofTexture->Create(roofTilePath, true, true);
	
	auto SF = make_shared<stu::Square>(*ADB, *DCB, wallTexture, withInterimShaderProrgam);	
	auto SB = make_shared<stu::Square>(*D_A_C_, *A_B_C_, wallTexture, withInterimShaderProrgam);
	auto RL = make_shared<stu::Square>(*J_JI_, *JII_, roofTexture, withInterimShaderProrgam);
	auto RR = make_shared<stu::Square>(*HH_I, *H_I_I, roofTexture, withInterimShaderProrgam);
	auto SBOTT = make_shared<stu::Square>(*M_N_K_, *N_L_K_, wallTexture, withInterimShaderProrgam);
	auto RW = make_shared<stu::Square>(*OPQ, *PRQ, wallTexture, withInterimShaderProrgam);
	auto LW = make_shared<stu::Square>(*P_O_R_, *O_Q_R_, wallTexture, withInterimShaderProrgam);
	auto CEIL = make_shared<stu::Square>(*VUS, *UTS, roofTexture, withInterimShaderProrgam);
	auto TF = EFG;
	auto TB = F_E_G_;

	TF->setTexture(roofTexture, withInterimShaderProrgam);
	TF->Create("TF");
	TB->setTexture(roofTexture, withInterimShaderProrgam);
	TB->Create("TB");
	SF->Create("SF");
	SB->Create("SB");
	RL->Create("LR");
	RR->Create("RR");
	SBOTT->Create("SBOTT");
	RW->Create("RW");
	LW->Create("LW");
	CEIL->Create("CEIL");

	scene.push_back(CEIL);
	scene.push_back(LW);
	scene.push_back(RW);
	scene.push_back(SBOTT);
	scene.push_back(RR);
	scene.push_back(RL);
	scene.push_back(TF);
	scene.push_back(TB);
	scene.push_back(SF);
	scene.push_back(SB);
	scene.push_back(RL);

}

void Shapes::drawSquareGrid(glm::vec3& A, bool draw = true)
{
	if(!draw)
		return;
	auto w = 2.0f;
	auto squareGrid = std::make_shared<stu::SquareGrid>(A, 6, 6, w, w);
	squareGrid->Create();

	for (auto& point : squareGrid->points)
	{
		//point.z +=w;
		auto dot = std::make_shared<stu::Line>(point, glm::vec3(point.x, point.y, point.z + w));
		dot->Create();
		//wayPoints.push_back(dot);
	}
}

void Shapes::CreateStandAloneTriangle(glm::vec3& initialPoint, bool draw, std::vector<std::shared_ptr<stu::IPerFrame>> &scene, std::shared_ptr<stu::ShaderProgram> withInterimShaderProrgam)
{
	if(!draw)
		return;

	auto y = 0.0f;
	auto x = 0.0f;
	glm::vec3 A = glm::vec3(initialPoint.x+x, initialPoint.y+y, initialPoint.z);
	glm::vec3 B(-0.5f+x, 0.5f+y, 0.0f);
	glm::vec3 C(0.5f+x, 0.5f+y, 0.0f);
	glm::vec3 D(0.5f+x, -0.5f+y, 0.0f);
	glm::vec3 E(-0.7f+x, 0.5f+y, 0.0f);
	glm::vec3 F(0.7f+x, 0.5f+y, 0.0f);
	glm::vec3 G(0.0f+x, 1.2f+y, 0.0f);
	glm::vec3 E_ = glm::vec3(E.x, E.y, -1.0);
	glm::vec3 F_ = glm::vec3(F.x, F.y, -1.0);
	glm::vec3 G_ = glm::vec3(G.x, G.y, -1.0);

	glm::vec3 A_(-0.5f+x, -0.5f+y, -1.0f);
	glm::vec3 B_(-0.5f+x, 0.5f+y, -1.0f);
	glm::vec3 C_(0.5f+x, 0.5f+y, -1.0f);
	glm::vec3 D_(0.5f+x, -0.5f+y, -1.0f);
	glm::vec3 J = glm::vec3(E.x, E.y, E.z + 0.2f);
	glm::vec3 I = glm::vec3(G.x, G.y, G.z + 0.2f);
	glm::vec3 J_ = glm::vec3(E_.x, E_.y, E_.z - 0.2f);
	glm::vec3 I_ = glm::vec3(G_.x, G_.y, G_.z - 0.2f);
	glm::vec3 H = glm::vec3(F.x, F.y, F.z + 0.2f);
	glm::vec3 H_ = glm::vec3(F_.x, F_.y, F_.z - 0.2f);

	glm::vec3 K_ = D_; // SBott
	glm::vec3 L_ = A_;
	glm::vec3 M_ = D;
	glm::vec3 N_ = A;
	glm::vec3 R_ = B_; // Left wall
	glm::vec3 Q_ = B;
	glm::vec3 P_ = A_;
	glm::vec3 O_ = A;
	glm::vec3 Q = C; // Right wall
	glm::vec3 R = C_;

	glm::vec3 P = D_;
	glm::vec3 O = D;
	glm::vec3 S = F_; // Roof mouth (ceil)
	glm::vec3 T = E_;
	glm::vec3 U = E;
	glm::vec3 V = F;

		
	shared_ptr<stu::Triangle> ADB(new stu::Triangle(A, D, B));
	reinterpret_pointer_cast<stu::Triangle>(ADB)->Create();
	shared_ptr<stu::Triangle> DCB(new stu::Triangle(D, C, B));
	reinterpret_pointer_cast<stu::Triangle>(DCB)->Create();

	shared_ptr<stu::Triangle> D_A_C_(new stu::Triangle(D_, A_, C_));

	D_A_C_->Create();

	// new:
	shared_ptr<stu::Triangle> D_A_B_(new stu::Triangle(D_, A_, B_));
	reinterpret_pointer_cast<stu::Triangle>(D_A_B_)->Create();
	
	
	shared_ptr<stu::Triangle> A_B_C_(new stu::Triangle(A_, B_, C_));
	reinterpret_pointer_cast<stu::Triangle>(A_B_C_)->Create();

	shared_ptr<stu::Triangle> EFG(new stu::Triangle(E, F, G));
	reinterpret_pointer_cast<stu::Triangle>(EFG)->Create();
	shared_ptr<stu::Triangle> F_E_G_(new stu::Triangle(F_, E_, G_));
	reinterpret_pointer_cast<stu::Triangle>(F_E_G_)->Create();

	// new:
	//shared_ptr<stu::Triangle> F_E_G_(new stu::Triangle(F_, E_, G_));
	//reinterpret_pointer_cast<stu::Triangle>(F_E_G_)->Create();

	//Roofs

	shared_ptr<stu::Triangle> J_JI_(new stu::Triangle(J_, J, I_));
	reinterpret_pointer_cast<stu::Triangle>(J_JI_)->Create();

	shared_ptr<stu::Triangle> JII_(new stu::Triangle(J, I, I_));
	reinterpret_pointer_cast<stu::Triangle>(JII_)->Create();

	shared_ptr<stu::Triangle> HH_I(new stu::Triangle(H, H_, I));
	reinterpret_pointer_cast<stu::Triangle>(HH_I)->Create();

	shared_ptr<stu::Triangle> H_I_I(new stu::Triangle(H_, I_, I));
	reinterpret_pointer_cast<stu::Triangle>(H_I_I)->Create();

	//SBOT
	shared_ptr<stu::Triangle> M_N_K_(new stu::Triangle(M_, N_, K_));
	reinterpret_pointer_cast<stu::Triangle>(M_N_K_)->Create();

	shared_ptr<stu::Triangle> N_L_K_(new stu::Triangle(N_, L_, K_));
	reinterpret_pointer_cast<stu::Triangle>(N_L_K_)->Create();

	// left wall

	shared_ptr<stu::Triangle> P_O_R_(new stu::Triangle(P_, O_, R_));
	reinterpret_pointer_cast<stu::Triangle>(P_O_R_)->Create();

	shared_ptr<stu::Triangle> O_Q_R_(new stu::Triangle(O_, Q_, R_));
	reinterpret_pointer_cast<stu::Triangle>(O_Q_R_)->Create();

	// right wall
	shared_ptr<stu::Triangle> OPQ(new stu::Triangle(O, P, Q));
	reinterpret_pointer_cast<stu::Triangle>(OPQ)->Create();

	shared_ptr<stu::Triangle> PRQ(new stu::Triangle(P, R, Q));
	reinterpret_pointer_cast<stu::Triangle>(PRQ)->Create();

	// roof mouth
	shared_ptr<stu::Triangle> VUS(new stu::Triangle(V, U, S));
	reinterpret_pointer_cast<stu::Triangle>(VUS)->Create();

	shared_ptr<stu::Triangle> UTS(new stu::Triangle(U, T, S));
	reinterpret_pointer_cast<stu::Triangle>(UTS)->Create();

	// front wall
	scene.push_back(ADB);
	scene.push_back(DCB);


	// back wall
	scene.push_back(D_A_C_);
	scene.push_back(A_B_C_);

	//font triangle
	scene.push_back(EFG);

	// back triange
	scene.push_back(F_E_G_);

	// roofs- left
	scene.push_back(J_JI_);
	scene.push_back(JII_);

	// roofs- right
	scene.push_back(HH_I);
	scene.push_back(H_I_I);


	// bott
	scene.push_back(M_N_K_);
	scene.push_back(N_L_K_);


	// left wall

	scene.push_back(P_O_R_);
	scene.push_back(O_Q_R_);

	// right wall
	scene.push_back(OPQ);
	scene.push_back(PRQ);


	// roof mouth
	scene.push_back(VUS);
	scene.push_back(UTS);

	
	
	vector<shared_ptr<stu::Triangle>> faces = {ADB, DCB, OPQ, PRQ, EFG, D_A_B_, /* F_E_G_,  M_N_K_, N_L_K_,D_A_C_, D_A_C_,  OPQ, PRQ, P_O_R_, O_Q_R_ */};

	
	vector<shared_ptr<stu::Triangle>> allFaces;

		for (auto& renderable : scene)
		{
			auto tri = dynamic_pointer_cast<stu::Triangle>(renderable);
			allFaces.push_back(tri);
		}

	for( auto &face : allFaces)
	{
		face->UpdateVertexNormals(allFaces);

		auto factor = 0.1f;


		auto centoid = (face->lv + face->rv + face->tv) / 3.0f;
		auto maybeNormal = face->face_normal - face->tv;
		auto face_normal = face->face_normal * factor;
		
		bool useFacesAverage = false;
		
		
		auto lvstart = face->lv;
		auto rvstart = face->rv;
		auto tvstart = face->tv;

		auto lvend = face->lvn * factor;
		auto rvend = face->rvn * factor;		
		auto tvend = face->tvn * factor;

		if(!useFacesAverage)
		{
			lvend = lvstart + face_normal;
			rvend = rvstart + face_normal;
			tvend = tvstart + face_normal;
		}
		else
		{
			lvend = lvstart + glm::vec3(lvstart.x, lvstart.y, face_normal.z);
			rvend = rvstart + glm::vec3(rvstart.x, rvstart.y, face_normal.z);
			tvend = tvstart + glm::vec3(tvstart.x, tvstart.y, face_normal.z);
		}

		
		std::shared_ptr<stu::Line> lvline(new stu::Line(lvstart, lvend));
		std::shared_ptr<stu::Line> rvline(new stu::Line(rvstart, rvend));
		std::shared_ptr<stu::Line> tvline(new stu::Line(tvstart, tvend));

		std::shared_ptr<stu::Line> zeroline(new stu::Line(glm::vec3(0.0, 0.0, 0.0), face_normal));
		std::shared_ptr<stu::Line> centoidLine(new stu::Line(centoid, centoid + face_normal, glm::vec3(0.0f, 255.0f, 0.0f)));		

		centoidLine->Create();
		scene.push_back(centoidLine);

		lvline->Create();
		rvline->Create();
		tvline->Create();
		scene.push_back(lvline);
		scene.push_back(rvline);
		scene.push_back(tvline);		
		scene.push_back(face);
		
	}
	
}

void Shapes::CreateHouse(glm::vec3& initialPoint, bool draw, vector<std::shared_ptr<stu::IPerFrame>> &scene, std::shared_ptr<stu::ShaderProgram> &withInterimShaderProrgam)
{
	if(!draw) return;

	glm::vec3 A = initialPoint;
	glm::vec3 B(-0.5f, 0.5f, 0.0f);
	glm::vec3 C(0.5f, 0.5f, 0.0f);
	glm::vec3 D(0.5f, -0.5f, 0.0f);
	glm::vec3 E(-0.7f, 0.5f, 0.0f);
	glm::vec3 F(0.7f, 0.5f, 0.0f);
	glm::vec3 G(0.0f, 1.2f, 0.0f);
	glm::vec3 E_ = glm::vec3(E.x, E.y, -1.0);
	glm::vec3 F_ = glm::vec3(F.x, F.y, -1.0);
	glm::vec3 G_ = glm::vec3(G.x, G.y, -1.0);

	glm::vec3 A_(-0.5f, -0.5f, -1.0f);
	glm::vec3 B_(-0.5f, 0.5f, -1.0f);
	glm::vec3 C_(0.5f, 0.5f, -1.0f);
	glm::vec3 D_(0.5f, -0.5f, -1.0f);
	glm::vec3 J = glm::vec3(E.x, E.y, E.z + 0.2f);
	glm::vec3 I = glm::vec3(G.x, G.y, G.z + 0.2f);
	glm::vec3 J_ = glm::vec3(E_.x, E_.y, E_.z - 0.2f);
	glm::vec3 I_ = glm::vec3(G_.x, G_.y, G_.z - 0.2f);
	glm::vec3 H = glm::vec3(F.x, F.y, F.z + 0.2f);
	glm::vec3 H_ = glm::vec3(F_.x, F_.y, F_.z - 0.2f);

	glm::vec3 K_ = D_; // SBott
	glm::vec3 L_ = A_;
	glm::vec3 M_ = D;
	glm::vec3 N_ = A;
	glm::vec3 R_ = B_; // Left wall
	glm::vec3 Q_ = B;
	glm::vec3 P_ = A_;
	glm::vec3 O_ = A;
	glm::vec3 Q = C; // Right wall
	glm::vec3 R = C_;

	glm::vec3 P = D_;
	glm::vec3 O = D;
	glm::vec3 S = F_; // Roof mouth (ceil)
	glm::vec3 T = E_;
	glm::vec3 U = E;
	glm::vec3 V = F;

	shared_ptr<stu::IPerFrame> ADB(new stu::Triangle(A, D, B));
	reinterpret_pointer_cast<stu::Triangle>(ADB)->Create("ADB");
	shared_ptr<stu::IPerFrame> DCB(new stu::Triangle(D, C, B));
	reinterpret_pointer_cast<stu::Triangle>(DCB)->Create("DCB");

	shared_ptr<stu::IPerFrame> D_A_C_(new stu::Triangle(D_, A_, C_));
	reinterpret_pointer_cast<stu::Triangle>(D_A_C_)->Create("D'A'C'");
	shared_ptr<stu::IPerFrame> A_B_C_(new stu::Triangle(A_, B_, C_));
	reinterpret_pointer_cast<stu::Triangle>(A_B_C_)->Create("A'B'C'");

	shared_ptr<stu::IPerFrame> EFG(new stu::Triangle(E, F, G));
	reinterpret_pointer_cast<stu::Triangle>(EFG)->Create("EFG");
	shared_ptr<stu::IPerFrame> F_E_G_(new stu::Triangle(F_, E_, G_));
	reinterpret_pointer_cast<stu::Triangle>(F_E_G_)->Create("F'E'G'");

	//Roofs

	shared_ptr<stu::IPerFrame> J_JI_(new stu::Triangle(J_, J, I_));
	reinterpret_pointer_cast<stu::Triangle>(J_JI_)->Create("J'JI'");

	shared_ptr<stu::IPerFrame> JII_(new stu::Triangle(J, I, I_));
	reinterpret_pointer_cast<stu::Triangle>(JII_)->Create("JII'");

	shared_ptr<stu::IPerFrame> HH_I(new stu::Triangle(H, H_, I));
	reinterpret_pointer_cast<stu::Triangle>(HH_I)->Create("HH'I'");

	shared_ptr<stu::IPerFrame> H_I_I(new stu::Triangle(H_, I_, I));
	reinterpret_pointer_cast<stu::Triangle>(H_I_I)->Create("H'I'I");

	//SBOT
	shared_ptr<stu::IPerFrame> M_N_K_(new stu::Triangle(M_, N_, K_));
	reinterpret_pointer_cast<stu::Triangle>(M_N_K_)->Create("M'N'K'");

	shared_ptr<stu::IPerFrame> N_L_K_(new stu::Triangle(N_, L_, K_));
	reinterpret_pointer_cast<stu::Triangle>(N_L_K_)->Create("N'L'K'");

	// left wall

	shared_ptr<stu::IPerFrame> P_O_R_(new stu::Triangle(P_, O_, R_));
	reinterpret_pointer_cast<stu::Triangle>(P_O_R_)->Create("P'O'R'");

	shared_ptr<stu::IPerFrame> O_Q_R_(new stu::Triangle(O_, Q_, R_));
	reinterpret_pointer_cast<stu::Triangle>(O_Q_R_)->Create("O'Q'R'");

	// right wall
	shared_ptr<stu::IPerFrame> OPQ(new stu::Triangle(O, P, Q));
	reinterpret_pointer_cast<stu::Triangle>(OPQ)->Create("OPQ");

	shared_ptr<stu::IPerFrame> PRQ(new stu::Triangle(P, R, Q));
	reinterpret_pointer_cast<stu::Triangle>(PRQ)->Create("PRQ");

	// roof mouth
	shared_ptr<stu::IPerFrame> VUS(new stu::Triangle(V, U, S));
	reinterpret_pointer_cast<stu::Triangle>(VUS)->Create("VUS");

	shared_ptr<stu::IPerFrame> UTS(new stu::Triangle(U, T, S));
	reinterpret_pointer_cast<stu::Triangle>(UTS)->Create("UTS");

	// front wall
	scene.push_back(ADB);
	scene.push_back(DCB);

	// back wall
	scene.push_back(D_A_C_);
	scene.push_back(A_B_C_);

	//font triangle
	scene.push_back(EFG);

	// back triange
	scene.push_back(F_E_G_);

	// roofs- left
	scene.push_back(J_JI_);
	scene.push_back(JII_);

	// roofs- right
	scene.push_back(HH_I);
	scene.push_back(H_I_I);


	// bott
	scene.push_back(M_N_K_);
	scene.push_back(N_L_K_);


	// left wall
	scene.push_back(P_O_R_);
	scene.push_back(O_Q_R_);

	// right wall
	scene.push_back(OPQ);
	scene.push_back(PRQ);


	// roof mouth
	scene.push_back(VUS);
	scene.push_back(UTS);

	// calculate normals

	bool drawNormals = true;
	if (drawNormals)
	{
		vector<shared_ptr<stu::Triangle>> faces;

		for (auto& renderable : scene)
		{
			auto tri = dynamic_pointer_cast<stu::Triangle>(renderable);
			faces.push_back(tri);
		}

		int count = 0;
		for (const auto& face : faces)
		{
			//if(count == 1) continue;
			face->UpdateVertexNormals(faces);
			auto factor = 0.1f;

			auto lvs = face->lv;
			auto lve = face->lvn * 0.1f;

			auto rvs = face->rv;
			auto rve = face->rvn * 0.1f;

			auto tvs = face->tv;
			auto tve = face->tvn * 0.1f;
			std::shared_ptr<stu::Line> lvline(new stu::Line(lvs, lve));
			std::shared_ptr<stu::Line> rvline(new stu::Line(rvs, rve));
			std::shared_ptr<stu::Line> tvline(new stu::Line(tvs, tve));

			lvline->Create();
			rvline->Create();
			tvline->Create();
			scene.push_back(lvline);
			scene.push_back(rvline);
			scene.push_back(tvline);
			count++;
		}
	}
}

void Shapes::CreateTextureSquare(glm::vec3& A, bool draw = true)
{
	if(!draw)
		return;

	
	
	//textureBox = make_shared<stu::Box>();
	//textureBox->Create();
	
	
}
