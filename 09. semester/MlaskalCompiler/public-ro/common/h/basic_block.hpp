#ifndef GUARD_MLASKAL_BASIC_BLOCK_H_
#define GUARD_MLASKAL_BASIC_BLOCK_H_

/*

		basic_block.hpp

		Basic block representation (blocks of instructions without jumps and a graph of jumps)

		Dokis, 2007

*/

#include <deque>
#include <map>
#include <set>
#include <stdexcept>
#include <utility>
#include <iostream>

#include "uid_gen.hpp"
#include "abstract_instr.hpp"

namespace mlaskal {


	/// @cond INTERNAL

	namespace util
	{
		template<typename Iter>
		class first_of_pair_iterator
		{
		public:
			typedef typename Iter::iterator_category iterator_category;
			typedef typename Iter::value_type::first_type value_type;
			typedef typename Iter::difference_type difference_type;
			typedef typename Iter::value_type::first_type* pointer;
			typedef typename Iter::value_type::first_type& reference;

			first_of_pair_iterator(const first_of_pair_iterator& other) : it_(other.it_) {}
			first_of_pair_iterator(const Iter& it) : it_(it) {}
			first_of_pair_iterator() : it_() {}
			const first_of_pair_iterator& operator=(const first_of_pair_iterator& other) { it_=other.it_; }
			bool operator==(const first_of_pair_iterator& other) const { return it_==other.it_; }
			bool operator!=(const first_of_pair_iterator& other) const { return it_!=other.it_; }
			bool operator<(const first_of_pair_iterator& other) const { return it_<other.it_; }
			bool operator>(const first_of_pair_iterator& other) const { return it_>other.it_; }
			bool operator<=(const first_of_pair_iterator& other) const { return it_<=other.it_; }
			bool operator>=(const first_of_pair_iterator& other) const { return it_>=other.it_; }
			reference operator*() { return it_->first; }
			pointer operator->() { return &(it_->first); }
			first_of_pair_iterator& operator++() { ++it_; return *this; }
			first_of_pair_iterator operator++(int) { Iter res(it_); ++it_; return res; }
			first_of_pair_iterator& operator--() { --it_; return *this; }
			first_of_pair_iterator operator--(int) { Iter res(it_); --it_; return res; }
			reference operator[] (difference_type idx) { return it_[idx].second; }
			difference_type operator- (const first_of_pair_iterator& other) const { return it_-other.it_; }
			first_of_pair_iterator operator- (difference_type distance) const { return first_of_pair_iterator(it_-distance); }
			first_of_pair_iterator operator+ (difference_type distance) const { return first_of_pair_iterator(it_+distance); }
			const first_of_pair_iterator& operator-= (difference_type distance) { it_-=distance; return *this; }
			const first_of_pair_iterator& operator+= (difference_type distance) { it_+=distance; return *this; }
		private:
			Iter it_;
		};
		template<typename Iter>
		first_of_pair_iterator<Iter> operator+(typename first_of_pair_iterator<Iter>::difference_type distance,const first_of_pair_iterator<Iter>& iter) { return iter+distance; }

		template<typename Iter>
		class second_of_pair_iterator
		{
		public:
			typedef typename Iter::iterator_category iterator_category;
			typedef typename Iter::value_type::second_type value_type;
			typedef typename Iter::difference_type difference_type;
			typedef const typename Iter::value_type::second_type* const_pointer;
			typedef const typename Iter::value_type::second_type& const_reference;

