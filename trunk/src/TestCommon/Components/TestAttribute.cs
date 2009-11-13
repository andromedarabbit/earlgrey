using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Components
{
    public enum TestType
    {
        AutoRun,        // void Test()
        Interactive     // bool Test(string)
    }

    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false, Inherited = false)]
    public class TestAttribute : Attribute
    {
        public TestAttribute(TestType testType)
        {
            this.TestType = testType;
        }

        public TestType TestType
        {
            get;
            private set;
        }
    }
}
