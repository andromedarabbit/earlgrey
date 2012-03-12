using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class FileTypeFactory
    {
        internal static IFileType CreateInstance(VisualStudioVersions version, string relativePath)
        {
            if (version == VisualStudioVersions.V2008)
            {
                return new UnityBuild.VS2008.FileType(relativePath);
            }

            throw new ArgumentException(
                string.Format("Unknown version of Visual Studio: {0}", version)
                );
        }
    }
}
