#pragma once
#include "Common.h"

namespace stu
{
	class TNBHelper
	{
		
		glm::vec3 T, y, N, B;
	public:
		enum class Axis {T, N, B};
		TNBHelper(glm::vec3 currentPosition, glm::vec3 nextPosition);

		glm::vec3 MovePositionAlongAxisBy(glm::vec3 yourPosition,  Axis axis, float by)
		{	

			yourPosition += by * GetAxisVector(axis);
			return yourPosition;
		}

		glm::vec3 GetAxisVector(Axis axis)
		{
			glm::vec3 AxisVector;

			if(axis == Axis::T)
				AxisVector = T;

			if(axis == Axis::N)
				AxisVector = N;

			if(axis == Axis::B)
				AxisVector = B;

			return AxisVector;
		}

		glm::mat4 RotateAboutAxis(glm::mat4 matrix, Axis axis, float by)
		{
			return glm::rotate(matrix, by, GetAxisVector(axis));
		}

		glm::mat4 RotateT(glm::mat4 matrix, float by)
		{
			return RotateAboutAxis(matrix, Axis::T, by);
		}

		glm::mat4 RotateN(glm::mat4 matrix, float by)
		{
			return RotateAboutAxis(matrix, Axis::N, by);
		}

		glm::mat4 RotateB(glm::mat4 matrix, float by)
		{
			return RotateAboutAxis(matrix, Axis::B, by);
		}

		glm::vec3 MoveT(float by, glm::vec3 yourPosition)
		{
			return MovePositionAlongAxisBy(yourPosition, Axis::T, by);
		}
		glm::vec3 MoveN(float by, glm::vec3 yourPosition)
		{
			return MovePositionAlongAxisBy(yourPosition, Axis::N, by);
		}

		glm::vec3 MoveB(float by, glm::vec3 yourPosition)
		{
			return MovePositionAlongAxisBy(yourPosition, Axis::B, by);
		}
	};
}
