// zadanie:
/*
 * nacitat telefonny zoznam zo suboru
 * nacitat do vlastnych struktur
 * za chodu - pridat a ubrat zo zoznamu
 * vypisat zotridene podla mena
 * rychlo vyhladavat cislo podla mena a obratene
 * format: cislo prijmeni meno adresa aj s medzerami
 */

#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<memory>

#include<map>

#include "item.hpp"
#include "phone_reg.hpp"

using namespace std;

void read_file(ifstream &input, phone_reg &reg)
{
	while (!input.eof())
	{
		std::shared_ptr<item> item(new item());
		input >> (*item);

		reg.add_item(item);
	}
}

void print_opt()
{
	cout << "q - quit" << endl;
	cout << "h - help" << endl;
	cout << "n - add new item" << endl;
	cout << "r - remove an item" << endl;
	cout << "f - lookup" << endl;
	cout << "l - list register items sorted by name" << endl;
	cout << "s - save" << endl;
}
void add_new_item(phone_reg &reg)
{
	cout << "Enter a new item (phone name surname address): ";
	std::shared_ptr<item> item(new item());
	cin >> (*item);
	reg.add_item(item);
}
void remove_item(phone_reg &reg)
{
	cout << "Enter number to remove: ";
	item::num_type num;
	cin >> num;
	reg.remove_item(num);
}
void find_item_phone(phone_reg &reg)
{
	cout << "Enter phone: ";
	item::num_type num;
	cin >> num;
	reg.find_by_phone(num);
}
void find_item_name(phone_reg &reg)
{
	cout << "Enter full name: ";
	item::text_type name;
	std::getline(cin, name);
	reg.find_by_name(name);
}
void find_item(phone_reg &reg)
{
	cout << "n - lookup by name" << endl;
	cout << "p - lookup by phone" << endl;

	char ch = std::getchar();
	switch (ch)
	{
	case 'n':
		std::getchar();
		find_item_name(reg);
		break;
	case 'p':
		std::getchar();
		find_item_phone(reg);
		break;
	}
}
void run_cmd(phone_reg &reg, char ch)
{
	switch (ch)
	{
	case 'h':
		print_opt();
		break;
	case 'l':
		reg.print_by_name();
		break;
	case 'n':
		std::getchar();		// skip enter
		add_new_item(reg);
		break;
	case 'r':
		remove_item(reg);
		break;
	case 'f':
		find_item(reg);
		break;
	case 's':
		reg.save();
		break;
	default:
		cout << "No such option" << endl;
		break;
	}
}
void run_app(string &filename, phone_reg &reg)
{
	char ch = ' ';

	while (ch != 'q')
	{
		ch = std::getchar();

		run_cmd(reg, ch);
	}
}

int main(int argc, char **argv)
{
	vector<string> params(argv, argv + argc);
	if (params.size() < 2)
	{
		cout << "Missing argument: input file" << endl;
		return 1;
	}

	string filename = params.at(1);


	phone_reg reg(filename);
	reg.load();

	print_opt();
	run_app(filename, reg);

	//system("pause");
	return 0;
}

