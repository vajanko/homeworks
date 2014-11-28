/*

DUTABLES.CPP

DB

Mlaskal's semantic tables 

*/

#include "dutables.h"
#include "literal_storage.hpp"
#include "duerr.h"
#include <map>
#include <vector>
#include <ostream>
#include <iostream>
#include <string>
#include <sstream>


/*************************************************************************/

namespace mlc {

	/*************************************************************************/

	type_category type_entry::cat_() const
	{
		return typecat;
	}

	array_type_pointer type_entry::arrayType_()
	{
		return typecat == TCAT_ARRAY ? array_type_pointer( this) : array_type_pointer();
	}

	record_type_pointer type_entry::recordType_()
	{
		return typecat == TCAT_RECORD ? record_type_pointer( this) : record_type_pointer();
	}

	int_type_pointer type_entry::integerType_()
	{
		return typecat == TCAT_INT ? int_type_pointer( this) : int_type_pointer();
	}

	range_type_pointer type_entry::rangeType_()
	{
		return typecat == TCAT_RANGE ? range_type_pointer( this) : range_type_pointer();
	}

	bool_type_pointer type_entry::booleanType_()
	{
		return typecat == TCAT_BOOL ? bool_type_pointer( this) : bool_type_pointer();
	}

	real_type_pointer type_entry::realType_()
	{
		return typecat == TCAT_REAL ? real_type_pointer( this) : real_type_pointer();
	}

	str_type_pointer type_entry::stringType_()
	{
		return typecat == TCAT_STR ? str_type_pointer( this) : str_type_pointer();
	}

	type_pointer type_entry::elementType_() const
	{
		return typecat == TCAT_ARRAY ? ltype2 : type_pointer();
	}

	type_pointer type_entry::indexType_() const
	{
		return typecat == TCAT_ARRAY ? ltype1 : type_pointer();
	}

	ls_int_index type_entry::lowerBound_() const
	{
		return typecat == TCAT_RANGE ? idx1 : ls_int_index();
	}

	ls_int_index type_entry::upperBound_() const
	{
		return typecat == TCAT_RANGE ? idx2 : ls_int_index();
	}

	size_t type_entry::get_idx_() const
	{
		return idx_;
	}

	field_pointer type_entry::record_find_( ls_id_index idx) const
	{
		record_cache_type::const_iterator fi = record_cache.find( idx);
		return field_pointer( fi == record_cache.end() ? 0 : fi->second);
	}

	field_vector::const_iterator type_entry::record_begin_() const
	{
		return fldlist->begin();
	}

	field_vector::const_iterator type_entry::record_end_() const
	{
		return fldlist->end();
	}

	const type_entry * type_entry::access_entry() const
	{
		return this;
	}

	/*************************************************************************/

	void symbol_entry::kill()
	{
		if ( parlist )
		{
			delete parlist;
			parlist = 0;
		}

		if ( local_tables_ )
		{
			delete local_tables_;
			local_tables_ = 0;
		}
	}

	struct check_label {
		check_label() {}
		void operator()( label_map::value_type & e) const
		{
			e.second.check();
		}
	};

	void kill_symbol( symbol_map::value_type & e)
	{
		e.second.kill();
	}

	symbol_tables::~symbol_tables()
	{
		std::for_each( glabels.begin(), glabels.end(), check_label());
		std::for_each( globals.begin(), globals.end(), kill_symbol);
	}

	symbol_kind symbol_entry::kind_() const
	{
		return symtype;
	}

	typed_symbol_pointer symbol_entry::typedSymbol_()
	{
		return symtype == SKIND_FUNCTION 
			|| symtype == SKIND_GLOBAL_VARIABLE 
			|| symtype == SKIND_LOCAL_VARIABLE 
			|| symtype == SKIND_PARAMETER_BY_REFERENCE 
			|| symtype == SKIND_TYPE 
			|| symtype == SKIND_CONST 
			? typed_symbol_pointer( this)
			: typed_symbol_pointer();
	}

	variable_symbol_pointer symbol_entry::variableSymbol_()
	{
		return symtype == SKIND_GLOBAL_VARIABLE 
			|| symtype == SKIND_LOCAL_VARIABLE 
			|| symtype == SKIND_PARAMETER_BY_REFERENCE 
			? variable_symbol_pointer( this)
			: variable_symbol_pointer();
	}

	subprogram_symbol_pointer symbol_entry::subprogramSymbol_()
	{
		return symtype == SKIND_PROCEDURE 
			|| symtype == SKIND_FUNCTION
			? subprogram_symbol_pointer( this)
			: subprogram_symbol_pointer();
	}

	procedure_symbol_pointer symbol_entry::procSymbol_()
	{
		return symtype == SKIND_PROCEDURE 
			? procedure_symbol_pointer( this)
			: procedure_symbol_pointer();
	}

	function_symbol_pointer symbol_entry::fncSymbol_()
	{
		return symtype == SKIND_FUNCTION 
			? function_symbol_pointer( this)
			: function_symbol_pointer();
	}

	global_variable_symbol_pointer symbol_entry::gVarSymbol_()
	{
		return symtype == SKIND_GLOBAL_VARIABLE 
			? global_variable_symbol_pointer( this)
			: global_variable_symbol_pointer();
	}

	local_variable_symbol_pointer symbol_entry::lVarSymbol_()
	{
		return symtype == SKIND_LOCAL_VARIABLE 
			? local_variable_symbol_pointer( this)
			: local_variable_symbol_pointer();
	}

	parameter_by_reference_symbol_pointer symbol_entry::varParSymbol_()
	{
		return symtype == SKIND_PARAMETER_BY_REFERENCE 
			? parameter_by_reference_symbol_pointer( this)
			: parameter_by_reference_symbol_pointer();
	}

	type_symbol_reference symbol_entry::typeSymbol_()
	{
		return symtype == SKIND_TYPE 
			? type_symbol_reference( this)
			: type_symbol_reference();
	}

	const_symbol_reference symbol_entry::constSymbol_()
	{
		return symtype == SKIND_CONST 
			? const_symbol_reference( this)
			: const_symbol_reference();
	}

	const parameter_list * symbol_entry::parList_() const
	{
		return symtype == SKIND_PROCEDURE 
			|| symtype == SKIND_FUNCTION 
			? parlist 
			: 0;
	}

	ic_function_pointer symbol_entry::label_() const
	{
		return symtype == SKIND_PROCEDURE 
			|| symtype == SKIND_FUNCTION 
			? magic
			: ic_function_pointer();
	}

	type_pointer symbol_entry::type_() const
	{
		return symtype == SKIND_FUNCTION 
			|| symtype == SKIND_GLOBAL_VARIABLE 
			|| symtype == SKIND_LOCAL_VARIABLE 
			|| symtype == SKIND_PARAMETER_BY_REFERENCE 
			|| symtype == SKIND_TYPE 
			|| symtype == SKIND_CONST 
			? ltype 
			: type_pointer();
	}

	stack_address symbol_entry::varOffset_() const
	{
		return symtype == SKIND_GLOBAL_VARIABLE 
			|| symtype == SKIND_LOCAL_VARIABLE 
			|| symtype == SKIND_PARAMETER_BY_REFERENCE 
			? relnum_ 
			: 0;
	}

	bool_const_symbol_pointer symbol_entry::boolConstSymbol_()
	{
		return symtype == SKIND_CONST
			&& ltype->cat() == TCAT_BOOL
			? bool_const_symbol_pointer( this)
			: bool_const_symbol_pointer();
	}

	int_const_symbol_pointer symbol_entry::integerConstSymbol_()
	{
		return symtype == SKIND_CONST
			&& ltype->cat() == TCAT_INT
			? int_const_symbol_pointer( this)
			: int_const_symbol_pointer();
	}

	real_const_symbol_pointer symbol_entry::realConstSymbol_()
	{
		return symtype == SKIND_CONST
			&& ltype->cat() == TCAT_REAL
			? real_const_symbol_pointer( this)
			: real_const_symbol_pointer();
	}

