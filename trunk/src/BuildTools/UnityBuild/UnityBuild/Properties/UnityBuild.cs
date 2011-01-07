using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild.Properties 
{
    internal sealed partial class UnityBuild //  : global::System.Configuration.ApplicationSettingsBase
    {
        public bool IsSourceFile(string filePath)
        {
            string filePathExtension = Path.GetExtension(filePath).TrimStart('.');
            if (string.IsNullOrEmpty(filePathExtension))
                return false;

            foreach(var extension in SourceFileExtensions)
            {
                // if(filePath.EndsWith(extension, StringComparison.OrdinalIgnoreCase))
                if(filePathExtension.Equals(extension, StringComparison.CurrentCultureIgnoreCase))
                    return true;
            }
            return false;
        }
    }
}