			second_of_pair_iterator(const second_of_pair_iterator& other) : it_(other.it_) {}
			second_of_pair_iterator(const Iter& it) : it_(it) {}
			second_of_pair_iterator() : it_() {}
			const second_of_pair_iterator& operator=(const second_of_pair_iterator& other) { it_=other.it_; }
			bool operator==(const second_of_pair_iterator& other) const { return it_==other.it_; }
			bool operator!=(const second_of_pair_iterator& other) const { return it_!=other.it_; }
			bool operator<(const second_of_pair_iterator& other) const { return it_<other.it_; }
			bool operator>(const second_of_pair_iterator& other) const { return it_>other.it_; }
			bool operator<=(const second_of_pair_iterator& other) const { return it_<=other.it_; }
			bool operator>=(const second_of_pair_iterator& other) const { return it_>=other.it_; }
			const_reference operator*() const { return it_->second; }
			const_pointer operator->() const { return &(it_->second); }
			second_of_pair_iterator& operator++() { ++it_; return *this; }
			second_of_pair_iterator operator++(int) { Iter res(it_); ++it_; return res; }
			second_of_pair_iterator& operator--() { --it_; return *this; }
			second_of_pair_iterator operator--(int) { Iter res(it_); --it_; return res; }
			const_reference operator[] (difference_type idx) const { return it_[idx].second; }
			difference_type operator- (const second_of_pair_iterator& other) const { return it_-other.it_; }
			second_of_pair_iterator operator- (difference_type distance) const { return second_of_pair_iterator(it_-distance); }
			second_of_pair_iterator operator+ (difference_type distance) const { return second_of_pair_iterator(it_+distance); }
			const second_of_pair_iterator& operator-= (difference_type distance) { it_-=distance; return *this; }
			const second_of_pair_iterator& operator+= (difference_type distance) { it_+=distance; return *this; }
		private:
			Iter it_;
		};
		template<typename Iter>
		second_of_pair_iterator<Iter> operator+(typename second_of_pair_iterator<Iter>::difference_type distance,const second_of_pair_iterator<Iter>& iter) { return iter+distance; }

	}

	/// @endcond

	template<typename I>
	class basic_block;

	///jump (edge in basic block graph)
	template<typename I>
	class jump
	{
		typedef typename std::list<basic_block<I> > basic_block_internal_storage;
	public:
		typedef typename std::list<basic_block<I> >::iterator basic_block_iterator;
		///source block (the jump/edge originates at the end of this block)
		basic_block_iterator get_source() const { return source_; }
		///destination block (the jump/edge ends at the beginning of this block)
		basic_block_iterator get_target() const { return target_; }

		/// @cond INTERNAL
   		jump(basic_block_iterator source, basic_block_iterator target) : source_(source), target_(target) {}
		/// @endcond
	private:
		basic_block_iterator source_;
		basic_block_iterator target_;
	};

	///set of colored jumps (jumps originating in one basic block together with their color)
	template<typename I>
	class jumps
	{
		typedef std::map<uid_all_type::UID_type, jump<I> > jumps_internal_storage;
	public:
		/// container requirement: value_type
		typedef typename jumps_internal_storage::value_type value_type;
		/// container requirement: reference
		typedef typename jumps_internal_storage::reference reference;
		/// container requirement: const_reference
		typedef typename jumps_internal_storage::const_reference const_reference;
		/// container requirement: size_type
		typedef typename jumps_internal_storage::size_type size_type;
		/// container requirement: iterator
		typedef typename jumps_internal_storage::iterator iterator;
		/// container requirement: const_iterator
		typedef typename jumps_internal_storage::const_iterator const_iterator;

		/// @cond INTERNAL

		std::pair<iterator,bool> insert(uid_all_type::UID_type color, typename jump<I>::basic_block_iterator source, typename jump<I>::basic_block_iterator target)
		{
				return jumps_.insert(std::make_pair(color,jump<I>(source,target)));
		}
		/// @endcond

		///first jump in the set
		iterator begin() { return jumps_.begin(); }
		///one position behind last jump
		iterator end() { return jumps_.end(); }
		///first jump in the set
		const_iterator begin() const { return jumps_.begin(); }
		///one position behind last jump
		const_iterator end() const { return jumps_.end(); }

		///Get jump by its color. Returns end() if jump is not found.
		const const_iterator find_by_color(uid_all_type::UID_type color) const
		{
			const_iterator it=jumps_.find(color);
			return it;
		}

		///number of jumps in the set
		size_type size() const { return jumps_.size(); }
		///check if the set contains no jumps
		bool empty() const { return jumps_.empty(); }

	private:
		jumps_internal_storage jumps_;
	};

	///one basic block
	template<typename I>
	class basic_block
	{
	public:
		/// @cond INTERNAL
		typedef ::std::vector<typename jumps<I>::const_iterator> incoming_jumps_internal_type;
		typedef jump<I> jump_type;
		/// @endcond

		/// container for jumps leading from the basic block
		typedef jumps<I> outgoing_jumps_type;