	str_const_symbol_pointer symbol_entry::stringConstSymbol_()
	{
		return symtype == SKIND_CONST
			&& ltype->cat() == TCAT_STR
			? str_const_symbol_pointer( this)
			: str_const_symbol_pointer();
	}

	bool symbol_entry::boolValue_() const
	{
		return symtype == SKIND_CONST
			&& ltype->cat() == TCAT_BOOL
			? value 
			: false;
	}

	ls_int_index symbol_entry::integerValue_() const
	{
		return symtype == SKIND_CONST
			&& ltype->cat() == TCAT_INT
			? intidx 
			: ls_int_index();
	}

	ls_real_index symbol_entry::realValue_() const
	{
		return symtype == SKIND_CONST
			&& ltype->cat() == TCAT_REAL
			? realidx 
			: ls_real_index();
	}

	ls_str_index symbol_entry::stringValue_() const
	{
		return symtype == SKIND_CONST
			&& ltype->cat() == TCAT_STR
			? stridx 
			: ls_str_index();
	}


	/*************************************************************************/

	std::string idftext( ls_id_index idx)
	{
		return * idx;
	}

	int intval( ls_int_index idx)
	{
		return * idx;
	}

	std::ostream & operator<<( std::ostream & o, type_pointer e)
	{
		return o << "T" << e->get_idx();
	}
};

/*************************************************************************/

namespace mlc {

	std::ostream & operator<<( std::ostream & o, symbol_kind e)
	{
		switch ( e ) 
		{
		case SKIND_UNDEF: o << "SKIND_UNDEF"; break;
		case SKIND_PROCEDURE: o << "SKIND_PROCEDURE"; break;
		case SKIND_FUNCTION: o << "SKIND_FUNCTION"; break;
		case SKIND_GLOBAL_VARIABLE: o << "SKIND_GLOBAL_VARIABLE"; break;
		case SKIND_LOCAL_VARIABLE: o << "SKIND_LOCAL_VARIABLE"; break;
		case SKIND_PARAMETER_BY_REFERENCE: o << "SKIND_PARAMETER_BY_REFERENCE"; break;
		case SKIND_TYPE: o << "SKIND_TYPE"; break;
		case SKIND_CONST: o << "SKIND_CONST"; break;
		default: o << "SKIND_?(" << (int)e << ")"; break;
		}
		return o;
	}

	/*************************************************************************/

	std::ostream & operator<<( std::ostream & o, parameter_mode e)
	{
		switch ( e ) 
		{
		case PMODE_UNDEF: o << "PMODE_UNDEF"; break;
		case PMODE_BY_VALUE: o << "PMODE_BY_VALUE"; break;
		case PMODE_BY_REFERENCE: o << "PMODE_BY_REFERENCE"; break;
		default: o << "PARTYPE_?(" << (int)e << ")"; break;
		}
		return o;
	}

	/*************************************************************************/

	type_entry::type_entry( size_t idx, type_category p)
		: idx_( idx), typecat( p)
	{
	}

	type_entry::type_entry( size_t idx, type_category p, field_list * f)
		: idx_( idx), typecat( p), fldlist( f)
	{
	}

	type_entry::type_entry( size_t idx, type_category p, type_pointer t1, type_pointer t2)
		: idx_( idx), typecat( p), ltype1( t1), ltype2( t2)
	{
	}

	type_entry::type_entry( size_t idx, type_category p, type_pointer t1, type_pointer t2, ls_int_index s1, ls_int_index s2)
		: idx_( idx), typecat( p), ltype1( t1), ltype2( t2), idx1( s1), idx2( s2)
	{
	}

	std::ostream & operator<<( std::ostream & o, const type_entry & e)
	{
		switch ( e.typecat ) 
		{
		case TCAT_RANGE:
			o << e.typecat << "(" << intval( e.idx1) << "," << intval( e.idx2) << ")"; break;
		case TCAT_INT: 
		case TCAT_BOOL:
		case TCAT_REAL: 
		case TCAT_STR: 
			o << e.typecat; break;
		case TCAT_ARRAY: 
			o << e.typecat << "(" << e.ltype1 << "," << e.ltype2 << ")"; break;
		case TCAT_RECORD: 
			o << e.typecat << "(" << * e.fldlist << ")"; break;
		case TCAT_UNDEF: 
		default: 
			o << e.typecat << "(I" << intval( e.idx1) << ",I" << intval( e.idx2) << "," << e.ltype1 << "," << e.ltype2 << ")"; break;
		}
		return o;
	}

	/*************************************************************************/

	class DumpUniquizer {
	private:
		typedef std::map< size_t, std::string> UniqMap;
	public:
		std::string uniquize_type( std::ostream & o, type_pointer ltype)
		{
			if ( ! ltype )
			{
				return "T?";
			}
			UniqMap::iterator rv = uniqmap_.find( ltype->get_idx());
			if ( rv != uniqmap_.end() )
			{
				// already exists
				return rv->second;
			}
			std::string name;
			o << (* ltype->access_entry())( name, o, * this); // print my line (and dependables before)
			return name;
		}
	private:
		std::string next_name( size_t idx)
		{
			std::ostringstream os;
			os << "T" << uniqmap_.size();
			std::pair< UniqMap::iterator, bool> rv = uniqmap_.insert( UniqMap::value_type( idx, os.str()));
			assert( rv.second);
			return os.str();
		}
		UniqMap uniqmap_;
		friend class type_entry;
	};

	std::string field_entry::operator()( std::ostream & oo, DumpUniquizer & uniq) const
	{
		std::ostringstream o;

		o << "\t\t\t\t<FIELD name=\"" << idftext( idx) << "\" type=\"" << uniq.uniquize_type( oo, ltype) << "\"/>" << std::endl; 

		return o.str();
	}

	std::string type_entry::operator()( std::string & name, std::ostream & oo, DumpUniquizer & uniq) const
	{
		std::ostringstream o;

		switch ( typecat ) 
		{
		case TCAT_RANGE:
			name = uniq.next_name( get_idx());
			o << "\t\t<" << typecat << " type=\"" << name << "\"";
			o << " lower_bound=\"" << intval( idx1) << "\" upper_bound=\"" << intval( idx2) << "\"/>" << std::endl; 
			break;
		case TCAT_INT: 
		case TCAT_BOOL:
		case TCAT_REAL: 
		case TCAT_STR: 
			name = uniq.next_name( get_idx());
			o << "\t\t<" << typecat << " type=\"" << name << "\"";
			o << "/>" << std::endl; 
			break;
		case TCAT_ARRAY: 
			{
				std::string t1 = uniq.uniquize_type( oo, ltype1);
				std::string t2 = uniq.uniquize_type( oo, ltype2);
				name = uniq.next_name( get_idx());
				o << "\t\t<" << typecat << " type=\"" << name << "\"";
				o << " index_type=\"" << t1;
				o << "\" element_type=\"" << t2 << "\"/>" << std::endl; 
				break;
			}
		case TCAT_RECORD: 
			{
				name = uniq.next_name( get_idx());
				o << "\t\t<" << typecat << " type=\"" << name << "\">" << std::endl; 
				field_vector::const_iterator it = fldlist->begin();
				for ( ; it != fldlist->end(); ++it)
				{
					o << (* it)( oo, uniq);
				}
				o << "\t\t</" << typecat << ">" << std::endl; 
				break;
			}
		case TCAT_UNDEF: 
		default: 
			name = uniq.next_name( get_idx());
			o << "\t\t<" << typecat << " type=\"" << name << "\"";
			o << "/>" << std::endl; 
			break;
		}
		return o.str();
	}


	/*************************************************************************/

	class parameter_list;

	std::ostream & operator<<( std::ostream & o, const parameter_list & e);

