#include<sstream>
#include<vector>

#include "item.hpp"


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::ostream& operator<<(std::ostream& stream, const item& item)
{
	item.print(stream);
	return stream;
}
std::istream & operator>>(std::istream &stream, item& item)
{
	item.parse(stream);
	return stream;
}
bool item::operator<(const item &item)
{
	return this->surname < item.surname || this->name < item.name;
}

void item::print(std::ostream& stream) const
{
	stream << this->name << " " << this->surname << " (" << this->name << ") - " << this->direction;
}
void item::parse(std::istream& stream)
{
	std::string line;
	std::getline(stream, line);
	std::stringstream ss(line);

	std::string num;
	std::getline(ss, num, ' ');
	this->number = std::stoull(num);

	std::string name;
	std::getline(ss, name, ' ');
	this->name = name;

	std::string surname;
	std::getline(ss, surname, ' ');
	this->surname = surname;

	std::string dir;
	std::getline(ss, dir, '\n');
	this->direction = dir;
}
std::string item::to_string() const
{
	std::ostringstream str;
	print(str);
	return str.str();
}