// ========================================================
//
// FILE:			/source/core/math.hpp
//
// AUTHORS:			Jaycie Ewald
//
// PROJECT:			Duke4Ever
//
// LICENSE:			TBD
//
// DESCRIPTION:		Math namespace header
//
// LAST EDITED:		November 30th, 2022
//
// ========================================================

//
//
// Math namespace
//
//

// Math namespace definition (public)
namespace Math
{

// https://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
template <typename T> inline constexpr
int signum(T x, std::false_type is_signed)
{
	return T(0) < x;
}

template <typename T> inline constexpr
int signum(T x, std::true_type is_signed) 
{
	return (T(0) < x) - (x < T(0));
}

template <typename T> inline constexpr
int signum(T x)
{
	return signum(x, std::is_signed<T>());
}

}
