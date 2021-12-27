#include "camera.h"
#include "gamewindow.h"
#include "stulib/events/GameObjectEvent.h"
#include "stulib/GameObject.h"

// Constructor for camera -- initialise with some default values
CCamera::CCamera(std::shared_ptr<gamelib::event_manager> event_admin)
{
	m_position = glm::vec3(0.0f, 10.0f, 100.0f);
	m_view = glm::vec3(0.0f, 0.0f, 0.0f);
	m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
	m_speed = 0.025f;
	m_cameraRotation = 0.0f;
	this->event_admin = event_admin;	
}
CCamera::~CCamera()
{}

void CCamera::Initialize()
{	
	auto event = std::make_shared<gamelib::GameObjectEvent>(1, nullptr, gamelib::GameObjectEventContext::FirstPersonView);
	event_admin->raise_event(event);
}
 
// Set the camera at a specific position, looking at the view point, with a given up vector
void CCamera::Set(glm::vec3 &position, glm::vec3 &viewpoint, glm::vec3 &upVector)
{		
	m_position = position;
	m_view = viewpoint;
	m_upVector = upVector;
}

// Respond to mouse movement
void CCamera::SetViewByMouse()
{  
	int middle_x = GameWindow::SCREEN_WIDTH >> 1;
	int middle_y = GameWindow::SCREEN_HEIGHT >> 1;

	float angle_y = 0.0f;
	float angle_z = 0.0f;
	static float rotation_x = 0.0f;

	POINT mouse;
	GetCursorPos(&mouse);

	if (mouse.x == middle_x && mouse.y == middle_y) {
		return;
	}

	SetCursorPos(middle_x, middle_y);

	angle_y = (float) (middle_x - mouse.x) / 10000.0f;
	angle_z = (float) (middle_y - mouse.y) / 10000.0f;

	rotation_x -= angle_z;

	float maxAngle = 1.56f; // Just a little bit below PI / 2

	if (rotation_x > maxAngle) {
		rotation_x = maxAngle;
	} else if (rotation_x < -maxAngle) {
		rotation_x = -maxAngle;
	} else {
		glm::vec3 cross = glm::cross(m_view - m_position, m_upVector);
		glm::vec3 axis = glm::normalize(cross);

		RotateViewPoint(angle_z, axis);
	}

	RotateViewPoint(angle_y, glm::vec3(0, 1, 0));
}

// Rotate the camera view point -- this effectively rotates the camera since it is looking at the view point
void CCamera::RotateViewPoint(float fAngle, const glm::vec3 &vPoint)
{
	glm::vec3 vView = m_view - m_position;
	
	glm::mat4 R = glm::rotate(glm::mat4(1), fAngle * 180.0f / (float) M_PI, vPoint);
	glm::vec4 newView = R * glm::vec4(vView, 1);

	m_view = m_position + glm::vec3(newView);
}

// Strafe the camera (side to side motion)
void CCamera::Strafe(double direction)
{
	float speed = (float) (m_speed * direction);

	m_position.x = m_position.x + m_strafeVector.x * speed;
	m_position.z = m_position.z + m_strafeVector.z * speed;

	m_view.x = m_view.x + m_strafeVector.x * speed;
	m_view.z = m_view.z + m_strafeVector.z * speed;
}

// Advance the camera (forward / backward motion)
void CCamera::Advance(double direction)
{
	float speed = (float) (m_speed * direction);
	glm::vec3 view = glm::normalize(m_view - m_position);
	
	m_view = m_view + view * speed;

	auto shouldConstrainY = false;

	if(shouldConstrainY)
	{
		auto newPosition = m_position + view * speed;
		auto clampToGroundLevel = glm::vec3(newPosition.x, 5.0f, newPosition.z);
		if(newPosition.y < 2.0f)
			m_position = clampToGroundLevel;
	}
	else
	{
		m_position = m_position + view * speed;
	}
}

// Update the camera to respond to mouse motion for rotations and keyboard for translation
void CCamera::Update(double dt)
{
	glm::vec3 vector = glm::cross(m_view - m_position, m_upVector);
	m_strafeVector = glm::normalize(vector);

	SetViewByMouse();
	TranslateByKeyboard(dt);
}


// Update the camera to respond to key presses for translation
void CCamera::TranslateByKeyboard(double dt)
{
	if (GetKeyState('W') & 0x80) {
		Advance(1.0*dt);
	}

	if (GetKeyState('S') & 0x80) {
		Advance(-1.0*dt);
	}

	if (GetKeyState('A') & 0x80) {
		Strafe(-1.0*dt);
		m_cameraRotation -= m_rotationFactor;//1.0 *dt;
	}

	if (GetKeyState('D') & 0x80) {
		Strafe(1.0*dt);
		m_cameraRotation += m_rotationFactor;// *dt;
	}
	if (GetKeyState('M') & 0x80) {
		 m_position.y += 1.0 *dt;
	}
	if (GetKeyState('E') & 0x80) {
		 m_position.y -= 1.0 *dt;
	}
}
// Return the camera position
glm::vec3 CCamera::GetPosition() const
{
	return m_position;
}

float CCamera::GetRotation() const
{
	return m_cameraRotation;
}
// Return the camera view point
glm::vec3 CCamera::GetView() const
{
	return m_view;
}

// Return the camera up vector
glm::vec3 CCamera::GetUpVector() const
{
	return m_upVector;
}

// Return the camera strafe vector
glm::vec3 CCamera::GetStrafeVector() const
{
	return m_strafeVector;
}

// Return the camera perspective projection matrix
glm::mat4* CCamera::GetPerspectiveProjectionMatrix()
{
	return &m_perspectiveProjectionMatrix;
}

// Return the camera orthographic projection matrix
glm::mat4* CCamera::GetOrthographicProjectionMatrix()
{
	return &m_orthographicProjectionMatrix;
}

// Set the camera perspective projection matrix to produce a view frustum with a specific field of view, aspect ratio, 
// and near / far clipping planes
void CCamera::SetPerspectiveProjectionMatrix(float fov, float aspectRatio, float nearClippingPlane, float farClippingPlane)
{
	m_perspectiveProjectionMatrix = glm::perspective(fov, aspectRatio, nearClippingPlane, farClippingPlane);
}

// The the camera orthographic projection matrix to match the width and height passed in
void CCamera::SetOrthographicProjectionMatrix(int width, int height)
{
	m_orthographicProjectionMatrix = glm::ortho(0.0f, float(width), 0.0f, float(height));
}

// Get the camera view matrix
glm::mat4 CCamera::GetViewMatrix()
{
	return glm::lookAt(m_position, m_view, m_upVector);
}

// The normal matrix is used to transform normals to eye coordinates -- part of lighting calculations
glm::mat3 CCamera::ComputeNormalMatrix(const glm::mat4 &modelViewMatrix)
{
	return glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));
}

