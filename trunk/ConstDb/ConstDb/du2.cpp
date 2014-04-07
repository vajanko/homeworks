#include<iostream>
#include<string>
#include "du2.hpp"

using namespace std;

// Example 1) (from slides)
enum color { red, green, blue };
struct db_colors : public const_db<db_colors, color, std::string>
{
	db_colors()
	{
		insert(color::red, "red");
		insert(color::green, "green");
		insert(color::blue, "blue");
	}
};
std::string to_string(color c) 
{ 
	return std::get<1>(find<db_colors, 0>(c));
}
color from_string(const std::string &str)
{ 
	return std::get<0>(find<db_colors, 1>(str));
}
void example1()
{
	cout << "EXAMPLE 1" << endl;
	cout << color::red << " == " << to_string(color::red) << endl;
	cout << from_string("blue") << " == " << "blue" << endl;
	cout << endl;
}
// End of example 1)

// Example 2) (tree structure)
struct node
{
	int id;
	const node &parent;

	bool operator<(const node &n) const { return id < n.id; }

	node(int id) : id(id), parent(*this) { }
	node(int id, const node &parent) : id(id), parent(parent) { }
};
struct db_tree : public const_db<db_tree, int, node>
{
	static const node &find_node(int key)
	{
		return std::get<1>(find<db_tree, 0>(key));
	}
	
	db_tree()
	{
		insert(0, node(0));
		insert(1, node(1, find_node(0)));
		insert(2, node(2, find_node(0)));

		insert(3, node(3, find_node(1)));
	}
};
void example2()
{
	cout << "EXAMPLE 2" << endl;

	int key = 3;

	// traversing path from leaf to the root
	while (key != 0)
	{
		const node &n = db_tree::find_node(key);
		cout << key << endl;
		key = n.parent.id;
	}

	cout << endl;
}
// End of example 2)

// Example 3) (personal)
// Id, Name, Email, Age
struct db_personal : public const_db<db_personal, int, string, string, int>
{
	db_personal()
	{
		insert(1, "Bob", "bob@gmail.com", 21);
		insert(2, "Alice", "alice@email.cz", 25);
		insert(3, "John", "john@yahoo.com", 21);
		insert(4, "Mary", "mary@me.com", 18);
	}
};
void example3()
{
	cout << "EXAMPLE 3" << endl;

	vector<string> names{ "John", "Ashely", "Mary" };
	for (auto &name : names)
	{
		try {
			auto &row = find<db_personal, 1>(name);
			cout << name << ": Id: " << get<0>(row) << " Email: " << get<2>(row) << endl;
		}
		catch (not_found_exception) {
			cout << "There is no " << name << " in the database." << endl;
		}
	}
	cout << endl;

	vector<int> ages{ 25, 21, 19 };
	for (auto age : ages)
	{
		cout << "Who is " << age << " years old?" << endl;

		try {
			auto &row = find<db_personal, 3>(age);
			cout << "It is " << get<1>(row) << endl;
		}
		catch (not_found_exception) {
			cout << "There is no one " << age << " years old or more of same age." << endl;
		}
	}

	cout << endl;
}
// End of example 3)

// Example 4) (permutation)
struct permutation : public const_db<permutation, int, int>
{
	permutation()
	{
		// two cycles (0 - 3 - 2) and (1 - 5 - 4)
		insert(0, 3);
		insert(1, 5);
		insert(2, 0);
		insert(3, 2);
		insert(4, 1);
		insert(5, 4);
	}
};
void fwd_permut(const std::vector<int> &src, std::vector<int> &trg)
{
	for (int i = 0; i < 6; ++i)
	{
		int idx = get<1>(find<permutation, 0>(i));
		trg[idx] = src[i];
	}
}
void print(const std::vector<int> &vec)
{
	for (int i : vec)
	{
		cout << i << " ";
	}
	cout << endl;
}
void example4()
{
	cout << "EXAMPLE 4" << endl;

	std::vector<int> src{ 0, 1, 2, 3, 4, 5 };
	std::vector<int> trg(6, 0);

	cout << "Before permutation: ";
	print(src);
	for (int i = 0; i < 3; ++i)
	{
		fwd_permut(src, trg);
		print(trg);
		src = trg;
	}

	cout << endl;
}
// End of example 4)

int main(int argc, char **argv)
{
	example1();
	example2();
	example3();
	example4();

	system("pause");
}

