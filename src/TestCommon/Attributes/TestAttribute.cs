using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple=false, Inherited=false)]
    public class TestAttribute : Attribute
    {
        public string Prompt
        {
            get;
            private set;
        }

        public string Description
        {
            get;
            private set;
        }

        [AttributeUsage(AttributeTargets.Method, AllowMultiple=false, Inherited=false)]
        public class ExecuteAttribute : Attribute
        {

        }
    }
}
