using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCommon.Net
{
    [AttributeUsage(AttributeTargets.Method, AllowMultiple = false, Inherited = false)]
    public class ProtocolAttribute : Attribute
    {
        public ProtocolAttribute(uint ID)
        {
            this.ID = ID;
        }

        public uint ID
        {
            get;
            private set;
        }
    }
}
