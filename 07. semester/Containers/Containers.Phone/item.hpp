#pragma once

#include<string>
#include<iostream>

struct item
{
	typedef unsigned long long num_type;
	typedef std::string text_type;

	text_type surname;
	text_type name;
	num_type number;

	text_type direction;

	friend std::ostream & operator<<(std::ostream &stream, const item& item);
	friend std::istream & operator>>(std::istream &istream, item& item);

	bool operator<(const item &item);

	void parse(std::istream& str);
	void print(std::ostream& stream) const;
	std::string to_string() const;
};