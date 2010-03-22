#pragma 

namespace Earlgrey
{
// 	namespace Test
// 	{
		class Mock 
		{
		public:
			explicit Mock()
				: m_index(0)
			{

			}

			explicit Mock(int index)
				: m_index(index)
			{

			}

			inline bool operator == (const Mock& obj) const
			{
				return m_index == obj.m_index;
			}


			// friend std::ostream& << (const Mock& obj);

			int m_index;
		};

		std::ostream& operator << (std::ostream& stream, const Mock& obj)
		{
			stream << obj.m_index;
			return stream;
		}

		namespace Serialization
		{
			template <typename W, typename T>
			BOOL Write(W& writer, const T& obj);

			template <typename W>
			inline BOOL Write(W& writer, const Mock& obj)
			{
				return writer.Write(obj.m_index);
			}

			template <typename R, typename T>
			BOOL Read(R& reader, T& obj);

			template <typename R>
			inline BOOL Read(R& reader, Mock& obj)
			{
				return reader.Read(obj.m_index);
			}

// 			template <typename W>
// 			inline BOOL Write(W& writer, const Mock* obj)
// 			{
// 				return writer.Write(obj->m_index);
// 			}
		}
		
	//}
}