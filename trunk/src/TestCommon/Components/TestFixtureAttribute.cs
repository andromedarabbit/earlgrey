using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Components
{
    [AttributeUsage(AttributeTargets.Class, AllowMultiple=false, Inherited=false)]
    public class TestFixtureAttribute : Attribute
    {
        public TestFixtureAttribute(string prompt, string description)
        {
            Prompt = prompt;
            Description = description;
        }

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
    }
}
