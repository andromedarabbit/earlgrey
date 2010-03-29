#include "stdafx.h"
#include "DateTime.h"
#include "DateTimeKind.h"
#include "TimeSpan.h"

namespace Earlgrey {
	namespace ADO {
		namespace Test {

			TEST(TimeSpanTest, MySql)
			{
				DateTime datetime(1000, 1, 1, 0, 0, 0, 0, DATETIMEKIND_UNSPECIFIED);
				const INT64 ticks = datetime.Ticks();
				ASSERT_GT(ticks, 0);
			}

			TEST(TimeSpanTest, MSSql)
			{
				DateTime datetime(1753, 1, 1, 0, 0, 0, 0, DATETIMEKIND_UNSPECIFIED);
				const INT64 ticks = datetime.Ticks();
				ASSERT_GT(ticks, 0);
			}
	
		}
	}
}