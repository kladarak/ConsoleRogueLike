#pragma once

#include <vector>

#define gElemCount( arr ) (sizeof(arr)/sizeof(arr[0]))

template<typename T>
inline std::vector<T> gCArrayToVector(const T* inArray, size_t inElemCount)
{
	std::vector<T> out;
	out.reserve(inElemCount);

	for (size_t i = 0; i < inElemCount; ++i)
	{
		out.push_back( inArray[i] );
	}

	return out;
}

template<typename TValue, typename TInBothFunc, typename TInLeftOnlyFunc, typename TInRightOnlyFunc>
inline void WalkSortedVectors(	const std::vector<TValue>&	inLHS,
								const std::vector<TValue>&	inRHS,
								const TInBothFunc&			inBothFunc,
								const TInLeftOnlyFunc&		inInLeftOnlyFunc,
								const TInRightOnlyFunc&		inInRightOnlyFunc)
{
	// Entity IDs should be in order, so walk over both lists, incrementing as we go.
	auto lhsBegin	= inLHS.begin();
	auto lhsEnd		= inLHS.end();
	auto rhsBegin	= inRHS.begin();
	auto rhsEnd		= inRHS.end();

	while (lhsBegin != lhsEnd || rhsBegin != rhsEnd)
	{
		if (lhsBegin != lhsEnd && rhsBegin != rhsEnd)
		{
			auto& left = *lhsBegin;
			auto& right = *rhsBegin;
			if (left == right)
			{
				inBothFunc(left);
				++lhsBegin;
				++rhsBegin;
			}
			else if (left < right)
			{
				inInLeftOnlyFunc(left);
				++lhsBegin;
			}
			else // if (left > right)
			{
				inInRightOnlyFunc(right);
				++rhsBegin;
			}
		}
		else if (lhsBegin != lhsEnd)
		{
			inInLeftOnlyFunc(*lhsBegin);
			++lhsBegin;
		}
		else //if (rhsBegin != rhsEnd)
		{
			inInRightOnlyFunc(*rhsBegin);
			++rhsBegin;
		}
	}
}

template<typename TValue>
inline std::vector<TValue> MergeSortedVectors(const std::vector<TValue>& inLHS, const std::vector<TValue>& inRHS)
{
	std::vector<TValue> out;
	auto addToOut = [&] (const TValue& inValue) { out.push_back(inValue); };
	WalkSortedVectors(inLHS, inRHS, addToOut, addToOut, addToOut);
	return out;
}
