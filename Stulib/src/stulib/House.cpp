#include "pch.h"
#include "House.h"
#include "events/GameObjectEvent.h"
#include "Triangle.h"
#include "Square.h"

House::House(std::string name, 
	std::shared_ptr<stu::ShaderProgram> shaderProgram,
	std::shared_ptr<gamelib::event_manager> event_admin,
	std::shared_ptr<stu::Texture> wallTexture,
	std::shared_ptr<stu::Texture> roofTexture,
	glm::vec3 initialOffset) 
	: GameObject(name, shaderProgram, event_admin, initialOffset) 
{
	this->roofTexture = roofTexture;
	this->wallTexture = wallTexture;
}

void House::Create()
{
	// A house is constructed from a set of primatives where are all IPerFrames
	renderables = std::vector<std::shared_ptr<stu::Renderable>>();

	auto A = initialPoint;
	glm::vec3 red(255.0f, 0.0f, 0.0f);

	// Construct the house by arranging its vertices into primatives...

	glm::vec3 B(-0.5f, 0.5f, 0.0f);
	glm::vec3 C(0.5f, 0.5f, 0.0f);
	glm::vec3 D(0.5f, -0.5f, 0.0f);
	glm::vec3 E(-0.7f, 0.5f, 0.0f);
	glm::vec3 F(0.7f, 0.5f, 0.0f);
	glm::vec3 G(0.0f, 1.2f, 0.0f);
	glm::vec3 E_ = glm::vec3(E.x, E.y, -1.0); // _ versions are back positons of the front versions (without _)
	glm::vec3 F_ = glm::vec3(F.x, F.y, -1.0);
	glm::vec3 G_ = glm::vec3(G.x, G.y, -1.0);

	glm::vec3 A_(-0.5f, -0.5f, -1.0f);
	glm::vec3 B_(-0.5f, 0.5f, -1.0f);
	glm::vec3 C_(0.5f, 0.5f, -1.0f);
	glm::vec3 D_(0.5f, -0.5f, -1.0f);

	// Some positions are just shared with other vertices but we give them diffirent names to track 
	// which face they belong to and where they are

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

	// Construct the faces from the vertices into dedicated primatives.
	// We use Triangles and Squares exclusively to represent a house's faces:

	// square front
	shared_ptr<stu::Triangle> ADB(new stu::Triangle(A, D, B));
	shared_ptr<stu::Triangle> DCB(new stu::Triangle(D, C, B));

	// square back
	shared_ptr<stu::Triangle> D_A_C_(new stu::Triangle(D_, A_, C_));
	shared_ptr<stu::Triangle> A_B_C_(new stu::Triangle(A_, B_, C_));

	// triangle front
	shared_ptr<stu::Triangle> EFG(new stu::Triangle(E, F, G, red, red, red)); // we can optionally specify the vertex colours if we decide to use them later 

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

	// We need to 'merge' the triangles above into squares if we want to stretch a texture over the two triangles.
	// otherwise rendering each triangle indpendantly with a texture stetchs the texture only over its 3 vertices.
	// We want 4 vertices, we want a square from the triangles.

	// Use the texture associated with this Game Object, also pass in the the shader program of this Game Object - we
	// use this to help use determine which vertices layout we should use - each shader program has a diffient
	// vertex layout it can decide to create/use depending on the supporting shader passed in here.s


	// Make 'merged' squares from triangles:
	auto SF = make_shared<stu::Square>(*ADB, *DCB, wallTexture, shaderProgram);	
	auto SB = make_shared<stu::Square>(*D_A_C_, *A_B_C_, wallTexture, shaderProgram);
	auto RL = make_shared<stu::Square>(*J_JI_, *JII_, roofTexture, shaderProgram);	
	auto RR = make_shared<stu::Square>(*HH_I, *H_I_I, roofTexture, shaderProgram);		
	auto SBOTT = make_shared<stu::Square>(*M_N_K_, *N_L_K_, wallTexture, shaderProgram);
	auto RW = make_shared<stu::Square>(*OPQ, *PRQ, wallTexture, shaderProgram);
	auto LW = make_shared<stu::Square>(*P_O_R_, *O_Q_R_, wallTexture, shaderProgram);
	auto CEIL = make_shared<stu::Square>(*VUS, *UTS, roofTexture, shaderProgram);
	auto TF = EFG;
	auto TB = F_E_G_;

	// Show under overhangs
	RR->SetDisableBackFaceCulling(true);
	RL->SetDisableBackFaceCulling(true);

	TF->SetDrawNormals(showNormals);
	TB->SetDrawNormals(showNormals);	
	SF->SetDrawNormals(showNormals);
	SB->SetDrawNormals(showNormals);
	RL->SetDrawNormals(showNormals);
	RR->SetDrawNormals(showNormals);
	SBOTT->SetDrawNormals(showNormals);
	RW->SetDrawNormals(showNormals);
	LW->SetDrawNormals(showNormals);
	CEIL->SetDrawNormals(showNormals);

	// Triangles:

	// Triangles dont set the texture in the constructor, they need to be manually told they must support texturing
	// over their 3 vertices.
	TF->setTexture(roofTexture, shaderProgram);
	TF->Create("TF");
	TB->setTexture(roofTexture, shaderProgram);
	TB->Create("TB");

	// Create() Squares
	SF->Create("SF");
	SB->Create("SB");
	RL->Create("LR");
	RR->Create("RR");
	SBOTT->Create("SBOTT");
	RW->Create("RW");
	LW->Create("LW");
	CEIL->Create("CEIL");

	

	// Add to list of all primatives that make up this house. This is used in Render() to render each primative 
	renderables.push_back(CEIL);
	renderables.push_back(LW);
	renderables.push_back(RW);
	renderables.push_back(SBOTT);
	renderables.push_back(RR);
	renderables.push_back(RL);
	renderables.push_back(TF);
	renderables.push_back(TB);
	renderables.push_back(SF);
	renderables.push_back(SB);
	renderables.push_back(RL);

	// Oh, and I'd like to subscribe to any GameObject event please on the event bus.
	event_admin->subscribe_to_event(gamelib::event_type::GameObject, shared_from_this());
}

