using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Xml;

namespace UnityBuild
{
    internal static class CustomExtensions
    {
        public static bool IsSrcFile(this FileType file)
        {
            return Properties.UnityBuild.Default.IsSourceFile(file.RelativePath);
        }

        // todo: 어떻게 단위테스트를 할까?
        public static bool IsExcludedFromBuild(this FileType file)
        {
            return file.Items.Any(
                configurationType => ((ConfigurationType)configurationType).AnyAttr.Any(attr => attr.Name == "ExcludedFromBuild" && attr.Value == "true")
                );
        }

        /*
        public static void ExcludeFromBuild(this FileType file)
        {
            foreach(var item in file.Items)
            {
                Debug.Assert(item != null);
                Debug.Assert(item is ConfigurationType);

                var configuration = (ConfigurationType) item;
                XmlAttribute attr = configuration.AnyAttr.First(attr => attr.Name == "ExcludedFromBuild" && attr.Value == "true");
                
                if(attr != null)
                    continue;

                // ((XmlAttributeCollection)configuration.AnyAttr).
            }
        } 
         * */
    }
}
