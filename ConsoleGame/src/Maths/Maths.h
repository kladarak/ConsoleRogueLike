#pragma once

template <typename T>
inline bool gIsBetween(const T& inT, const T& inMin, const T& inMax)
{
	return (inT >= inMin) && (inT < inMax);
};

template <typename T>
inline T gLerp2D(const T& inT, const T& inMin, const T& inMax)
{
	return inT * (inMax - inMin) + inMin;
};

template <typename T>
inline T gFraction(const T& inT, const T& inMin, const T& inMax)
{
	return (inT - inMin) / (inMax - inMin);
}