void House::Update(float delta) 
{
	// Update any of our primatives if they need updating
	for (auto& renderable : renderables)
		renderable->Update(delta);
}

void House::Render()
{
	
	GameObject::Render();

	// This allows us to see the underside of the overhangs etc
	

	// Render all our parts of the house
	for (auto& renderable : renderables)
	{
		// turn it on
		if(renderable->ShouldDisableBackFaceCulling())
			glDisable(GL_CULL_FACE);

		renderable->Render();

		// turn if off
		if(renderable->ShouldDisableBackFaceCulling())
			glEnable(GL_CULL_FACE);
	}
	
}

glutil::MatrixStack House::Transform(glutil::MatrixStack& modelViewMatrixStack)
{
	return GameObject::Transform(modelViewMatrixStack);
	// We will move ourself by default to a particualr world coordinate by default (this can be changed outside the object at any point)
	//modelViewMatrixStack.Translate(glm::vec3(0.0f, 10.0f, 50.0f));

	// We will scale ourselves to x20 by default, unless this is changed by the outside
	//modelViewMatrixStack.Scale(20.0f);
}

std::vector<std::shared_ptr<gamelib::event>> House::handle_event(std::shared_ptr<gamelib::event> evt)
{
	// Not handling any events yet
	auto events = std::vector<std::shared_ptr<gamelib::event>>();
	if(evt->type == gamelib::event_type::GameObject)
	{
		const auto gameObjectEvent = dynamic_pointer_cast<gamelib::GameObjectEvent>(evt);
		switch(gameObjectEvent->context)
		{
			case gamelib::GameObjectEventContext::TestEvent:
				int i = 0;
				break;
		}
	}
	return events;
}

std::string House::get_subscriber_name()
{
	return std::string();
}

GameObject::GameObjectType House::GetType() { return GameObjectType::House; }

void House::SetShowNormals(bool yesNo)
{
	this->showNormals = yesNo;
}
