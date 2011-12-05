using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public enum UsePrecompiledHeaderOptions
    {
        None = 0
        ,
        Create = 1
        ,
        Use = 2
        ,
        InheritFromProject = int.MaxValue // 명시 안 된 경우
    }
}