#pragma once

#include<algorithm>

class MathUtils
{
public:

	// Got this function from stackoverflow (first answer):
	// https://stackoverflow.com/questions/9323903/most-efficient-elegant-way-to-clip-a-number
	template <typename T>
	static T clip(const T& n, const T& lower, const T& upper) {
		return std::max(lower, std::min(n, upper));
	}
};