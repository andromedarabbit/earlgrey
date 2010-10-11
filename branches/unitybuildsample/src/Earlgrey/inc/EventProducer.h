
namespace Earlgrey {

	template<class _Fty>
	class EventProducer
	{
	protected:
		typedef std::tr1::function<_Fty> FunctorType;
		typedef std::map<DWORD_PTR, FunctorType> ListenerMapType;
	public:
		void AddListener(const void* key, const FunctorType& func)
		{
			m_map[reinterpret_cast<const DWORD_PTR>(key)] = func;
		}

		void RemoveListener(const void* key)
		{
			m_map.erase( reinterpret_cast<const DWORD_PTR>(key) );
		}

		void Fire()
		{
			ListenerMapType::const_iterator iter = m_map.begin();
			for (; iter != m_map.end(); iter++)
			{
				iter->second();
			}
		}

		template<typename T1>
		void Fire(T1 val1)
		{
			ListenerMapType::const_iterator iter = m_map.begin();
			for (; iter != m_map.end(); iter++)
			{
				iter->second( val1 );
			}
		}

		template<typename T1, typename T2>
		void Fire(T1 val1, T2 val2)
		{
			ListenerMapType::const_iterator iter = m_map.begin();
			for (; iter != m_map.end(); iter++)
			{
				iter->second( val1, val2 );
			}
		}

		template<typename T1, typename T2, typename T3>
		void Fire(T1 val1, T2 val2, T3 val3)
		{
			ListenerMapType::const_iterator iter = m_map.begin();
			for (; iter != m_map.end(); iter++)
			{
				iter->second( val1, val2, val3 );
			}
		}

		template<typename T1, typename T2, typename T3, typename T4>
		void Fire(T1 val1, T2 val2, T3 val3, T4 val4)
		{
			ListenerMapType::const_iterator iter = m_map.begin();
			for (; iter != m_map.end(); iter++)
			{
				iter->second( val1, val2, val3, val4 );
			}
		}

	private:
		ListenerMapType m_map; //! listener 를 저장하고 있는 맵 컨테이너
	};

}