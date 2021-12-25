#include "pch.h"
#include "Renderable.h"


/// <summary>
/// Let user set if normals should be drawn ot not
/// </summary>
/// <param name="yesno"></param>

void stu::Renderable::SetDrawNormals(bool yesno)
{
	this->drawNormals = yesno;
}

void stu::Renderable::SetDisableBackFaceCulling(bool yesno)
{
	this->disableBackFaceCulling = yesno;
}

bool stu::Renderable::ShouldDrawNormals()
{
	return drawNormals;
}

bool stu::Renderable::ShouldDisableBackFaceCulling()
{
	return disableBackFaceCulling;
}
