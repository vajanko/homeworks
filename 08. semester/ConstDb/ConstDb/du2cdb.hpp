// DU2cdb.hpp
// Ondrej Kov�� NPRG051 2013/2014

#include<tuple>
#include<vector>
#include<map>
#include<memory>
#include<stdexcept>

// Exception which is thrown when lookup in constant database is not successfull
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

	// Gets iterator to row which contains "key" in column at "ColumnIndex" position
	// or end() if such a row does not exist
	row_ptr get(const column_type& key)
	{ 
		auto res = data_index_.find(key);
		return res != data_index_.end() ? res->second : data_.end();
	}

private:
	// Index to data for column at "ColumnIndex" position
	std::map<column_type, row_ptr> data_index_;
	// Reference to raw data of the database
	data_type &data_;

public:
	void insert(row_ptr it)
	{	// new value inserted - update the index
		auto key = std::get<ColumnIndex>(*it);
		// Check whether current key is alredy present
		if (data_index_.find(key) != data_index_.end())
		{	// if key is present more than once row won't be accesible through this key
			data_index_[key] = data_.end();
		}
		else
		{
			data_index_.insert(std::make_pair(key, it));
		}
	}

	// Builds index on given database data (no const index is necessary)
	index(data_type &data) : data_(data)
	{	// build index
		for (row_ptr it = data.begin(); it < data.end(); ++it)
		{
			insert(it);
		}
	}
};
// "index_holder" wrapps "index" class and creates it instance if necessary (lazy)
// IndexType = type of index class, instance of upper template
template<typename IndexType> class index_holder
{
public:
	typedef typename IndexType::data_type data_type;
private:
	// Pointer to index if called from user code or nullptr otherwise
	std::unique_ptr<IndexType> index_;
	// Reference to raw data of the database - necessary when building index
	data_type &data_;
public:
	// pretend to be index ...
	IndexType *operator->()
	{
		// create index if does not exist yet
		if (index_ == nullptr)
			index_.reset(new IndexType(data_));

		// Seems to be very symbolic to me
		return index_.operator->();
	}
	void insert(typename IndexType::row_ptr row)
	{	// new value inserted - update the index (do not if index is not built yet)
		if (index_ != nullptr)
			index_->insert(row);
	}

	// Notice that index_ is uninitialized
	index_holder(data_type &data) : data_(data), index_(nullptr) { }
};

// Just compile time collection of database indexes. The reason why we did not
// use std::tuple is that each tuple argument type need its ColumnIndex 
template<size_t ColumnIndex, typename RowType> struct index_tuple
	: public index_tuple<ColumnIndex - 1, RowType>
{
	typedef index_tuple<ColumnIndex - 1, RowType> base_type;
	typedef typename base_type::data_type data_type;		// takes from inherited class

	typedef index_holder<index<ColumnIndex, RowType>> index_type;
	index_type index_holder_;

	void insert(typename index<ColumnIndex, RowType>::row_ptr row)
	{
		index_holder_.insert(row);
		base_type::index_holder_.insert(row);
	}

	index_tuple(data_type &data) : index_holder_(data), base_type(data) { }
};
template<typename RowType> struct index_tuple<0, RowType>
{
	typedef index_holder<index<0, RowType>> index_type;
	typedef typename index_type::data_type data_type;		// definition of database raw data type
	
	index_type index_holder_;

	void insert(typename index<0, RowType>::row_ptr row)
	{
		index_holder_.insert(row);
	}

	index_tuple(data_type &data) : index_holder_(data) { }
};

// Table encapsulate raw data and indexes for each column
// ValueType = std::tuple<...>
template<typename ValueType> class table
{
public:
	typedef ValueType value_type;
	typedef std::vector<value_type> data_type;

	// type of column data at ColIndex
	template<size_t ColIndex>
	using column_type = typename std::tuple_element<ColIndex, value_type>::type;

private:
	data_type data_;
	typedef typename data_type::iterator row_ptr;

	// creates structure for as much indexes as number of columns
	// -1 ??? => because column indexes are zero-based
	index_tuple<std::tuple_size<value_type>::value - 1, value_type> index_tuple_;

	// Lookup by column at "index" position - for internal purpose only
	// Returns iterator to founded row or end() if not found
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
	
	// Lookup by column at "index" position
	// Returns reference to founded row or an exception is thrown when no such a row exist
	// or multiple
	template<size_t index>
	const value_type &find(const column_type<index> &key)
	{	
		row_ptr row = get<index>(key);
		if (row == data_.end())
			throw not_found_exception("Given key not found or multiple instances are present in current column.");

		// returns reference to founded row
		return *row;
	}

	// Returns value indicating whether search by column at "index" will be successful
	template<size_t index>
	bool contains(const column_type<index> &key)
	{
		return get<index>(key) != data_.end();
	}

	// Inserts a new row to the table
	void insert(const value_type &row)
	{
		data_.push_back(row);
		row_ptr it = --data_.end();
		index_tuple_.insert(it);
	}

	table() : data_(), index_tuple_(data_) { }
	explicit table(std::initializer_list<value_type> rows) : data_(rows), index_tuple_(data_) { }
	explicit table(const data_type &data) : data_(data), index_tuple_(data_) { }
};

// Singleton class for user defined constant database
template<typename DbType, typename ...ColumnType>
class const_db
{
public:
	typedef std::tuple<ColumnType...> value_type;
	typedef table<value_type> table_type;
private:
	table_type table_;

public:
	// Returns reference to singleton table
	static table_type &instance()
	{	// here user-defined constructor of DbType is called which allows to insert rows
		static DbType instance_;

		return instance_.table_;
	}
	// Inserts new row to constat database. Call this method from your constructor
	void insert(ColumnType ...value)
	{
		table_.insert(std::make_tuple(value...));
	}
};

// Search constant database of type ConstDb in columnt at "ColumnIndex" position and
// return a reference to founded row or an exception is thrown when no such a row exist
// or multiple with same value.
template<typename ConstDb, size_t ColumnIndex>
const typename ConstDb::value_type &
	find(const typename std::tuple_element<ColumnIndex, typename ConstDb::value_type>::type &key)
{
	return ConstDb::instance().find<ColumnIndex>(key);
}