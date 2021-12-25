#pragma once
#include "GameObject.h"
#include "TNBHelper.h"
#include "PlayerCollisionEvent.h"
#include "cgtlib/HighResolutionTimer.h"


class Player : public GameObject
{
    glm::mat4 playerOrientation;
    bool turning = false;
    Direction turningDirection = Direction::Undefined;
    CHighResolutionTimer directionResetTimer;
    float directionOrentationScale = 0.2;
public:
    Player(std::string name, 
            std::string meshPath, 
            std::shared_ptr<stu::ShaderProgram> shaderProgram,
            std::shared_ptr<gamelib::event_manager> event_admin, 
            glm::vec3 initialOffset = glm::vec3(0.0f, 0.0f, 5.0f));

    glm::vec3 direction = glm::vec3(0.0, 0.0, -1.0); // initially facing towards -z direction
    float centerDeviation = 0.0f;

    // Inherited via GameObject
    virtual std::vector<std::shared_ptr<gamelib::event>> handle_event(std::shared_ptr<gamelib::event> evt) override;
    virtual std::string get_subscriber_name() override;
    virtual glutil::MatrixStack  Transform(glutil::MatrixStack& modelViewMatrixStack) override;
    void SetOrientation(glm::mat4 orientation, stu::TNBHelper tnb);
    virtual void Create() override;

    std::vector<shared_ptr<GameObject>> Inventory;
    
	void SetDirection(glm::vec3 direction);
    glm::vec3 GetDirection();
    
    GameObjectType GetType() override;

    void Update(float delta) override;

    int GetPoints();
};

