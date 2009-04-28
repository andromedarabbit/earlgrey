#pragma once 
#include <string>
#include <memory>
#include <iostream>
#include <sstream>

#include <vector>
#include <map>
#include <set>
#include <queue>
#include <list>
#include <stack>

namespace Earlgrey
{

	//! \TODO 나중에 최적화된 메모리 할당자를 완성한 후 아래 코드를 고친다.
	//! \note C++은 템플릿 typedef를 지원하지 않아서 http://www.gotw.ca/gotw/079.htm 문서의 방법을 적용한다.
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
		//! \note 중간에 typename이 들어간 이유는 MSDN 라이브러리 중 "C4346"를 참고하자.
		typedef 
			std::vector<T, typename NewAllocator<T>::Type  > 
			Type; 
	};

	// <map>
	template<typename Key, typename T, typename Compare = std::less<Key> >
	struct xmap
	{ 
		typedef 
			std::map<Key, T, Compare,  typename NewAllocator< std::pair<Key, T> >::Type  > 
			Type; 
	};

	template<typename Key, typename T, typename Compare = std::less<Key> >
	struct xmultimap
	{
		typedef 
			std::multimap<Key, T, Compare,  typename NewAllocator< std::pair<Key, T> >::Type > 
			Type; 
	};

	// <set>
	template<typename Key, typename Compare = std::less<Key> >
	struct xset
	{
		typedef 
			std::set<Key, Compare, typename NewAllocator< Key >::Type > 
			Type; 
	};


	template<typename Key, typename Compare = std::less<Key> >
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
		std::basic_string<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwstring
		;

	typedef 
		std::basic_string<char, std::char_traits<char>, NewAllocator<char>::Type >
		xstring
		;

#ifdef _UNICODE
typedef xwstring _txstring;
#else
typedef	xstring _txstring;
#endif

	// <sstream>
	typedef 
		std::basic_stringbuf<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwstringbuf
		;

	typedef 
		std::basic_stringbuf<char, std::char_traits<char>, NewAllocator<char>::Type > 
		xstringbuf
		;

#ifdef _UNICODE
typedef xwstringbuf _txstringbuf;
#else
typedef	xstringbuf _txstringbuf;
#endif

	typedef 
		std::basic_istringstream<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwistringstream
		;

	typedef 
		std::basic_istringstream<char, std::char_traits<char>, NewAllocator<char>::Type > 
		xistringstream
		;

#ifdef _UNICODE
typedef xwistringstream _txistringstream;
#else
typedef xistringstream _txistringstream;
#endif

	typedef 
		std::basic_ostringstream<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwostringstream;

	typedef 
		std::basic_ostringstream<char, std::char_traits<char>, NewAllocator<char>::Type > 
		xostringstream
		;

#ifdef _UNICODE
typedef xwostringstream _txostringstream;
#else
typedef	xostringstream _txostringstream;
#endif


	typedef 
		std::basic_stringstream<wchar_t, std::char_traits<wchar_t>, NewAllocator<wchar_t>::Type > 
		xwstringstream
		;

	typedef 
		std::basic_stringstream<char, std::char_traits<char>, NewAllocator<char>::Type > 
		xstringstream
		;

#ifdef _UNICODE
typedef xwstringstream _txstringstream;
#else
typedef	xstringstream _txstringstream;
#endif


	// <iostream>
#ifdef _UNICODE
typedef std::wios _tios;
#else
typedef	std::ios _tios;
#endif

#ifdef _UNICODE
typedef std::wstreambuf _tstreambuf;
#else
typedef	std::streambuf _tstreambuf;
#endif

#ifdef _UNICODE
typedef std::wistream _tistream;
#else
typedef	std::istream _tistream;
#endif

#ifdef _UNICODE
typedef std::wostream _tostream;
#else
typedef	std::ostream _tostream;
#endif

#ifdef _UNICODE
typedef std::wiostream _tiostream;
#else
typedef	std::iostream _tiostream;
#endif

}