	std::ostream & operator<<( std::ostream & o, const symbol_entry & e)
	{
		switch ( e.symtype ) 
		{
		case SKIND_PROCEDURE: 
			o << e.symtype << "(" << * e.parlist << ")"; 
			break;
		case SKIND_FUNCTION: 
			o << e.symtype << "(" << e.ltype << "," << * e.parlist << ")"; 
			break;
		case SKIND_GLOBAL_VARIABLE: 
			o << e.symtype << "(G" << e.relnum_ << "," << e.ltype << ")"; 
			break;
		case SKIND_LOCAL_VARIABLE: 
		case SKIND_PARAMETER_BY_REFERENCE:
			o << e.symtype << "(L" << e.relnum_ << "," << e.ltype << ")"; 
			break;
		case SKIND_TYPE: 
			o << e.symtype << "(" << e.ltype << ")"; 
			break;
		case SKIND_CONST: 
			o << e.symtype << "(";
			switch ( e.ltype->cat() )
			{
			case TCAT_BOOL: o << e.value; break;
			case TCAT_INT: o << * e.intidx; break;
			case TCAT_REAL: o << * e.realidx; break;
			case TCAT_STR: o << * e.stridx; break;
			default: o << "?"; break;
			}
			o << "," << e.ltype << ")"; 
			break;
		case SKIND_UNDEF: 
		default: 
			o << e.symtype; 
			break;
		}

		return o;
	}

	/*************************************************************************/

	parameter_entry::parameter_entry( parameter_mode s, ls_id_index i, type_pointer p)
		: partype( s), idx( i), ltype( p)
	{
	}

	std::ostream & operator<<( std::ostream & o, const parameter_entry & e)
	{
		switch ( e.partype ) 
		{
		case PMODE_BY_VALUE: 
		case PMODE_BY_REFERENCE:
			o << e.partype << "(\"" << idftext( e.idx) << "\"," << e.ltype << ")"; 
			break;
		default: 
			o << e.partype << "(\"" << idftext( e.idx) << "\"," << e.ltype << ")"; 
			break;
		}

		return o;
	}

	std::string parameter_entry::operator()( std::ostream & oo, DumpUniquizer & uniq) const
	{
		std::ostringstream o;

		switch ( partype ) 
		{
		case PMODE_BY_VALUE: 
		case PMODE_BY_REFERENCE:
			o << "\t\t\t\t<" << partype << " name=\"" << idftext( idx) << "\" type=\"" << uniq.uniquize_type( oo, ltype) << "\"/>" << std::endl; 
			break;
		default: 
			o << "\t\t\t\t<" << partype << " name=\"" << idftext( idx) << "\" type=\"" << uniq.uniquize_type( oo, ltype) << "\"/>" << std::endl; 
			break;
		}

		return o.str();
	}

	/*************************************************************************/

	void parameter_list::push_back( const parameter_entry & v)
	{
		v_.push_back( v);
	}

	void parameter_list::append( const parameter_list & l2)
	{
		v_.insert( v_.end(), l2.v_.begin(), l2.v_.end());
	}

	parameter_vector::size_type parameter_list::size() const
	{
		return v_.size();
	}

	std::ostream & operator<<( std::ostream & o, const parameter_list & e)
	{
		o << "[";
		parameter_vector::const_iterator it = e.v_.begin();
		if ( it != e.v_.end() )
		{
			o << std::endl << "\t\t";
			o << * it;
			for ( ++it; it != e.v_.end(); ++it)
			{
				o << ",";
				o << std::endl << "\t\t";
				o << * it;
			}
			o << std::endl << "\t";
		}
		o << "]";
		return o;
	}

	std::string parameter_list::operator()( std::ostream & oo, DumpUniquizer & uniq) const
	{
		std::ostringstream o;

		o << "\t\t\t<parameters>" << std::endl;
		parameter_vector::const_iterator it = v_.begin();
		for ( ; it != v_.end(); ++it)
		{
			o << (* it)( oo, uniq);
		}
		o << "\t\t\t</parameters>" << std::endl;

		return o.str();
	}

	/*************************************************************************/

	field_entry::field_entry( ls_id_index i, type_pointer p)
		: idx( i), ltype( p)
	{
	}

	void field_list::push_back( const field_entry & v)
	{
		v_.push_back( v);
	}

	void field_list::append( const field_list & l2)
	{
		v_.insert( v_.end(), l2.v_.begin(), l2.v_.end());
	}

	void field_list::assign_offsets()
	{
		stack_address offset = 0;
		for ( field_vector::iterator it = v_.begin(), ite = v_.end();
			it != ite;
			++ it )
		{
			it->offset_ = offset;
			offset += it->ltype->compute_size();
		}
		total_size_ = offset;
	}

	field_vector::size_type field_list::size() const
	{
		return v_.size();
	}

	std::ostream & operator<<( std::ostream & o, const field_list & e)
	{
		o << "[";
		field_vector::const_iterator it = e.v_.begin();
		if ( it != e.v_.end() )
		{
			o << std::endl << "\t\t";
			o << * it;
			for ( ++it; it != e.v_.end(); ++it)
			{
				o << ",";
				o << std::endl << "\t\t";
				o << * it;
			}
			o << std::endl << "\t";
		}
		o << "]";
		return o;
	}

	std::ostream & operator<<( std::ostream & o, const field_entry & e)
	{
		o << "(\"" << idftext( e.idx) << "\"," << e.ltype << ")"; 

		return o;
	}

};

namespace mlc {

	parameter_list * create_parameter_list()
	{
		return new parameter_list;
	}

	void parameter_list::append_parameter_by_value( ls_id_index idx, type_pointer ltype)
	{
		parameter_entry v( PMODE_BY_VALUE, idx, ltype);
		push_back( v);
	}

	void parameter_list::append_parameter_by_reference( ls_id_index idx, type_pointer ltype)
	{
		parameter_entry v( PMODE_BY_REFERENCE, idx, ltype);
		push_back( v);
	}

	void parameter_list::append_and_kill( parameter_list * ll2)
	{
		append( * ll2);
		delete ll2;
	}

	parameter_list::const_iterator parameter_list::begin() const
	{
		return v_.begin();
	}

	parameter_list::const_iterator parameter_list::end() const
	{
		return v_.end();
	}
	
	
	/*************************************************************************/

	field_list * create_field_list()
	{
		return new field_list;
	}

	void field_list::append_field( ls_id_index idx, type_pointer ltype)
	{
		field_entry v( idx, ltype);
		push_back( v);
	}

	void field_list::append_and_kill( field_list * ll2)
	{
		append( * ll2);
		delete ll2;
	}

	field_list::const_iterator field_list::begin() const
	{
		return v_.begin();
	}

	field_list::const_iterator field_list::end() const
	{
		return v_.end();
	}

	/*************************************************************************/

	local_symbol_tables::local_symbol_tables()
		: lvarsize( 0), retval_( -1)
	{
	}

	local_symbol_tables::~local_symbol_tables()
	{
		std::for_each( llabels.begin(), llabels.end(), check_label());
		std::for_each( locals.begin(), locals.end(), kill_symbol);
	}

	symbol_tables::symbol_tables( bool debug, mlaskal::abstract_ic * aic)
		: nested_( false), gvarsize( 0), myfncname_( 0), debug_( debug),
		aic_( aic)
	{
		//				undef_.idx = types.end();
		//				bool_.idx = types.end();
		//				integer_.idx = types.end();
		//				real_.idx = types.end();
		//				string_.idx = types.end();
		one_ = aic_->get_ls_int().add( 1);
		global_block_ = mlaskal::tr1::shared_ptr< mlaskal::labeled_icblock>( new mlaskal::labeled_icblock);
		global_epilogue_ = icblock_create();
	}

	type_pointer symbol_tables::create_atomic_type( type_category ptype)
	{
		types.push_back( type_entry( types.size(), ptype));
		dump_type( -- types.end());
		return type_pointer( & * -- types.end());
	}

	type_pointer symbol_tables::create_array_type( type_pointer t1, type_pointer t2)
	{
		types.push_back( type_entry( types.size(), TCAT_ARRAY, t1, t2));
		dump_type( -- types.end());
		return type_pointer( & * -- types.end());
	}

