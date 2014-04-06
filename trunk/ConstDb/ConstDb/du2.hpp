#include<tuple>
#include<vector>
#include<map>
#include<memory>
#include<stdexcept>

class not_found_exception : public std::runtime_error
{
public:
	not_found_exception(const std::string &msg) : std::runtime_error(msg) { }
};

// ColumnIndex = zero-based column position
// RowType = type of the entire row data type (probably std::tuple<...>)
template<size_t ColumnIndex, typename RowType> class index
{
public:
	typedef typename std::tuple_element<ColumnIndex, RowType>::type column_type;
	typedef std::vector<RowType> data_type;		// shold be defined in the table
	typedef typename data_type::iterator row_ptr;

	row_ptr get(const column_type& key)
	{ 
		auto res = data_index_.find(key);
		return res != data_index_.end() ? res->second : data_.end();
	}

private:
	std::map<column_type, row_ptr> data_index_;
	data_type &data_;

public:
	index(data_type &data) : data_(data)
	{	// build index
		for (row_ptr it = data.begin(); it < data.end(); ++it)
		{
			auto key = std::get<ColumnIndex>(*it);
			data_index_.insert(std::make_pair(key, it));
		}
	}
};
// Type = type of index class, instance of upper template
template<typename IndexType> class index_holder
{
public:
	typedef typename IndexType::data_type data_type;
private:
	std::unique_ptr<IndexType> index_;
	data_type &data_;
public:
	IndexType *operator->()
	{
		// create index if does not exist yet
		if (index_ == nullptr)
			index_.reset(new IndexType(data_));

		// Seems to be very symbolic to me
		return index_.operator->();
	}

	index_holder(data_type &data) : data_(data), index_(nullptr) { }
};

template<size_t ColumnIndex, typename RowType> struct index_tuple
	: public index_tuple<ColumnIndex - 1, RowType>
{
	typedef index_tuple<ColumnIndex - 1, RowType> base_type;
	typedef typename base_type::data_type data_type;

	typedef index_holder<index<ColumnIndex, RowType>> index_type;
	index_type index_holder_;

	index_tuple(data_type &data) : index_holder_(data), base_type(data) { }
};
template<typename RowType> struct index_tuple<0, RowType>
	//: public index_holder<index<0, RowType>>
{
	typedef index_holder<index<0, RowType>> base_type;

	typedef index_holder<index<0, RowType>> index_type;
	typedef typename index_type::data_type data_type;
	
	index_type index_holder_;

	index_tuple(data_type &data) : index_holder_(data) { }
};

template<typename ...ColumnType> class table
{
public:
	typedef std::tuple<ColumnType...> value_type;
	typedef std::vector<value_type> data_type;

	template<size_t ColIndex>
	using column_type = typename std::tuple_element<ColIndex, value_type>::type;

private:
	data_type data_;
	typedef typename data_type::iterator row_ptr;

	index_tuple<std::tuple_size<value_type>::value - 1, value_type> index_tuple_;

	template<size_t index>
	row_ptr get(const column_type<index> &key)
	{
		// get index for column at "index" position
		index_tuple<index, value_type> &idx = index_tuple_;
		// find by that index
		row_ptr row = idx.index_holder_->get(key);
		return row;
	}

public:
	
	template<size_t index>
	const value_type &find(const column_type<index> &key)
	{	
		row_ptr row = get<index>(key);
		if (row == data_.end())
			throw not_found_exception("Given key not found!");

		// returns reference to founded row
		return *row;
	}

	template<size_t index>
	bool contains(const column_type<index> &key)
	{
		return get<index>(key) != data_.end();
	}

	explicit table(std::initializer_list<value_type> rows) :
		data_(rows), index_tuple_(data_) { }
	explicit table(const data_type &data) :
		data_(data), index_tuple_(data_) { }
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