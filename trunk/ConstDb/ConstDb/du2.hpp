#include<tuple>
#include<string>

// Column = zero-based column position, Type = column data type
template<size_t Column, typename Type, typename Db> struct index
{
	// TODO: data representation???
	

	const Db::value_type &find(const typename std::tuple_element<Column, typename db::value_type>::type &key)
	{

	}
};
// TODO: specialize index for integral types


template<typename ...Columns>
struct const_db
{
	typedef std::tuple<Columns...> value_type;


};

struct my_db : const_db<int, std::string> {

		//typedef std::tuple<…> value_type;
	
};

template <class db, int idx>
const typename db::value_type &find(const typename std::tuple_element<idx, typename db::value_type>::type &key);