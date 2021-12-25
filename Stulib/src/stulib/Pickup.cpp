#include "pch.h"
#include "Pickup.h"
#include "events/GameObjectEvent.h"
#include "freeimage/FreeImage.h"
#include "Triangle.h"
#include "Square.h"
#include "StuTexture.h"

Pickup::Pickup(std::string name, shared_ptr<stu::ShaderProgram> program, std::shared_ptr<gamelib::event_manager> event_admin, std::shared_ptr<stu::Texture> texture, glm::vec3 initialOffset)
	: GameObject(name, program, event_admin, initialOffset)
{
	this->texture = texture;
}

void Pickup::Create()
{
	event_admin->subscribe_to_event(gamelib::event_type::GameObject, shared_from_this());
	
	// Create a Pickup which is just composed of the lower portion of the house - it just a cube...
	renderables = std::vector<std::shared_ptr<IPerFrame>>();

	auto A = glm::vec3(-0.5f, -0.5f, 0.0f);
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
	
	
	
	auto SF = make_shared<stu::Square>(*ADB, *DCB, texture, shaderProgram);	
	auto SB = make_shared<stu::Square>(*D_A_C_, *A_B_C_, texture, shaderProgram);	
	auto SBOTT = make_shared<stu::Square>(*M_N_K_, *N_L_K_, texture, shaderProgram);
	auto RW = make_shared<stu::Square>(*OPQ, *PRQ, texture, shaderProgram);
	auto LW = make_shared<stu::Square>(*P_O_R_, *O_Q_R_, texture, shaderProgram);
	auto STOP = make_shared<stu::Square>(*KLM, *LNM, texture, shaderProgram);
	
		
	SF->Create("SF");
	SB->Create("SB");	

	RW->Create("RW");
	LW->Create("LW");
	STOP->Create("STOP");
	SBOTT->Create("SBOTT");

	renderables.push_back(LW);
	renderables.push_back(RW);
	renderables.push_back(SBOTT);
	renderables.push_back(SF);
	renderables.push_back(SB);
	renderables.push_back(STOP);
}

glutil::MatrixStack Pickup::Transform(glutil::MatrixStack& modelViewMatrixStack)
{
	
	this->currentMatrix = GameObject::Transform(modelViewMatrixStack).Top();
	return modelViewMatrixStack;
}

void Pickup::Render()
{
	GameObject::Render();

	for(auto &renderable : renderables)
		renderable->Render();
}

void Pickup::Update(float delta)
{
	//rotationAngle += delta * 0.002;
}

std::vector<std::shared_ptr<gamelib::event>> Pickup::handle_event(std::shared_ptr<gamelib::event> evt)
{
	/// <summary>
	///  Not handling any events right now
	/// </summary>
	/// <param name="evt"></param>
	/// <returns></returns>
	std::vector<std::shared_ptr<gamelib::event>> events;
	return events;
}

std::string Pickup::get_subscriber_name()
{
	return "Pickup";
}

GameObject::GameObjectType Pickup::GetType()
{
	return GameObjectType::Pickup;
}