	public:
		/// @cond INTERNAL
		std::pair<typename outgoing_jumps_type::iterator,bool> add_outgoing_jump(uid_all_type::UID_type color, typename jump<I>::basic_block_iterator source, typename jump<I>::basic_block_iterator target)
		{
			return outgoing_jumps_.insert(color,source,target);
		}
		void add_incoming_jump(typename outgoing_jumps_type::const_iterator& jmp)
		{
			incoming_jumps_.push_back(jmp);
		}
		/// @endcond

		/// container for jumps ending in this basic block
		class incoming_jumps_type
		{
		public:
			/// forward iterator over jumps ending in this basic block
			class const_iterator
			{
			public:
				/// iterator requirement: iterator_category
				typedef typename ::std::forward_iterator_tag iterator_category;
				/// iterator requirement: value_type
				typedef typename jumps<I>::const_iterator::value_type value_type;
				/// iterator requirement: difference_type
				typedef size_t difference_type;
				/// iterator requirement: const_pointer
				typedef const value_type* pointer;
				/// iterator requirement: const_reference
				typedef const value_type& reference;

				/// forward iterator requirement: operator*
				reference operator*() const
				{
					return **it_;
				}

				/// forward iterator requirement: operator->
				pointer operator->() const
				{
					return &**it_;
				}
				/// forward iterator requirement: ++operator
				const_iterator& operator++()
				{
					++it_;
					return *this;
				}
				/// forward iterator requirement: operator++
				const_iterator operator++(int)
				{
					const_iterator res(*this);
					++it_;
					return res;
				}
				/// forward iterator requirement: operator==
				bool operator==(const const_iterator& other) const
				{
					return it_==other.it_;
				}
				/// forward iterator requirement: operator!=
				bool operator!=(const const_iterator& other) const
				{
					return it_!=other.it_;
				}

				/// @cond INTERNAL
				const_iterator(typename incoming_jumps_internal_type::const_iterator it) : it_(it) {}
				/// @endcond
			private:
				typename incoming_jumps_internal_type::const_iterator it_;
			};

			/// container requirement: value_type
			typedef typename std::pair<uid_all_type::UID_type, jump<I> > value_type;
			/// container requirement: reference
			typedef value_type& reference;
			/// container requirement: const_reference
			typedef const value_type& const_reference;
			/// container requirement: size_type
			typedef typename incoming_jumps_internal_type::size_type size_type;
			/// container requirement: iterator
			typedef const_iterator iterator;
			/// container requirement: const_iterator
			typedef const_iterator const_iterator;

			/// first jump
			const_iterator begin() const
			{
				return const_iterator(data_->begin());
			}
			/// iterator to position one place behind last jump
			const_iterator end() const
			{
				return const_iterator(data_->end());
			}
			/// number of jumps in the container
			size_type size() const
			{
				return data_->size();
			}
			/// checks if the container contains no jumps
			bool empty() const
			{
				return data_->empty();
			}

			/// @cond INTERNAL
			incoming_jumps_type(const incoming_jumps_internal_type* data) : data_(data) {}
			/// @endcond
		private:
			const incoming_jumps_internal_type* data_;
		};

		/// get user data
		I& get_data() { return *user_data_; }
		/// get user data
		const I& get_data() const { return *user_data_; }
		/// get UID of the block
		uid_all_type::UID_type get_UID() const { return uid_; }
		/// get the UID specified in the contrustor of the block
		uid_all_type::UID_type get_other_UID() const { return other_uid_; }

		/// get jumps leading from the end of the block
		const outgoing_jumps_type& get_outgoing_jumps() const { return outgoing_jumps_; }
		/// get jumps ending at the beginning of the block
		incoming_jumps_type get_incoming_jumps() const { return incoming_jumps_type(&incoming_jumps_); }

		/// constructor
		/**
		\param other_uid UID to store in the basic block. It can be retreieved later by call to get_other_UID()
		**/
		basic_block(uid_all_type::UID_type other_uid)
			:
			user_data_(new I()),
			uid_(uid_all.get_UID()),
			other_uid_(other_uid)
			{
			}
		/// constructor
		basic_block()
			:
			user_data_(new I()),
			uid_(uid_all.get_UID()),
			other_uid_(uid_all_type::null_value)
			{
			}
	private:
		tr1::shared_ptr<I> user_data_;
		uid_all_type::UID_type uid_;
		uid_all_type::UID_type other_uid_;

		outgoing_jumps_type outgoing_jumps_;
		incoming_jumps_internal_type incoming_jumps_;
	};

