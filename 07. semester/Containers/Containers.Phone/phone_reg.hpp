#pragma once

#include<map>
#include<memory>
#include<vector>
#include<set>
#include<ostream>

#include "item.hpp"

class phone_reg
{
public:
	typedef std::vector<std::shared_ptr<item>> list;
	typedef std::multimap<item::text_type, std::shared_ptr<item>> text_dic;
	typedef std::multimap<item::num_type, std::shared_ptr<item>> num_dic;
private:
	std::string filename;
public:
	text_dic name_index;
	num_dic phone_index;
	list items;

	void print_item(std::shared_ptr<item> item) const;
	void print_item(std::shared_ptr<item> item, std::ostream &stream) const;

	void add_item(std::shared_ptr<item> item);
	void print_by_name();

	void remove_item(item::num_type number);

	void find_by_phone(item::num_type phone);
	void find_by_name(item::text_type &name);

	void save();
	void load();

	phone_reg(std::string filename);
	~phone_reg(void);
};