	type_pointer symbol_tables::create_record_type( field_list * fldlist, int line)
	{
		fldlist->assign_offsets();
		types.push_back( type_entry( types.size(), TCAT_RECORD, fldlist));
		type_vector::iterator tit = -- types.end();
		tit->fill_record_cache( line);
		dump_type( tit);
		return type_pointer( & * tit);
	}

	void type_entry::fill_record_cache( int line)
	{
		for ( field_vector::const_iterator it = record_begin_(), ite = record_end_();
			it != ite;
			++ it )
		{
			std::pair< record_cache_type::iterator, bool> rv = record_cache.insert(
				record_cache_type::value_type( it->idx, & * it));

			if ( ! rv.second )
			{
				error( DUERR_DUPSYM, line, * it->idx);
			}
		}
	}

	type_pointer symbol_tables::create_range_type( ls_int_index i1, ls_int_index i2)
	{
		types.push_back( type_entry( types.size(), TCAT_RANGE, undef_, undef_, i1, i2));
		dump_type( -- types.end());
		return type_pointer( & * -- types.end());
	}
	
	label_symbol_pointer symbol_tables::find_label( ls_int_index idx)
	{
		if ( nested_ )
		{
			label_map::iterator it = llabels().find( idx);
			if ( it == llabels().end() )
			{
				return label_symbol_pointer();
			}
			return label_symbol_pointer( & it->second);
		}
		else
		{
			label_map::iterator it = glabels.find( idx);
			if ( it == glabels.end() )
			{
				return label_symbol_pointer();
			}
			return label_symbol_pointer( & it->second);
		}
	}

	symbol_pointer symbol_tables::find_symbol( ls_id_index symidx)
	{
		symbol_map::iterator it = locals().find( symidx);
		if ( it == locals().end() )
		{
			it = globals.find( symidx);
			if ( it == globals.end() )
			{
				return symbol_pointer();
			}
		}
		return symbol_pointer( & it->second);
	}

	stack_address type_entry::compute_size_()
	{
		switch ( cat() )
		{
		case TCAT_ARRAY: 
			{
				type_pointer indexltype = arrayType_()->index_type();
				type_pointer baseltype = arrayType_()->element_type();
				if ( ! indexltype->access_range() )
					return baseltype->compute_size();
				int valfrom = * indexltype->access_range()->lowerBound();
				int valto = * indexltype->access_range()->upperBound();
				return (valto - valfrom + 1) * baseltype->compute_size();
			}
		case TCAT_RECORD: 
			{
				return fldlist->total_size_;
			}
		case TCAT_BOOL: 
			return 1;
		case TCAT_RANGE:
		case TCAT_INT:
			return 1;
		case TCAT_REAL:
			return 1;
		case TCAT_STR:
			return 1;
		default:; // !!! internal error
			return 0;
		}
	}

	mlaskal::physical_type type_entry::compute_final_ptype_()
	{
		switch ( cat() )
		{
		case TCAT_ARRAY: 
			{
				type_pointer baseltype = arrayType_()->element_type();
				return baseltype->compute_final_ptype();
			}
		case TCAT_RECORD: 
			return mlaskal::PTYPE_ANY;
		case TCAT_BOOL: 
			return mlaskal::PTYPE_BOOL;
		case TCAT_RANGE:
		case TCAT_INT:
			return mlaskal::PTYPE_INTEGER;
		case TCAT_REAL:
			return mlaskal::PTYPE_REAL;
		case TCAT_STR:
			return mlaskal::PTYPE_STRING;
		default:; // !!! internal error
			return mlaskal::PTYPE_UNDEF;
		}
	}

	void emit_init( mlaskal::tr1::shared_ptr< mlaskal::labeled_icblock> icb, type_pointer ltype)
	{
		switch ( ltype->cat() )
		{
		case TCAT_ARRAY: 
			{
				type_pointer indexltype = ltype->access_array()->index_type();
				type_pointer baseltype = ltype->access_array()->element_type();
				if ( ! indexltype->access_range() )
					break;
				int valfrom = * indexltype->access_range()->lowerBound(), 
					valto = * indexltype->access_range()->upperBound();
				for ( ; valfrom <= valto; ++valfrom )
				{
					emit_init( icb, baseltype);	// recursion
				}
			}
			break;
		case TCAT_RECORD: 
			{
				record_type_pointer rectype = ltype->access_record();				
				for ( record_type::const_iterator it = rectype->begin(), ite = rectype->end();
					it != ite;
					++ it)
				{
					emit_init( icb, it->type());			// recursion
				}
			}
			break;
		case TCAT_BOOL: 
			icb->append_instruction( new ai::INITB);
			break;
		case TCAT_RANGE:
		case TCAT_INT: 
			icb->append_instruction( new ai::INITI);
			break;
		case TCAT_REAL: 
			icb->append_instruction( new ai::INITR);
			break;
		case TCAT_STR: 
			icb->append_instruction( new ai::INITS);
			break;
		default:; // !!! internal error
		}
	}							


	void emit_dtor( icblock_pointer icb, type_pointer ltype)
	{
		switch ( ltype->cat() )
		{
		case TCAT_ARRAY: 
			{
				type_pointer indexltype = ltype->access_array()->index_type();
				type_pointer baseltype = ltype->access_array()->element_type();
				if ( ! indexltype->access_range() )
					break;
				int valfrom = * indexltype->access_range()->lowerBound(), 
					valto = * indexltype->access_range()->upperBound();
				for ( ; valfrom <= valto; ++valfrom )
				{
					emit_dtor( icb, baseltype);	// recursion
				}
			}
			break;
		case TCAT_RECORD: 
			{
				record_type_pointer rectype = ltype->access_record();				
				for ( record_type::const_iterator itb = rectype->begin(), it = rectype->end();
					it != itb;)
				{
					-- it;
					emit_dtor( icb, it->type());			// recursion
				}
			}
			break;
		case TCAT_BOOL: 
			icb->append_instruction( new ai::DTORB);
			break;
		case TCAT_RANGE:
		case TCAT_INT: 
			icb->append_instruction( new ai::DTORI);
			break;
		case TCAT_REAL: 
			icb->append_instruction( new ai::DTORR);
			break;
		case TCAT_STR: 
			icb->append_instruction( new ai::DTORS);
			break;
		default:; // !!! internal error
		}
	}							

	variable_symbol_pointer symbol_tables::add_var( int line, ls_id_index idx, type_pointer ltype)
	{
		std::pair< symbol_map::iterator, bool> rv;
		if ( nested_ )
		{
			mlaskal::labeled_icblock::const_iterator ib = my_function_code()->get_block()->end();
			ic_label bl = mlaskal::uid_all.get_UID();
			my_function_code()->get_block()->add_label( bl);
			emit_init( my_function_code()->get_block(), ltype);
			mlaskal::labeled_icblock::const_iterator ie = my_function_code()->get_block()->end();
			ic_label el = mlaskal::uid_all.get_UID();
			my_function_code()->get_block()->add_label( el);
			
			icblock_pointer tmp = my_function_entry()->epilogue_;
			my_function_entry()->epilogue_ = icblock_create();
			emit_dtor( my_function_entry()->epilogue_, ltype);
			icblock_append_delete( my_function_entry()->epilogue_, tmp);

			my_function_entry()->local_vars_.push_back(
				mlaskal::abstract_function_var(
					idx,
					ltype->compute_final_ptype(),
					ib,
					ie,		// will not work !!!
					ltype->compute_size(),
					lvarsize()));
			rv = locals().insert( symbol_map::value_type( idx, 
				symbol_entry( SKIND_LOCAL_VARIABLE, lvarsize(), ltype, bl, el)));
			lvarsize() += ltype->compute_size();
			if ( rv.second ) dump_local( rv.first);
		}
		else
		{
			mlaskal::labeled_icblock::const_iterator ib = global_block_->end();
			ic_label bl = mlaskal::uid_all.get_UID();
			global_block_->add_label( bl);
			emit_init( global_block_, ltype);
			mlaskal::labeled_icblock::const_iterator ie = global_block_->end();
			ic_label el = mlaskal::uid_all.get_UID();
			global_block_->add_label( el);
			
			icblock_pointer tmp = global_epilogue_;
			global_epilogue_ = icblock_create();
			emit_dtor( global_epilogue_, ltype);
			icblock_append_delete( global_epilogue_, tmp);

			global_vars_.push_back(
				mlaskal::abstract_function_var(
					idx,
					ltype->compute_final_ptype(),
					ib,
					ie,		// will not work !!!
					ltype->compute_size(),
					gvarsize));
			rv = globals.insert( symbol_map::value_type( idx, 
				symbol_entry( SKIND_GLOBAL_VARIABLE, gvarsize, ltype, bl, el)));
			gvarsize += ltype->compute_size();
			if ( rv.second ) dump_global( rv.first);
		}

		if ( ! rv.second )
		{
			error( DUERR_DUPSYM, line, * idx);
			return variable_symbol_pointer();
		}
		return variable_symbol_pointer( & rv.first->second);
	}

