#pragma once
#include "core.h"
#include <bitset>

#define REFLECT_ENUM(data) {#data, data}

template <typename TBase>
struct property_t
{
	const char* name;
	TBase data;
};

template <typename TBase>
bit_mask_t CreateBitMask(TBase* properties, size_t size, std::vector<std::string>& strings)
{
	bit_mask_t bit_mask;

	for (std::string& string : strings)
	{
		for (size_t i = 0; i < size; i++)
		{
			std::string s(properties[i].name);
			if (string_compare_no_case(s, string))
			{
				bit_mask.set(properties[i].data);
			}
		}
	}

	return bit_mask;
}