#pragma once

/// <summary>
/// Options that can be set per frame for IPerfame objects
/// </summary>
class PerFrameOptions
{
public:
	PerFrameOptions(bool RenderWireframe = false);
	bool RenderWireframe;
};
