#include "pch.h"
#include "TNBHelper.h"



stu::TNBHelper::TNBHelper(glm::vec3 currentPosition, glm::vec3 nextPosition)
{
		
	T = glm::normalize(nextPosition - currentPosition); // forward vector
	y = glm::vec3(0.0f, 1.0f, 0.0);
	N = glm::normalize(glm::cross(T, y)); // side vector
	B = glm::normalize(glm::cross(N, T)); // up vector
}
