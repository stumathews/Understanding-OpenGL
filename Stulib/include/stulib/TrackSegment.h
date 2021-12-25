#pragma once
#include "stulib/GameObject.h"
#include "stulib/events/Event.h"
#include "glm/detail/type_vec.hpp"
#include "stulib/StuTexture.h"
#include "stulib/Square.h"
class TrackSegment :public GameObject
{
	shared_ptr<stu::Square> square;
	std::shared_ptr<stu::Texture> texture;
	bool shouldDrawNormals = false;
public:
	TrackSegment(std::string name, 
		glm::vec3 &A, 
		glm::vec3 &D,
		glm::vec3 &B, 
		glm::vec3 &C, 
		std::shared_ptr<stu::Texture> texture, 
		std::shared_ptr<stu::ShaderProgram> shaderProgram,  
		std::shared_ptr<gamelib::event_manager> event_admin);

	void SetTextureCoordinates(glm::vec2 tca, glm::vec2 tcd, glm::vec2 tcc, glm::vec2 tcb);

	void SetDrawNormals(bool yesno);
	
	// Inherited via GameObject
	virtual std::vector<std::shared_ptr<gamelib::event>> handle_event(std::shared_ptr<gamelib::event> evt) override;
	virtual std::string get_subscriber_name() override;
	virtual glutil::MatrixStack  Transform(glutil::MatrixStack& modelViewMatrixStack) override;
	virtual void Create() override;
	virtual void Update(float delta) override;		
	virtual void Render() override;
	virtual GameObjectType GetType() override;
};