	bool_const_symbol_pointer symbol_tables::add_const_bool( int line, ls_id_index idx, bool value)
	{
		symbol_map::value_type val( idx, symbol_entry( SKIND_CONST, value, bool_));
		std::pair< symbol_map::iterator, bool> rv;
		if ( nested_ )
		{
			rv = locals().insert( val);
			if ( rv.second ) dump_local( rv.first);
		}
		else
		{
			rv = globals.insert( val);
			if ( rv.second ) dump_global( rv.first);
		}

		if ( ! rv.second )
		{
			error( DUERR_DUPSYM, line, * idx);
			return bool_const_symbol_pointer();
		}
		return bool_const_symbol_pointer( & rv.first->second);
	}

	int_const_symbol_pointer symbol_tables::add_const_int( int line, ls_id_index idx, ls_int_index symidx)
	{
		symbol_map::value_type val( idx, symbol_entry( SKIND_CONST, symidx, integer_));
		std::pair< symbol_map::iterator, bool> rv;
		if ( nested_ )
		{
			rv = locals().insert( val);
			if ( rv.second ) dump_local( rv.first);
		}
		else
		{
			rv = globals.insert( val);
			if ( rv.second ) dump_global( rv.first);
		}

		if ( ! rv.second )
		{
			error( DUERR_DUPSYM, line, * idx);
			return int_const_symbol_pointer();
		}
		return int_const_symbol_pointer( & rv.first->second);
	}

	real_const_symbol_pointer symbol_tables::add_const_real( int line, ls_id_index idx, ls_real_index symidx)
	{
		symbol_map::value_type val( idx, symbol_entry( SKIND_CONST, symidx, real_));
		std::pair< symbol_map::iterator, bool> rv;
		if ( nested_ )
		{
			rv = locals().insert( val);
			if ( rv.second ) dump_local( rv.first);
		}
		else
		{
			rv = globals.insert( val);
			if ( rv.second ) dump_global( rv.first);
		}

		if ( ! rv.second )
		{
			error( DUERR_DUPSYM, line, * idx);
			return real_const_symbol_pointer();
		}
		return real_const_symbol_pointer( & rv.first->second);
	}

	str_const_symbol_pointer symbol_tables::add_const_str( int line, ls_id_index idx, ls_str_index symidx)
	{
		symbol_map::value_type val( idx, symbol_entry( SKIND_CONST, symidx, string_));
		std::pair< symbol_map::iterator, bool> rv;
		if ( nested_ )
		{
			rv = locals().insert( val);
			if ( rv.second ) dump_local( rv.first);
		}
		else
		{
			rv = globals.insert( val);
			if ( rv.second ) dump_global( rv.first);
		}

		if ( ! rv.second )
		{
			error( DUERR_DUPSYM, line, * idx);
			return str_const_symbol_pointer();
		}
		return str_const_symbol_pointer( & rv.first->second);
	}

	type_symbol_reference symbol_tables::add_type( int line, ls_id_index idx, type_pointer ltype)
	{
		symbol_map::value_type val( idx, symbol_entry( SKIND_TYPE, ltype));
		std::pair< symbol_map::iterator, bool> rv;
		if ( nested_ )
		{
			rv = locals().insert( val);
			if ( rv.second ) dump_local( rv.first);
		}
		else
		{
			rv = globals.insert( val);
			if ( rv.second ) dump_global( rv.first);
		}

		if ( ! rv.second )
		{
			error( DUERR_DUPSYM, line, * idx);
			return type_symbol_reference();
		}
		return type_symbol_reference( & rv.first->second);
	}

	label_symbol_pointer symbol_tables::add_label_entry( int line, ls_int_index idx, ic_label lbl)
	{
		std::pair< label_map::iterator, bool> rv;
		if ( nested_ )
		{
			rv = llabels().insert( label_map::value_type( idx, label_entry( line, idx, lbl)));
			if ( rv.second ) dump_llabel( rv.first);
		}
		else
		{
			rv = glabels.insert( label_map::value_type( idx, label_entry( line, idx, lbl)));
			if ( rv.second ) dump_glabel( rv.first);
		}

		if ( ! rv.second )
		{
			error( DUERR_DUPLABEL, line, * idx);
			return label_symbol_pointer();
		}
		return label_symbol_pointer( & rv.first->second);
	}

	function_symbol_pointer symbol_tables::add_fnc( int line, ls_id_index idx, type_pointer ltype, parameter_list * parlist)
	{
		std::pair< symbol_map::iterator, bool> rv;
		if ( nested_ )
		{
			// !!! internal error - must not be local
			return function_symbol_pointer();
		}
		else
		{
			mlaskal::abstract_function_params params;
			mlaskal::ICSTKOFFS defsize = 0;
			mlaskal::ICSTKOFFS rv_stkoffs = 0;

			for ( parameter_list::const_iterator pit = parlist->begin(); pit != parlist->end(); ++pit)
			{
				switch ( pit->partype )
				{
				case PMODE_BY_VALUE:
					defsize += pit->ltype->compute_size();
					break;
				case PMODE_BY_REFERENCE:
					defsize += 1;
					break;
				default:
					// do nothing - fuck off GCC
					break;
				}
			}
			rv_stkoffs = - defsize - ltype->compute_size();
			for ( parameter_list::const_iterator pit2 = parlist->begin(); pit2 != parlist->end(); ++pit2)
			{
				switch ( pit2->partype )
				{
				case PMODE_BY_VALUE:
					params.push_back( mlaskal::abstract_function_param(
						pit2->idx, 
						pit2->ltype->compute_final_ptype(), 
						pit2->ltype->compute_size(),
						- defsize));
					defsize -= pit2->ltype->compute_size();
					break;
				case PMODE_BY_REFERENCE:
					params.push_back( mlaskal::abstract_function_param(
						pit2->idx, 
						mlaskal::PTYPE_ADDR, 
						1,
						- defsize));
					defsize -= 1;
					break;
				default:
					// do nothing - fuck off GCC
					break;
				}
			}

			mlaskal::tr1::shared_ptr< mlaskal::labeled_icblock> empty_code( 
				new mlaskal::labeled_icblock);

			aic_->get_abstract_functions().push_back( 
				mlaskal::abstract_function( 
					idx,
					mlaskal::abstract_function_rv(
						ltype->compute_final_ptype(),
						ltype->compute_size(),
						rv_stkoffs),
					params,
					mlaskal::abstract_function_vars(),
					empty_code));   

			ic_function_pointer my_code = -- aic_->get_abstract_functions().end();

			rv = globals.insert( symbol_map::value_type( idx, symbol_entry( SKIND_FUNCTION, -1, ltype, my_code, new local_symbol_tables)));
			if ( rv.second ) 
			{
				rv.first->second.parlist = parlist;
				dump_global( rv.first);
			}
		}

		if ( ! rv.second )
		{
			error( DUERR_DUPSYM, line, * idx);
			return function_symbol_pointer();
		}
		return function_symbol_pointer( & rv.first->second);
	}

