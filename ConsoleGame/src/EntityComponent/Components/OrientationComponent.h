#pragma once

#include "EOrientation.h"

class OrientationComponent
{
public:
	OrientationComponent() : mOrientation(EOrientation_FaceUp)						{ }
	OrientationComponent(EOrientation inOrientation) : mOrientation(inOrientation)	{ }
	~OrientationComponent()															{ }

	void			SetOrientation(EOrientation inOrientation)	{ mOrientation = inOrientation; }
	EOrientation	GetOrientation() const						{ return mOrientation;			}

private:
	EOrientation	mOrientation;
};
