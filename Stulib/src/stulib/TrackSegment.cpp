#include "pch.h"
#include "TrackSegment.h"

TrackSegment::TrackSegment(std::string name, 
    glm::vec3& A, 
    glm::vec3& D, 
    glm::vec3& B, 
    glm::vec3& C, 
    std::shared_ptr<stu::Texture> texture, 
    std::shared_ptr<stu::ShaderProgram> shaderProgram, 
    std::shared_ptr<gamelib::event_manager> event_admin)
    : GameObject(name, shaderProgram, event_admin, A)
{
    auto ADB = stu::Triangle(A, D, B);
	auto DCB = stu::Triangle(D, C, B);
    this->texture = texture;

    square = std::make_shared<stu::Square>(ADB, DCB, texture, shaderProgram);
}

void TrackSegment::SetTextureCoordinates(glm::vec2 tca, glm::vec2 tcd, glm::vec2 tcc, glm::vec2 tcb)
{
	square->SetTextureCoordinates(tca, tcd, tcc, tcb);
}
void TrackSegment::SetDrawNormals(bool yesno)
{
    this->shouldDrawNormals = yesno;
}

std::vector<std::shared_ptr<gamelib::event>> TrackSegment::handle_event(std::shared_ptr<gamelib::event> evt)
{
    return std::vector<std::shared_ptr<gamelib::event>>();
}

std::string TrackSegment::get_subscriber_name()
{
    return std::string();
}


glutil::MatrixStack  TrackSegment::Transform(glutil::MatrixStack& modelViewMatrixStack)
{
    // We are already in word co-ordinates, no need to transform
    return modelViewMatrixStack;
}

void TrackSegment::Create()
{
    GameObject::Create();
        square->Create();
}

void TrackSegment::Update(float delta)
{
    GameObject::Update(delta);
    square->Update(delta);
}

void TrackSegment::Render()
{
    GameObject::Render();
    square->SetDrawNormals(shouldDrawNormals);
    square->Render();
}

GameObject::GameObjectType TrackSegment::GetType()
{
    return GameObjectType::TrackSegment;
}
