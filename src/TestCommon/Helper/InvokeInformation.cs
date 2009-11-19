using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace TestCommon.Helper
{
    public class InvokeInformation
    {
        public InvokeInformation(object instance, object[] attributes, MethodInfo methodInfo)
        {
            this.Instance = instance;
            this.Attributes = attributes;
            this.Method = methodInfo;
        }

        public object Instance
        {
            get;
            private set;
        }

        public MethodInfo Method
        {
            get;
            private set;
        }

        public object[] Attributes
        {
            get;
            private set;
        }
    }
}