	procedure_symbol_pointer symbol_tables::add_proc( int line, ls_id_index idx, parameter_list * parlist)
	{
		std::pair< symbol_map::iterator, bool> rv;
		if ( nested_ )
		{
			// !!! internal error - must not be local
			return procedure_symbol_pointer();
		}
		else
		{
			mlaskal::abstract_function_params params;
			mlaskal::ICSTKOFFS defsize = 0;

			for ( parameter_list::const_iterator pit = parlist->begin(); pit != parlist->end(); ++pit)
			{
				switch ( pit->partype )
				{
				case PMODE_BY_VALUE:
					defsize += pit->ltype->compute_size();
					break;
				case PMODE_BY_REFERENCE:
					defsize += 1;
					break;
				default:
					// do nothing - fuck off GCC
					break;
				}
			}

			for ( parameter_list::const_iterator pit2 = parlist->begin(); pit2 != parlist->end(); ++pit2)
			{
				switch ( pit2->partype )
				{
				case PMODE_BY_VALUE:
					params.push_back( mlaskal::abstract_function_param(
						pit2->idx, 
						pit2->ltype->compute_final_ptype(), 
						pit2->ltype->compute_size(),
						- defsize));
					defsize -= pit2->ltype->compute_size();
					break;
				case PMODE_BY_REFERENCE:
					params.push_back( mlaskal::abstract_function_param(
						pit2->idx, 
						mlaskal::PTYPE_ADDR, 
						1,
						- defsize));
					defsize -= 1;
					break;
				default:
					// do nothing - fuck off GCC
					break;
				}
			}

			mlaskal::tr1::shared_ptr< mlaskal::labeled_icblock> empty_code( 
				new mlaskal::labeled_icblock);

			aic_->get_abstract_functions().push_back( 
				mlaskal::abstract_function( 
					idx,
					params,
					mlaskal::abstract_function_vars(),
					empty_code));   // don't have any icblock yet

			ic_function_pointer my_code = -- aic_->get_abstract_functions().end();

			rv = globals.insert( symbol_map::value_type( idx, symbol_entry( SKIND_PROCEDURE, -1, undef_, my_code, new local_symbol_tables)));
			if ( rv.second ) rv.first->second.parlist = parlist;
			if ( rv.second ) dump_global( rv.first);
		}

		if ( ! rv.second )
		{
			error( DUERR_DUPSYM, line, * idx);
			return procedure_symbol_pointer();
		}
		return procedure_symbol_pointer( & rv.first->second);
	}

	bool symbol_tables::set_subprogram_code( ls_id_index idx, icblock_pointer icb)
	{
		symbol_map::iterator mit = globals.find( idx);
		if ( mit == globals.end() 
			|| ( mit->second.symtype != SKIND_PROCEDURE && mit->second.symtype != SKIND_FUNCTION ) )
		{
			return false;
		}

		icblock_append_delete( mit->second.magic->get_block().get(), icb);

		icblock_append_delete( mit->second.magic->get_block().get(), mit->second.epilogue_);

		mit->second.magic->get_block()->append_instruction( new ai::RET);

		// correct the icip pointers in local_vars_ and copy them to the ic function entry

		for ( mlaskal::abstract_function_vars::const_iterator it = mit->second.local_vars_.begin(), ite = mit->second.local_vars_.end();
			it != ite;
			++ it)
		{
			symbol_map::const_iterator sit = mit->second.local_tables_->locals.find( it->get_name());
			assert( sit != mit->second.local_tables_->locals.end());

			mlaskal::labeled_icblock::const_iterator start = mit->second.magic->get_block()->locate_label( sit->second.icip_begin_label);
			mlaskal::labeled_icblock::const_iterator end = mit->second.magic->get_block()->locate_label( sit->second.icip_end_label);
			mit->second.magic->get_vars().push_back(
				mlaskal::abstract_function_var( 
					it->get_name(),
					it->get_ptype(),
					start,
					end,
					it->get_size(),
					it->get_stkoffs()));
		}

		return true;
	}

	bool symbol_tables::set_main_code( ls_id_index idx, icblock_pointer icb)
	{
		icblock_append_delete( global_block_.get(), icb);

		icblock_append_delete( global_block_.get(), global_epilogue_);

		global_block_->append_instruction( new ai::HALT);

		// correct the icip pointers in global_vars_

		mlaskal::abstract_function_vars global_vars2;

		for ( mlaskal::abstract_function_vars::const_iterator it = global_vars_.begin(), ite = global_vars_.end();
			it != ite;
			++ it)
		{
			symbol_map::const_iterator sit = globals.find( it->get_name());
			assert( sit != globals.end());

			mlaskal::labeled_icblock::const_iterator start = global_block_->locate_label( sit->second.icip_begin_label);
			mlaskal::labeled_icblock::const_iterator end = global_block_->locate_label( sit->second.icip_end_label);
			global_vars2.push_back(
				mlaskal::abstract_function_var( 
					it->get_name(),
					it->get_ptype(),
					start,
					end,
					it->get_size(),
					it->get_stkoffs()));
		}

		aic_->get_abstract_functions().push_back(
				mlaskal::abstract_function( 
					idx,
					global_vars2,
					global_block_));   

		return true;
	}

	bool symbol_tables::enter( int line, ls_id_index idx)
	{
		symbol_map::iterator mit = globals.find( idx);
		if ( mit == globals.end() 
			|| ( mit->second.symtype != SKIND_PROCEDURE && mit->second.symtype != SKIND_FUNCTION ) )
		{
			return false;
		}

		dump_enter( idx);

		nested_ = true;
		myfncname_ = idx;

		// copy parameters into local vars

		int defsize = 0;

		for ( parameter_list::const_iterator pit = mit->second.parlist->begin(); pit != mit->second.parlist->end(); ++pit)
		{
			//type_pointer parltype = undef_;
			//ls_id_index idx;
			//parameter_mode partype = parlist_at( mit->second.parlist, i, & idx, & parltype);
			
			switch ( pit->partype )
			{
			case PMODE_BY_VALUE:
				defsize += pit->ltype->compute_size();
				break;
			case PMODE_BY_REFERENCE:
				defsize += 1;
				break;
			case PMODE_UNDEF:			// dazed GCC
				break;
			}
		}

		if ( mit->second.symtype == SKIND_FUNCTION )
			retval_() = - defsize - mit->second.ltype->compute_size();
		else
			retval_() = 0;

		for ( parameter_list::const_iterator pit = mit->second.parlist->begin(); pit != mit->second.parlist->end(); ++pit)
		{
			//type_pointer parltype = undef_;
			//ls_id_index idx;
			//parameter_mode partype = parlist_at( mit->second.parlist, i, & idx, & parltype);
			ic_label bl = mlaskal::uid_all.get_UID();
			my_function_code()->get_block()->add_label( bl);
			ic_label el = mlaskal::uid_all.get_UID();
			my_function_code()->get_block()->add_label( el);

			std::pair< symbol_map::iterator, bool> rv;
			switch ( pit->partype )
			{
			case PMODE_BY_VALUE:
				rv = locals().insert( symbol_map::value_type( pit->idx, symbol_entry( SKIND_LOCAL_VARIABLE, - defsize, pit->ltype, bl, el)));
				defsize -= pit->ltype->compute_size();
				break;
			case PMODE_BY_REFERENCE:
				rv = locals().insert( symbol_map::value_type( pit->idx, symbol_entry( SKIND_PARAMETER_BY_REFERENCE, - defsize, pit->ltype, bl, el)));
				defsize -= 1;
				break;
			case PMODE_UNDEF:			// dazed GCC
				break;
			}
			if ( rv.second ) dump_local( rv.first);

			if ( ! rv.second )
			{
				error( DUERR_DUPSYM, line, * idx);
			}

		}

		lvarsize() = 1;
		return true;
	}

