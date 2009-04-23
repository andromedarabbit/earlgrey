#pragma once 
#include <string>
#include <memory>
#include <iostream>

#include <vector>
#include <map>
#include <set>
#include <queue>
#include <list>
#include <stack>

namespace Earlgrey
{

	//! \TODO ���߿� ����ȭ�� �޸� �Ҵ��ڸ� �ϼ��� �� �Ʒ� �ڵ带 ��ģ��.
	//! \note C++�� ���ø� typedef�� �������� �ʾƼ� http://www.gotw.ca/gotw/079.htm ������ ����� �����Ѵ�.
	template<typename T>
	struct NewAllocator
	{
		typedef 
			std::allocator<T> 
			Type;
	};

	// <vector>
	template<typename T>
	struct xvector
	{
		//! \note �߰��� typename�� �� ������ MSDN ���̺귯�� �� "C4346"�� ��������.
		typedef 
			std::vector<T, typename NewAllocator<T>::Type  > 
			Type; 
	};

	// <map>
	template<typename Key, typename T, typename Compare = less<Key> >
	struct xmap
	{ 
		typedef 
			std::map<Key, T, Compare,  typename NewAllocator< std::pair<Key, T> >::Type  > 
			Type; 
	};

	template<typename Key, typename T, typename Compare = less<Key> >
	struct xmultimap
	{
		typedef 
			std::multimap<Key, T, Compare,  typename NewAllocator< std::pair<Key, T> >::Type > 
			Type; 
	};

	// <set>
	template<typename Key, typename Compare = less<Key> >
	struct xset
	{
		typedef 
			std::set<Key, Compare, typename NewAllocator< Key >::Type > 
			Type; 
	};


	template<typename Key, typename Compare = less<Key> >
	struct xmultiset
	{
		typedef 
			std::multiset<Key, Compare, typename NewAllocator< Key >::Type > 
			Type; 
	};

	// <queue>
	template<typename T>
	struct xdeque
	{
		typedef 
			std::deque<T, typename NewAllocator< T >::Type> 
			Type; 
	};

	template<typename T, typename Container = xdeque<T>::Type >
	struct xqueue
	{
		typedef 
			std::queue<T, Container> 
			Type; 
	};

	template<typename T, typename Container = xdeque<T>::Type >
	struct xpriority_queue
	{
		typedef 
			std::priority_queue<T, Container> 
			Type; 
	};


	// <list>
	template<typename T, typename Allocator = NewAllocator< T >::Type >
	struct xlist
	{
		typedef 
			std::list<T, Allocator> 
			Type; 
	};

	// <stack>
	template<typename T, typename Container = xdeque<T>::Type >
	struct xstack
	{
		typedef 
			std::stack<T, Container> 
			Type; 
	};


	// <string>
	typedef 
		std::basic_string<char, std::char_traits<char>, NewAllocator<char>::Type >
		xstring
		;

	typedef 
		std::basic_string<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwstring
		;

	// <iostream>
	typedef 
		std::basic_stringbuf<char, std::char_traits<char>, NewAllocator<char>::Type > 
		xstringbuf
		;

	typedef 
		std::basic_istringstream<char, std::char_traits<char>, NewAllocator<char>::Type > 
		xistringstream
		;

	typedef 
		std::basic_ostringstream<char, std::char_traits<char>, NewAllocator<char>::Type > 
		xostringstream
		;

	typedef 
		std::basic_stringstream<char, std::char_traits<char>, NewAllocator<char>::Type > 
		xstringstream
		;
	
	typedef 
		std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwstringbuf
		;
	
	typedef 
		std::basic_istringstream<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwistringstream
		;

	typedef 
		std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwostringstream
		;

	typedef 
		std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwstringstream
		;

}