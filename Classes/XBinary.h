fndef XBinary_H
#define XBinary_H
#include "XBase.h"
#include <cstdint>
#include <string>
XLIB_BEGAIN

using b_value = int64;
using b_type = std::string;

b_type getType(b_value intValue)
{
	std::string s = " ";
	for (int64 a = intValue; a; a = a / 2)
	{
		s = s + (a % 2 ? '1' : '0');
	}
	std::reverse(s.begin(), s.end());
	return s;
}

template<char..._0_1_series>
class binary_impl;

template<>
class binary_impl<'0'>
{
public:
	b_value value;
	b_type  type;
	binary_impl() 
	{
		value = 0;
		type = "0";
	}

};

template<>
class binary_impl<'1'>
{
public:
	b_value value;
	b_type  type;
	binary_impl()
	{
		value = 1;
		type = "1";
	}
};



template<char _0_or_1, char...rest_0_1_series>
class binary_impl<_0_or_1, rest_0_1_series...>
{
public:
	b_value value;
	b_type  type;

	binary_impl()
	{
		value = (binary_impl<_0_or_1>().value << sizeof...(rest_0_1_series)) +
			binary_impl<rest_0_1_series...>().value;
		type = binary_impl<_0_or_1>().type.append(binary_impl<rest_0_1_series...>().type);
	}
};

template<>
class binary_impl<>
{
public:
	b_value value;
	b_type  type;
	binary_impl(b_value v)
	{
		value = v;
		type = getType(v);
	}
};

template <char ..._0_1_series>
constexpr binary_impl<_0_1_series...> operator ""_b()
{
	return binary_impl<_0_1_series...>();
}



XLIB_END

#endif //!XBinary_H


