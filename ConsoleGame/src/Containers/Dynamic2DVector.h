#pragma once

#include <vector>
#include <Core/Debug/Assert.h>
#include <Maths/IVec2.h>

template<typename T>
class Dynamic2DVector
{
public:
	Dynamic2DVector() { }
	Dynamic2DVector(int inCols, int inRows)						: mData(inRows, std::vector<T>(inCols))			{ }
	Dynamic2DVector(int inCols, int inRows, const T& inInit)	: mData(inRows, std::vector<T>(inCols, inInit)) { }
	Dynamic2DVector(Dynamic2DVector&& inRHS)					: mData( std::move(inRHS.mData) )				{ }
	Dynamic2DVector(const Dynamic2DVector& inRHS)				: mData( inRHS.mData )							{ }

	void										Set(size_t inX, size_t inY, const T& inData);
	typename std::vector<T>::const_reference	Get(size_t inX, size_t inY) const;
	typename std::vector<T>::reference			Get(size_t inX, size_t inY);

	void										Set(const IVec2& inIndex, const T& inData)	{ Set(inIndex.mX, inIndex.mY, inData);	}
	typename std::vector<T>::const_reference	Get(const IVec2& inIndex) const				{ return Get(inIndex.mX, inIndex.mY);	}
	typename std::vector<T>::reference			Get(const IVec2& inIndex)					{ return Get(inIndex.mX, inIndex.mY);	}

	void										Clear();

	size_t										GetRowCount() const							{ return mData.size(); }
	size_t										GetColCount() const							{ return (mData.size() > 0) ? mData[0].size() : 0; }

	template<typename TFunctor>
	void										ForEach(const TFunctor& inFunctor) const;
	
private:
	std::vector< std::vector<T> >				mData;
};

template<typename T>
void Dynamic2DVector<T>::Set(size_t inX, size_t inY, const T& inData)
{
	if ( inY >= GetRowCount() )
	{
		size_t oldCount = GetRowCount();
		size_t colCount = GetColCount();

		mData.resize( inY + 1 );

		for (size_t i = oldCount; i < mData.size(); ++i)
		{
			mData[i].resize(colCount);
		}
	}

	if ( inX >= GetColCount() )
	{
		for (auto& row : mData)
		{
			row.resize( inX + 1 );
		}
	}

	mData[inY][inX] = inData;
}

template<typename T>
typename std::vector<T>::const_reference Dynamic2DVector<T>::Get(size_t inX, size_t inY) const
{
	assert(inX < GetColCount());
	assert(inY < GetRowCount());
	
	return mData[inY][inX];
}

template<typename T>
typename std::vector<T>::reference Dynamic2DVector<T>::Get(size_t inX, size_t inY)
{
	assert(inX < GetColCount());
	assert(inY < GetRowCount());
	
	return mData[inY][inX];
}

template<typename T>
void Dynamic2DVector<T>::Clear()
{
	mData.clear();
}

template<typename T>
template<typename TFunctor>
void Dynamic2DVector<T>::ForEach(const TFunctor& inFunctor) const
{
	for (size_t row = 0; row < mData.size(); ++row)
	{
		for (size_t col = 0; col < mData[row].size(); ++col)
		{
			inFunctor(col, row, mData[row][col]);
		}
	}
}
