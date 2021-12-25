#ifndef COMMON_H
#define COMMON_H

#include <functional>
#include <memory>
#include <memory>
#include <memory>
#include <vector>
#include <memory>

namespace gamelib
{

	template <typename ENUM>
	auto as_integer(ENUM const value)
	{
		return static_cast<typename std::underlying_type<ENUM>::type>(value);
	}
		

	inline int get_random_int(const int min, const int max)
	{
		return rand() % (max - min + 1) + min;
	}
}


#endif