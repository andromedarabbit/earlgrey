using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Components
{
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false, Inherited = false)]
    public class SetUpAttribute : Attribute
    {
    }
}