	/// collection of basic blocks (nodes) interconected by colored jumps (edges)
	template<typename I>
	class basic_blocks
	{
		typedef std::list<basic_block<I> > basic_block_internal_storage;
		typedef std::map<uid_all_type::UID_type, typename basic_block_internal_storage::iterator> uid_to_block_map;
		typedef jump<I> jump_type;
		typedef jumps<I> jumps_type;
	public:
		/// container requirement: value_type
		typedef basic_block<I> value_type;
		/// container requirement: reference
		typedef typename basic_block_internal_storage::reference reference;
		/// container requirement: const_reference
		typedef typename basic_block_internal_storage::const_reference const_reference;
		/// container requirement: size_type
		typedef typename basic_block_internal_storage::size_type size_type;
		/// container requirement: iterator
		typedef typename basic_block_internal_storage::iterator iterator;
		/// container requirement: const_iterator
		typedef typename basic_block_internal_storage::const_iterator const_iterator;

		/// first basic block in the collection (with respect to order of create_block calls)
		iterator begin() { return blocks_.begin(); }
		/// position one step behind end of the collection
		iterator end() { return blocks_.end(); }
		/// first basic block in the collection (with respect to order of create_block calls)
		const_iterator begin() const { return blocks_.begin(); }
		/// position one step behind end of the collection
		const_iterator end() const { return blocks_.end(); }

		/// find basic block by UID
		iterator find_by_UID(uid_all_type::UID_type uid) { typename uid_to_block_map::iterator it = blocks_by_uid_.find(uid); return it != blocks_by_uid_.end() ? it->second : blocks_.end(); }
		/// find basic block by other UID (the UID that was specified as an atgument to create_block call)
		iterator find_by_other_UID(uid_all_type::UID_type uid) { typename uid_to_block_map::iterator it = blocks_by_other_uid_.find(uid); return it != blocks_by_other_uid_.end() ? it->second : blocks_.end(); }

		/// Create basic block. If no other basic block is present in the collection, the newly created block is set as the start block (entry point to the function/procedure)
		iterator create_block(uid_all_type::UID_type other_uid)
		{
			typename uid_to_block_map::iterator it=blocks_by_other_uid_.find(other_uid);
			if (it!=blocks_by_other_uid_.end()) return it->second;
			blocks_.push_back(basic_block<I>(other_uid));
			if (blocks_.size()==1) first_block_=blocks_.begin();
			blocks_by_uid_.insert(std::make_pair(blocks_.back().get_UID(),--blocks_.end()));
			blocks_by_other_uid_.insert(std::make_pair(blocks_.back().get_other_UID(),--blocks_.end()));
			return --blocks_.end();
		}
		/// Set start block (entry point to the function/procedure)
		void set_start_block(typename basic_block_internal_storage::iterator block)
		{
			first_block_=block;
		}
		/// Get start block (entry point to the function/procedure)
		iterator get_start_block()
		{
			return first_block_;
		}

		/// Add colored jump (edge) to the basic block graph
		void add_jump(uid_all_type::UID_type color,iterator source,iterator destination)
		{
			typename jumps_type::iterator j=source->add_outgoing_jump(color,source,destination).first;
			destination->add_incoming_jump(j);
		}

		/// forward iterator used to traverse the basic block collection in an order independent of the block creation order
		class canonical_iterator
		{
			void update()
			{
					if (next_.empty()) current_=parent_->end();
					else current_=(parent_->find_by_UID(next_.front()));
			}
		public:
			/// iterator requirement: iterator_category
			typedef std::forward_iterator_tag iterator_category;
			/// iterator requirement: value_type
			typedef basic_block<I> value_type;
			/// iterator requirement: difference_type
			typedef typename basic_block_internal_storage::iterator::difference_type difference_type;
			/// iterator requirement: pointer
			typedef basic_block<I>* pointer;
			/// iterator requirement: reference
			typedef basic_block<I>& reference;
					
