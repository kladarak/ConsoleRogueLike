#pragma once

#include <functional>
#include <vector>

class Entity;

class ProgramComponent
{
public:
	ProgramComponent()	{ }
	~ProgramComponent()	{ }
	ProgramComponent(ProgramComponent&& inRHS)		: mPrograms( std::move(inRHS.mPrograms) )	{ }
	ProgramComponent(const ProgramComponent& inRHS)	: mPrograms( inRHS.mPrograms )				{ }

	typedef std::function<void (const Entity& inThis, float inFrameTime)> ProgramFunc;
	void								RegisterProgram(const ProgramFunc& inProgram)	{ mPrograms.push_back(inProgram); }
	
	const std::vector<ProgramFunc>&		GetPrograms() const								{ return mPrograms; }
private:
	std::vector<ProgramFunc>			mPrograms;
};
