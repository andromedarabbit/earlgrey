using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal static class CustomExtensions
    {
        public static bool IsSrcFile(this FileType file)
        {
            return Properties.UnityBuild.Default.IsSourceFile(file.RelativePath);
        }
    }
}