	void symbol_tables::leave( int /*line*/)
	{
		nested_ = false;
		myfncname_ = 0;

		dump_leave();
	}

	ls_id_index symbol_tables::my_function_name() const
	{
		return myfncname_;
	}

	mlc::ls_int_type::const_pointer symbol_tables::one()
	{
		return one_;
	}

	stack_address symbol_tables::my_return_address()
	{
		return retval_();
	}

	ic_function_pointer symbol_tables::my_function_code()
	{
		symbol_map::iterator mit = globals.find( myfncname_);
		if ( mit == globals.end() 
			|| ( mit->second.symtype != SKIND_FUNCTION
				&& mit->second.symtype != SKIND_PROCEDURE ) )
		{
			return aic_->get_abstract_functions().end();
		}

		return mit->second.magic;
	}

	symbol_entry * symbol_tables::my_function_entry()
	{
		symbol_map::iterator mit = globals.find( myfncname_);
		if ( mit == globals.end() 
			|| ( mit->second.symtype != SKIND_FUNCTION
				&& mit->second.symtype != SKIND_PROCEDURE ) )
		{
			return 0;
		}

		return & mit->second;
	}

	const symbol_entry * symbol_tables::my_function_entry() const
	{
		symbol_map::const_iterator mit = globals.find( myfncname_);
		if ( mit == globals.end() 
			|| ( mit->second.symtype != SKIND_FUNCTION
				&& mit->second.symtype != SKIND_PROCEDURE ) )
		{
			return 0;
		}

		return & mit->second;
	}

	type_pointer symbol_tables::logical_undef()
	{
		return undef_;
	}

	type_pointer symbol_tables::logical_bool()
	{
		return bool_;
	}

	type_pointer symbol_tables::logical_integer()
	{
		return integer_;
	}

	type_pointer symbol_tables::logical_real()
	{
		return real_;
	}

	type_pointer symbol_tables::logical_string()
	{
		return string_;
	}

	void symbol_tables::init_builtins()
	{
		dump_builtins();

		bool_ = create_atomic_type( TCAT_BOOL);
		char s_bool[] = "BOOLEAN";
		add_type( 0, aic_->get_ls_id().add( s_bool), bool_);

		integer_ = create_atomic_type( TCAT_INT);
		char s_integer[] = "INTEGER";
		add_type( 0, aic_->get_ls_id().add( s_integer), integer_);

		real_ = create_atomic_type( TCAT_REAL);
		char s_real[] = "REAL";
		add_type( 0, aic_->get_ls_id().add( s_real), real_);

		string_ = create_atomic_type( TCAT_STR);
		char s_string[] = "STRING";
		add_type( 0, aic_->get_ls_id().add( s_string), string_);

		char s_false[] = "FALSE";
		add_const_bool( 0, aic_->get_ls_id().add( s_false), false);

		char s_true[] = "TRUE";
		add_const_bool( 0, aic_->get_ls_id().add( s_true), true);

		parameter_list * readi_pl; 
		readi_pl = create_parameter_list();
		char s_readi[] = "READI";
		ls_id_index idx_readi = aic_->get_ls_id().add( s_readi);
		add_fnc( 0, idx_readi, integer_, readi_pl);
		mlaskal::labeled_icblock *licb_readi=new mlaskal::labeled_icblock;
		licb_readi->append_instruction(new ai::FNCREADI());
		licb_readi->append_instruction(new ai::LSTI(-1));
		set_subprogram_code( idx_readi, licb_readi);

		parameter_list * readr_pl; 
		readr_pl = create_parameter_list();
		char s_readr[] = "READR";
		ls_id_index idx_readr = aic_->get_ls_id().add( s_readr);
		add_fnc( 0, idx_readr, real_, readr_pl);
		mlaskal::labeled_icblock *licb_readr=new mlaskal::labeled_icblock;
		licb_readr->append_instruction(new ai::FNCREADR());
		licb_readr->append_instruction(new ai::LSTR(-1));
		set_subprogram_code( idx_readr, licb_readr);

		parameter_list * reads_pl; 
		reads_pl = create_parameter_list();
		char s_reads[] = "READS";
		ls_id_index idx_reads = aic_->get_ls_id().add( s_reads);
		add_fnc( 0, idx_reads, string_, reads_pl);
		mlaskal::labeled_icblock *licb_reads=new mlaskal::labeled_icblock;
		licb_reads->append_instruction(new ai::FNCREADS());
		licb_reads->append_instruction(new ai::LSTS(-1));
		set_subprogram_code( idx_reads, licb_reads);

		char s_p[] = "P";
		ls_id_index idx_p = aic_->get_ls_id().add( s_p);

		parameter_list * writei_pl; 
		writei_pl = create_parameter_list();
		writei_pl->append_parameter_by_value( idx_p, integer_);
		char s_writei[] = "WRITEI";
		ls_id_index idx_writei = aic_->get_ls_id().add( s_writei);
		add_proc( 0, idx_writei, writei_pl);
		mlaskal::labeled_icblock *licb_writei=new mlaskal::labeled_icblock;
		licb_writei->append_instruction(new ai::LLDI(-1));
		licb_writei->append_instruction(new ai::FNCWRITEI());
		set_subprogram_code( idx_writei, licb_writei);

		parameter_list * writer_pl; 
		writer_pl = create_parameter_list();
		writer_pl->append_parameter_by_value( idx_p, real_);
		char s_writer[] = "WRITER";
		ls_id_index idx_writer = aic_->get_ls_id().add( s_writer);
		add_proc( 0, idx_writer, writer_pl);
		mlaskal::labeled_icblock *licb_writer=new mlaskal::labeled_icblock;
		licb_writer->append_instruction(new ai::LLDR(-1));
		licb_writer->append_instruction(new ai::FNCWRITER());
		set_subprogram_code( idx_writer, licb_writer);

		parameter_list * writes_pl; 
		writes_pl = create_parameter_list();
		writes_pl->append_parameter_by_value( idx_p, string_);
		char s_writes[] = "WRITES";
		ls_id_index idx_writes = aic_->get_ls_id().add( s_writes);
		add_proc( 0, idx_writes, writes_pl);
		mlaskal::labeled_icblock *licb_writes=new mlaskal::labeled_icblock;
		licb_writes->append_instruction(new ai::LLDS(-1));
		licb_writes->append_instruction(new ai::FNCWRITES());
		set_subprogram_code( idx_writes, licb_writes);

		parameter_list * writeln_pl; 
		writeln_pl = create_parameter_list();
		char s_writeln[] = "WRITELN";
		ls_id_index idx_writeln = aic_->get_ls_id().add( s_writeln);
		add_proc( 0, idx_writeln, writeln_pl);
		mlaskal::labeled_icblock *licb_writeln=new mlaskal::labeled_icblock;
		licb_writeln->append_instruction(new ai::FNCWRITELN());
		set_subprogram_code( idx_writeln, licb_writeln);

		dump_main();
	}

	void symbol_tables::dump_enter( ls_id_index idx) const
	{
		if ( debug_ ) std::cout << "*** Entering block \"" << idftext( idx) << "\" ***" << std::endl;
	}

	void symbol_tables::dump_leave() const
	{
		if ( debug_ ) std::cout << "*** Leaving block ***" << std::endl;
	}

	void symbol_tables::dump_builtins() const
	{
		if ( debug_ ) std::cout << "*** Built-in definitions ***" << std::endl;
	}

	void symbol_tables::dump_main() const
	{
		if ( debug_ ) std::cout << "*** Program ***" << std::endl;
	}

	void symbol_tables::dump_type( const type_vector::const_iterator & tit) const
	{
		if ( debug_ ) dump_type( std::cout, tit);
	}

	void symbol_tables::dump_glabel( const label_map::const_iterator & lit) const
	{
		if ( debug_ ) dump_glabel( std::cout, lit);
	}

