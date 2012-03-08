using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public interface IFilterType : IFilterOrFile
    {
        List<object> Items
        {
            get;
        }
    }
}
