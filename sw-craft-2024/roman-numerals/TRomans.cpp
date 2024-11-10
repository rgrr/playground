/*
 * TRomans.cpp
 *
 *  Created on: Nov 5, 2024
 *      Author: i02441001
 */


#include <map>
#include <variant>
#include <vector>
#include "TRomans.h"


struct SRoman {
	int num;
	std::string roman;
};

const std::vector<SRoman> roman_vector {
	{ 1000, "M" },
	{ 900, "CM" },
	{ 500, "D" },
	{ 400, "CD" },
	{ 100, "C" },
	{ 90, "XC" },
	{ 50, "L" },
	{ 40, "XL" },
	{ 10, "X" },
	{ 9, "IX" },
	{ 5, "V" },
	{ 4, "IV" },
	{ 1, "I" }
};



std::string TRomans::ConvertToRomanNum(int num)
{
	std::string out = "";

	for (const auto & [arab_num, roman_num] : roman_vector)
	{
		while (num >= 0  &&  num >= arab_num)
		{
			out += roman_num;
			num -= arab_num;
		}
	}
	return out;
}
