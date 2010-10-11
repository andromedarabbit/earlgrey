using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Text;

namespace UnityBuild.Properties 
{
    internal sealed partial class UnityBuild //  : global::System.Configuration.ApplicationSettingsBase
    {
        public bool IsSourceFile(string filePath)
        {
            foreach(var extension in SourceFileExtensions)
            {
                if(filePath.EndsWith(extension, StringComparison.OrdinalIgnoreCase))
                    return true;
            }
            return false;
        }
    }
}
