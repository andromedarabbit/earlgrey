using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public interface IProjectFileNameConverter
    {
        string GetNewName(string fileName);
        string GetOldName(string fileName);
        bool IsNewName(string fileName);
    }
}