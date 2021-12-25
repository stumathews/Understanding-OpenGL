#pragma once
#include "Renderable.h"
namespace stu
{
	class Star : public Renderable
	{
		// Inherited via Renderable
		virtual void Create() override;
		virtual void Update(float delta) override;
		virtual void Render() override;
	};
}
