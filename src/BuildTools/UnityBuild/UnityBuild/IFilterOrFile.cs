using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public interface IFilterOrFile
    {
        bool IsFilter { get; }
        bool IsFile { get; }
    }
}
