#pragma once 
#include "Uncopyable.h"
#include "EarlgreyAssert.h"

namespace Earlgrey
{
	template < typename buffer_type >
	class basic_buffer_iterator
		: public std::iterator<std::random_access_iterator_tag,
		typename buffer_type::value_type,
		typename buffer_type::size_type,
		typename buffer_type::pointer,
		typename buffer_type::reference>
	{
	public:
		typedef typename buffer_type::iterator self_type;
		typedef typename buffer_type::size_type size_type;


		basic_buffer_iterator(buffer_type &parent, size_type index)
			: parent(parent), index(index) 
		{
		}

		basic_buffer_iterator& operator = (const basic_buffer_iterator& obj)			
		{
			parent = obj.parent;
			index = obj.index;
		}

		self_type &operator++()
		{
			++index;
			return *this;
		}
		self_type operator++(int) // postincrement
		{
			self_type old(*this);
			operator++();
			return old;
		}
		self_type &operator--()
		{
			--index;
			return *this;
		}
		self_type operator--(int) // postdecrement
		{
			self_type old(*this);
			operator--();
			return old;
		}

		reference operator*() { return parent[index]; }
		pointer operator->()  { return &(parent[index]); }

		bool operator==(const self_type &other) const
		{ return &parent == &other.parent && index == other.index; }
		bool operator!=(const self_type &other) const
		{ return !(other == *this); }

	private:
		buffer_type &parent;
		size_type    index;
	};

}

