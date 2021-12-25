#include "pch.h"
#include "Player.h"
#include "PlayerCollisionEvent.h"
#include "events/GameObjectEvent.h"
#include "Pickup.h"

Player::Player(std::string name, 
    std::string meshPath,
     std::shared_ptr<stu::ShaderProgram> shaderProgram,
    std::shared_ptr<gamelib::event_manager> event_admin,  glm::vec3 initialOffset) 
    : GameObject(name, meshPath, shaderProgram, event_admin, initialOffset)
{ 
}

// Static initializer
std::map<Pickup::PickupProperties, std::string> Pickup::WellKnownPropertyKeys = 
{  
    std::make_pair(PickupProperties::PickupValue, std::string("Value")) 
};


std::vector<std::shared_ptr<gamelib::event>> Player::handle_event(std::shared_ptr<gamelib::event> event)
{
    auto createdEvents = std::vector<std::shared_ptr<gamelib::event>>();

    // let the base class handle the event
    for(auto &newBaseEvents : GameObject::handle_event(event))
        createdEvents.push_back(newBaseEvents);
   
    if(gamelib::event_type::GameObject == event->type)
    {
        const auto gameObjectEvent = std::dynamic_pointer_cast<gamelib::GameObjectEvent>(event);
        switch(gameObjectEvent->context)
        {
            case gamelib::GameObjectEventContext::MoveRight:
                centerDeviation++;
                turningDirection = Direction::Right;
                directionResetTimer.Start();
            break;
            case gamelib::GameObjectEventContext::MoveLeft:
                centerDeviation--;
                 turningDirection = Direction::Left;
                 directionResetTimer.Start();
            break;
        }
    }

    if(gamelib::event_type::PlayerCollision == event->type)
    {
        const auto playerCollisionEvent = dynamic_pointer_cast<PlayerCollisionEvent>(event);  
        const auto collidedWith =  playerCollisionEvent->collider;       

        // We can get multiple events while the object is touching the player so ignore duplicate events...
        if(std::find(begin(Inventory), end(Inventory), collidedWith) == end(Inventory))
        {
             // pick up object
            Inventory.push_back(collidedWith);
        
            // What type of object is this?
            switch(collidedWith->GetType())
            {
                // Oh yippee its a pickup, read its values and update our internal state.
                case GameObjectType::Pickup:
                    PropertiesInt["Points"] += collidedWith->PropertiesInt[Pickup::WellKnownPropertyKeys[Pickup::PickupProperties::PickupValue]];
                    break;
            }
        }
    }
    
    return createdEvents;
}

void Player::SetDirection(glm::vec3 direction)
{
	this->direction = direction;
}

glm::vec3 Player::GetDirection()
{
    return direction;
}

GameObject::GameObjectType Player::GetType()
{
    return GameObjectType::Player;
}

void Player::Update(float delta)
{
    if(directionResetTimer.ElapsedSeconds() > 0.2)
        turningDirection = Direction::Undefined;    
}

int Player::GetPoints() 
{
    return PropertiesInt["Points"];
}

std::string Player::get_subscriber_name()
{
    return std::string();
}

glutil::MatrixStack  Player::Transform(glutil::MatrixStack& modelViewMatrixStack)
{
    modelViewMatrixStack = GameObject::Transform(modelViewMatrixStack);
    modelViewMatrixStack *= playerOrientation;
    return modelViewMatrixStack;
}

void Player::SetOrientation(glm::mat4 orientation, stu::TNBHelper tnb)
{
    if(turningDirection == Direction::Right)
	    orientation =  tnb.RotateB(orientation, -1 * directionOrentationScale); 
    if(turningDirection == Direction::Left)
        orientation =  tnb.RotateB(orientation, 1 * directionOrentationScale);    

    playerOrientation = orientation;
}

void Player::Create()
{
    // player needs to be itself a shared pointer for this to work
    event_admin->subscribe_to_event(gamelib::event_type::GameObject, shared_from_this());
    event_admin->subscribe_to_event(gamelib::event_type::PlayerCollision, shared_from_this());
    GameObject::Create();
}



