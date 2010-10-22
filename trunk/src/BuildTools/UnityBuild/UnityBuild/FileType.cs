using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public partial class FileType 
    {
        public bool ExcludedFromBuild
        {
            get
            {
                return this.Items.Any(
                    configurationType => ((BuildConfigurationType)configurationType).ExcludedFromBuild == true
                    );
            }
        }

        public bool IsSrcFile
        {
            get
            {
                return Properties.UnityBuild.Default.IsSourceFile(this.RelativePath);
            }
        }

        public void ExcludeFromBuild(string buildConfigurationName)
        {
            var result = from item in this.Items
                                     where item is BuildConfigurationType 
                                     && ((BuildConfigurationType)item).Name.Equals(buildConfigurationName, StringComparison.CurrentCultureIgnoreCase) == true
                                     select (BuildConfigurationType)item;

            if(result.Count() == 0)
                throw new Exception("Not Found!");

            var buildConfiguration = result.First();
            buildConfiguration.ExcludedFromBuild = true;
        }

        public void ExcludeFromBuild()
        {
            foreach (var item in this.Items)
            {
                Debug.Assert(item != null);
                Debug.Assert(item is BuildConfigurationType); // FileType 도 가능한데 그런 경우를 실제로 보지 못 했기에 방어적 차원에서...

                var configuration = (BuildConfigurationType)item;
                //if(configuration.ExcludedFromBuild)
                //    continue;

                configuration.ExcludedFromBuild = true;
            }
        } 
    }
}
