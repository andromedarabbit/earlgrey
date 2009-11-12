using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace TestCommon.Net
{
    public class ProtocolReference
    {
        public ProtocolReference(object Instance, MethodInfo Method)
        {
            this.Instance = Instance;
            this.Method = Method;
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
    }
}
