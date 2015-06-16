#pragma once

#include <Maths/IVec2.h>
#include <Maths/IRect.h>

#include "Dynamic2DVector.h"

template<typename T>
class Dynamic2DVectorWithOffset
{
public:
	Dynamic2DVectorWithOffset() { }
	Dynamic2DVectorWithOffset(int inCols, int inRows);
	Dynamic2DVectorWithOffset(int inCols, int inRows, const T& inInit, const IVec2& inCentreOffset = IVec2(0, 0));
	Dynamic2DVectorWithOffset(Dynamic2DVectorWithOffset<T>&& inRHS);
	Dynamic2DVectorWithOffset(const Dynamic2DVectorWithOffset<T>& inRHS);
	virtual ~Dynamic2DVectorWithOffset()  {}

	void				Set(size_t inX, size_t inY, const T& inData)	{ mVector.Set(inX - mCentreOffset.mX, inY - mCentreOffset.mY, inData);	}
	const T&			Get(size_t inX, size_t inY) const				{ return mVector.Get(inX - mCentreOffset.mX, inY - mCentreOffset.mY);	}
	void				Clear()											{ mVector.Clear(); }

	size_t				GetRowCount() const								{ return mVector.GetRowCount(); }
	size_t				GetColCount() const								{ return mVector.GetColCount(); }
	
	void				SetCentreOffset(const IVec2& inOffset)			{ mCentreOffset = inOffset; }
	const IVec2&		GetCentreOffset() const							{ return mCentreOffset;		}
	
	IRect				GetLocalBounds() const							{ return IRect(mCentreOffset.mX, mCentreOffset.mY, mVector.GetColCount(), mVector.GetRowCount()); }

	template<typename TFunctor>
	void				ForEach(const TFunctor& inFunctor) const;
	
private:
	Dynamic2DVector<T>	mVector;
	IVec2				mCentreOffset;
};

template<typename T>
Dynamic2DVectorWithOffset<T>::Dynamic2DVectorWithOffset(int inCols, int inRows)
	: mVector(inRows, inCols)
{
}
	
template<typename T>
Dynamic2DVectorWithOffset<T>::Dynamic2DVectorWithOffset(int inCols, int inRows, const T& inInit, const IVec2& inCentreOffset)	
	: mVector		(inRows, inCols, inInit)
	, mCentreOffset	(inCentreOffset)
{
}
	
template<typename T>
Dynamic2DVectorWithOffset<T>::Dynamic2DVectorWithOffset(Dynamic2DVectorWithOffset<T>&& inRHS)
	: mVector		(std::move(inRHS.mVector))
	, mCentreOffset	(inRHS.mCentreOffset)
{
}
	
template<typename T>
Dynamic2DVectorWithOffset<T>::Dynamic2DVectorWithOffset(const Dynamic2DVectorWithOffset<T>& inRHS)
	: mVector		(inRHS.mVector)
	, mCentreOffset	(inRHS.mCentreOffset)
{
}

template<typename T>
template<typename TFunctor>
void Dynamic2DVectorWithOffset<T>::ForEach(const TFunctor& inFunctor) const
{
	mVector.ForEach( [&] (int inX, int inY, const T& inValue)
	{
		int x = inX + mCentreOffset.mX;
		int y = inY + mCentreOffset.mY;
		inFunctor(x, y, inValue);
	} );
}