			/// forward iterator requirement: operator*
			basic_block<I>& operator* () { return *current_; }
			/// forward iterator requirement: operator->
			basic_block<I>* operator-> () { return current_; }
			/// forward iterator requirement: ++operator
			canonical_iterator& operator++()
			{ 
				typename basic_blocks<I>::iterator block=parent_->find_by_UID(next_.front());
				done_.insert(next_.front());
				next_.pop_front();
				for(typename basic_block<I>::outgoing_jumps_type::const_iterator it=block->get_outgoing_jumps().begin();it!=block->get_outgoing_jumps().end();++it)
				{
						if (done_.find(it->second.get_target()->get_UID())==done_.end()) next_.push_back(it->second.get_target()->get_UID());
				}
				update();
				return *this;
			}
			/// forward iterator requirement: operator++
			canonical_iterator operator++(int)
			{
				canonical_iterator res(*this);
				++*this;
				update();
				return res;
			}
			/// @cond INTERNAL
			canonical_iterator(basic_blocks<I>* parent, uid_all_type::UID_type first)
				:next_(1,first),
				parent_(parent)
			{
				update();
			}
			canonical_iterator(basic_blocks<I>* parent, const typename uid_to_block_map::iterator& begin, const typename uid_to_block_map::iterator& end)
				:done_(util::first_of_pair_iterator<typename uid_to_block_map::iterator>(begin),util::first_of_pair_iterator<typename uid_to_block_map::iterator>(end)),
				parent_(parent)
			{
				update();
			}
			/// @endcond

			/// forward iterator requirement: operator==
			bool operator==(const canonical_iterator& other)
			{
				return current_==other.current_;
			}
			/// forward iterator requirement: operator!=
			bool operator!=(const canonical_iterator& other)
			{
				return !(*this==other);
			}
		private:
			std::set<uid_all_type::UID_type> done_;
			std::deque<uid_all_type::UID_type> next_;
			typename basic_blocks<I>::iterator current_;
			basic_blocks<I>* parent_;
		};

		/// get iterator of the first block in the canonical order (which is the function entry point)
		canonical_iterator canonical_begin()
		{
			if (first_block_==blocks_.end()) return canonical_end();
			return canonical_iterator(this,first_block_->get_UID());
		}
		/// get iterator to position one step behind the last basic block in the canonical order
		canonical_iterator canonical_end()
		{
			return canonical_iterator(this,blocks_by_uid_.begin(),blocks_by_uid_.end());
		}

/*		template <typename F>
		void canonical_foreach(F f)
		{
				std::set<uid_all_type::UID_type> done;
				std::deque<uid_all_type::UID_type> next;
				if (!blocks_.size()) return;
				next.push_back(blocks_.front().get_UID());
				while (next.size())
				{
						basic_block<I>& block=find_by_UID(next.front());
						done.insert(next.front());
						next.pop_front();

						f(block);

						for(jumps::jump_target_list::const_iterator it=block.get_jumps().get_targets().begin(); it!=block.get_jumps().get_targets().end();++it)
						{
								if (done.find(it->get_target_UID())==done.end()) next.push_back(it->get_target_UID());
						}
				}
		}*/

		/// constuctor
		basic_blocks():first_block_(blocks_.end()) {}
	private:
		basic_block_internal_storage blocks_;
		uid_to_block_map blocks_by_uid_;
		uid_to_block_map blocks_by_other_uid_;
		typename basic_block_internal_storage::iterator first_block_;
	};

	/// Jump color generator. It provides UIDs of colors of the basic block graph edges. The colors should be ordered by operator< in a specific orded, otherwise canonical ordering of the basic blocks would be impossible.
	class uid_provider
	{
		uid_all_type::UID_type unconditional_;
		uid_all_type::UID_type true_;
		uid_all_type::UID_type false_;
	public:
		/// color used for unconditional jumps (JMP)
		uid_all_type::UID_type unconditional() { return unconditional_; }
		/// color used for conditional jumps that should jump if the condition is true (JT)
		uid_all_type::UID_type jump_on_true() { return true_; }
		/// color used for conditional jumps that should jump if the condition is false (JF)
		uid_all_type::UID_type jump_on_false() { return false_; }
		/// Constructor. Generates new UIDs for each color. This means, that each instance of the uid_provider class returns different values 
		uid_provider() :unconditional_(uid_all.get_UID()), true_(uid_all.get_UID()), false_(uid_all.get_UID()) {}
		/// @cond INTERNAL
		const char* get_text(uid_all_type::UID_type uid)
		{
			if (uid==unconditional_) return "JMP";
			if (uid==true_) return "JT";
			if (uid==false_) return "JF";
			return "invalid";
		}
		/// @endcond
	};

}

#endif