	void symbol_tables::dump_global( const symbol_map::const_iterator & sit) const
	{
		if ( debug_ ) dump_global( std::cout, sit);
	}

	void symbol_tables::dump_llabel( const label_map::const_iterator & lit) const
	{
		if ( debug_ ) dump_llabel( std::cout, lit);
	}

	void symbol_tables::dump_local( const symbol_map::const_iterator & sit) const
	{
		if ( debug_ ) dump_local( std::cout, sit);
	}

	std::ostream & symbol_tables::dump_type( std::ostream & o, const type_vector::const_iterator & tit) const
	{
		return o << "T" << tit->idx_ << " = " << * tit << std::endl;
	}

	std::ostream & symbol_tables::dump_glabel( std::ostream & o, const label_map::const_iterator & lit) const
	{
		return o << "G\"" << intval( lit->first) << "\" = LABEL(" << lit->second.label() << ")" << std::endl;
	}

	std::ostream & symbol_tables::dump_global( std::ostream & o, const symbol_map::const_iterator & sit) const
	{
		return o << "G\"" << idftext( sit->first) << "\" = " << sit->second << std::endl;
	}

	std::ostream & symbol_tables::dump_llabel( std::ostream & o, const label_map::const_iterator & lit) const
	{
		return o << "L\"" << intval( lit->first) << "\" = LABEL(" << lit->second.label() << ")" << std::endl;
	}

	std::ostream & symbol_tables::dump_local( std::ostream & o, const symbol_map::const_iterator & sit) const
	{
		return o << "L\"" << idftext( sit->first) << "\" = " << sit->second << std::endl;
	}

	std::string local_symbol_tables::dump_locals( std::ostream & oo, DumpUniquizer & uniq) const
	{
		std::ostringstream o;

		o << "\t\t\t<labels>" << std::endl;
		label_map::const_iterator lit = llabels.begin();
		label_map::const_iterator lite = llabels.end();
		for ( ; lit != lite; ++lit )
		{
			dump_llabel( o, lit, oo, uniq);
		}
		o << "\t\t\t</labels>" << std::endl;
		o << "\t\t\t<symbols>" << std::endl;
		symbol_map::const_iterator sit = locals.begin();
		symbol_map::const_iterator site = locals.end();
		for ( ; sit != site; ++sit )
		{
			dump_local( o, sit, oo, uniq);
		}
		o << "\t\t\t</symbols>" << std::endl;

		return o.str();
	}

	std::string symbol_entry::operator()( const std::string & indent, const std::string & name, std::ostream & oo, DumpUniquizer & uniq) const
	{
		std::ostringstream o;

		o << indent << "<" << symtype << " name=\"" << name << "\"";

		switch ( symtype ) 
		{
		case SKIND_PROCEDURE: 
			o << ">" << std::endl;
			o << (* parlist)( oo, uniq);
			o << local_tables_->dump_locals( oo, uniq); 
			o << indent << "</" << symtype << ">" << std::endl;
			break;
		case SKIND_FUNCTION: 
			o << " type=\"" << uniq.uniquize_type( oo, ltype) << "\">" << std::endl; // beware: uniquize_type may have side effects
			o << (* parlist)( oo, uniq);
			o << local_tables_->dump_locals( oo, uniq); 
			o << indent << "</" << symtype << ">" << std::endl;
			break;
		case SKIND_GLOBAL_VARIABLE: 
			o << " type=\"" << uniq.uniquize_type( oo, ltype) << "\"/>" << std::endl; 
			break;
		case SKIND_LOCAL_VARIABLE: 
		case SKIND_PARAMETER_BY_REFERENCE:
			o << " type=\"" << uniq.uniquize_type( oo, ltype) << "\"/>" << std::endl; 
			break;
		case SKIND_TYPE: 
			o << " type=\"" << uniq.uniquize_type( oo, ltype) << "\"/>" << std::endl; 
			break;
		case SKIND_CONST: 
			o << " value=\"";
			switch ( ltype->cat() )
			{
			case TCAT_BOOL: o << value; break;
			case TCAT_INT: o << * intidx; break;
			case TCAT_REAL: o << * realidx; break;
			case TCAT_STR: o << * stridx; break;
			default: o << "?"; break;
			}
			o << "\" type=\"" << uniq.uniquize_type( oo, ltype) << "\"/>" << std::endl; 
			break;
		case SKIND_UNDEF: 
		default: 
			o << "/>" << std::endl; 
			break;
		}

		return o.str();
	}

	std::ostream & symbol_tables::dump_glabel( std::ostream & o, const label_map::const_iterator & lit, std::ostream &, DumpUniquizer &) const
	{
		return o << "\t\t<LABEL name=\"" << intval( lit->first) << "\"/>" << std::endl;
	}

	std::ostream & symbol_tables::dump_global( std::ostream & o, const symbol_map::const_iterator & sit, std::ostream & oo, DumpUniquizer & uniq) const
	{
		return o << sit->second( "\t\t", idftext( sit->first), oo, uniq);
	}

	std::ostream & local_symbol_tables::dump_llabel( std::ostream & o, const label_map::const_iterator & lit, std::ostream &, DumpUniquizer &) const
	{
		return o << "\t\t\t\t<LABEL name=\"" << intval( lit->first) << "\"/>" << std::endl;
	}

	std::ostream & local_symbol_tables::dump_local( std::ostream & o, const symbol_map::const_iterator & sit, std::ostream & oo, DumpUniquizer & uniq) const
	{
		return o << sit->second( "\t\t\t\t", idftext( sit->first), oo, uniq);
	}

	std::ostream & symbol_tables::dump( std::ostream & out) const
	{
		std::ostringstream o;
		std::ostringstream oo;

		DumpUniquizer uniq;

		o << "\t<labels>" << std::endl;
		label_map::const_iterator lit = glabels.begin();
		label_map::const_iterator lite = glabels.end();
		for ( ; lit != lite; ++lit )
		{
			dump_glabel( o, lit, oo, uniq);
		}
		o << "\t</labels>" << std::endl;
		o << "\t<symbols>" << std::endl;
		symbol_map::const_iterator sit = globals.begin();
		symbol_map::const_iterator site = globals.end();
		for ( ; sit != site; ++sit )
		{
			dump_global( o, sit, oo, uniq);
		}
		o << "\t</symbols>" << std::endl;

		out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;
		out << "<symbol_tables>" << std::endl;
		out << "\t<types>" << std::endl;
		out << oo.str();
		out << "\t</types>" << std::endl;
		out << o.str();
		out << "</symbol_tables>" << std::endl;

		return out;
	}

	bool symbol_tables::nested() const { return nested_; }

	ic_label label_entry::label() const
	{
		return magic;
	}

	void label_entry::goto_found( int line)
	{
		if ( ! have_goto )
		{
			have_goto = true;
			goto_line = line;
		}
	}

	bool label_entry::label_found( int line)
	{
		if ( ! have_label )
		{
			have_label = true;
			label_line = line;
			return true;
		}
		else
		{
			error( DUERR_DUPLABELDEF, line, * id);
			return false;
		}
	}

	void label_entry::check()
	{		
		if ( ! have_label )
		{
			if ( ! have_goto )
			{
				error( DUERR_NOLABELUSE, decl_line, * id);
			}
			else
			{
				error( DUERR_MISLABELDEF, decl_line, * id);
				// !!! add fake label to ic
			}
		}
		else
		{
			if ( ! have_goto )
			{
				error( DUERR_NOLABELGOTO, decl_line, * id);
			}
		}
	}


	/*************************************************************************/

	bool identical_type( type_pointer a, type_pointer b)
	{
		return a->get_idx() == b->get_idx();
	}

	
	std::ostream &operator<<(std::ostream &os, type_category typecat) {
		static const char *ptypes[] = {
			"UNDEF", "ARRAY", "RECORD", "BOOLEAN", "RANGE", "INTEGER", "REAL", "STRING"
		};
		os << ptypes[ typecat];
		return os;
	}

	/*****************************************/
};
/*****************************************/

