#pragma once
namespace stu
{
	/// <summary>
	/// Objects that want to participate in the Create -> Update() -> Render() process should inherit from this class
	/// and implement the interface below
	/// </summary>
	class IPerFrame
	{	
	public:
		virtual void Create() = 0;
		virtual void Update(float delta) = 0;
		virtual void Render() = 0;

	};
}

