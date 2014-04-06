#include<tuple>
#include<vector>
#include<map>
#include<memory>

// all_same
template<typename T, typename ...Types> struct all_same;
template<typename T> struct all_same<T>
{
	static const bool value = true;
};
template<typename T, typename ...Types> struct all_same<T, T, Types...> : public all_same<T, Types...> { };
template<typename T, typename S, typename ...Types> struct all_same<T, S, Types...>
{
	static const bool value = false;
};

template<typename ...ColumnType> class table
{
public:
	typedef std::tuple<ColumnType...> value_type;
	typedef value_type &reference;
	typedef const reference const_reference;

	typedef std::vector<value_type> data_type;

private:
	data_type data_;

public:
	reference at(size_t index) { return data_.at(index); }
	const_reference at(size_t index) const { return data_.at(index); }

	table(std::initializer_list<value_type> rows) :
		data_(rows) { }
};


// Column = zero-based column position, Type = column data type
template<size_t KeyIndex, typename Key, typename Row> class index
{
public:
	typedef std::vector<Row> data_type;		// shold be defined in the table
	typedef typename data_type::iterator row_ptr;

	row_ptr get(const Key& value) { return data_.at(value); }

private:
	std::map<Key, row_ptr> data_;

public:
	index(data_type &data)
	{	// build index
		for (row_ptr it = data.begin(); it < data.end(); ++it)
		{
			auto key = std::get<KeyIndex>(*it);
			data_.insert(std::make_pair(key, it));
		}
	}
};
// Type = type of index class, instance of upper template
template<size_t Index, typename Type> class index_holder
{
public:
	typedef typename Type::data_type data_type;
private:
	std::unique_ptr<Type> index_;
	data_type &data_;
public:
	Type *operator->()
	{
		// create index if does not exist yet
		if (index_ == nullptr)
			index_.reset(new Type(data_));

		// This is just symbolic
		return index_.operator->();
	}

	index_holder(data_type &data) : data_(data), index_(nullptr) { }
};
/*const Db::value_type &find(const typename std::tuple_element<Column, typename db::value_type>::type &key)
{

}*/

template<size_t Size, typename ...ColumnType>
struct const_db
{
	typedef std::tuple<ColumnType...> value_type;
	typedef value_type row_type;

	template<size_t Index, typename Column>
	void get(Column key) { }

};

/*struct my_db : const_db<int, std::string> {

		//typedef std::tuple<…> value_type;
	
};

template <class db, int idx>
const typename db::value_type &find(const typename std::tuple_element<idx, typename db::value_type>::type &key);*/
