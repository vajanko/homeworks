#include "phone_reg.hpp"
#include<algorithm>
#include<iostream>
#include<fstream>

void phone_reg::add_item(std::shared_ptr<item> item)
{
	this->items.push_back(item);

	this->phone_index.insert(std::make_pair(item->number, item));
	this->name_index.insert(std::make_pair(item->surname + " " + item->name, item));
}
void phone_reg::print_by_name()
{
	text_dic::const_iterator cend = name_index.cend();
	for (text_dic::const_iterator it = name_index.cbegin(); it != cend; ++it)
	{
		print_item(it->second);
	}
}
void phone_reg::print_item(std::shared_ptr<item> item) const
{
	print_item(item, std::cout);
}
void phone_reg::print_item(std::shared_ptr<item> item, std::ostream &stream) const
{
	stream << item->name << " " << item->surname << " " << item->number << " " << item->direction << std::endl;
}

void phone_reg::remove_item(item::num_type number)
{
	for (list::iterator it = items.begin(); it < items.end(); ++it)
	{
		if ((*it)->number == number)
		{
			items.erase(it);
			break;
		}
	}

	for (text_dic::iterator it = name_index.begin(); it != name_index.end(); ++it)
	{
		if (it->second->number == number)
		{
			name_index.erase(it);
			break;
		}
	}

	phone_index.erase(number);
}

void phone_reg::find_by_phone(item::num_type phone)
{
	auto end = phone_index.upper_bound(phone);
	for (auto it = phone_index.lower_bound(phone); it != end; ++it)
	{
		print_item(it->second);
	}
}
void phone_reg::find_by_name(item::text_type &name)
{
	auto end = name_index.upper_bound(name);
	for (auto it = name_index.lower_bound(name); it != end; ++it)
	{
		print_item(it->second);
	}
}

void phone_reg::save()
{
	try {
		std::ofstream output;
		// an exception will be thrown when one of theese flags is set
		output.exceptions(std::ios::badbit | std::ios::failbit);
		output.open(filename, std::ios::out | std::ios::beg);

		text_dic::const_iterator cend = name_index.cend();
		for (text_dic::const_iterator it = name_index.cbegin(); it != cend; ++it)
		{
			print_item(it->second, output);
		}

		output.close();
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
}
void phone_reg::load()
{
	try {
		std::ifstream input;
		// an exception will be thrown when one of theese flags is set
		input.exceptions(std::ios::badbit | std::ios::failbit);
		input.open(filename, std::ios::in | std::ios::beg);

		while (!input.eof())
		{
			std::shared_ptr<item> item(new item());
			input >> (*item);

			add_item(item);
		}

		input.close();
	}
	catch (std::exception e) {
		std::cout << e.what() << std::endl;
	}
}

phone_reg::phone_reg(std::string filename) : filename(filename)
{
}
phone_reg::~phone_reg(void)
{
}
