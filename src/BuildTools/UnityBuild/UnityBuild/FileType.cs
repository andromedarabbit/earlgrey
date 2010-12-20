using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public partial class FileType 
    {
        //public bool ExcludedFromBuild
        //{
        //    get
        //    {
        //        return this.Items.Any(
        //            configurationType => ((BuildConfigurationType)configurationType).ExcludedFromBuild == true
        //            );
        //    }
        //}

        public bool ExcludedFromBuild(string buildConfigurationName)
        {
            BuildConfigurationType buildConfiguration = GetBuildConfiguration(buildConfigurationName);
            if (buildConfiguration == null)
                return false;

            return buildConfiguration.ExcludedFromBuild;
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
            ExcludeOrInclude(buildConfigurationName, true);
        }

        private void ExcludeOrInclude(string buildConfigurationName, bool exclude)
        {
            BuildConfigurationType buildConfiguration = GetBuildConfiguration(buildConfigurationName);
            if(buildConfiguration == null)
                buildConfiguration = AddBuildConfiguration(buildConfigurationName);
            buildConfiguration.ExcludedFromBuild = exclude;
            buildConfiguration.ExcludedFromBuildSpecified = true;

            this.ItemsSpecified = true;
        }

        private BuildConfigurationType GetBuildConfiguration(string buildConfigurationName)
        {
            var result = from item in this.Items
                         where item is BuildConfigurationType
                         && ((BuildConfigurationType)item).Name.Equals(buildConfigurationName, StringComparison.CurrentCultureIgnoreCase) == true
                         select (BuildConfigurationType)item;

            if (result.Count() > 0)
                return result.First();

            return null;
        }

        private BuildConfigurationType AddBuildConfiguration(string buildConfigurationName)
        {
            BuildConfigurationType buildConfiguration = new BuildConfigurationType();
            buildConfiguration.Name = buildConfigurationName;
            buildConfiguration.NameSpecified = true;
            this.Items.Add(buildConfiguration);

            ConfigurationTypeTool tool = new ConfigurationTypeTool();
            tool.Name = "VCCLCompilerTool";
            tool.NameSpecified = true;
            buildConfiguration.Tool.Add(tool);
            buildConfiguration.ToolSpecified = true;

            return buildConfiguration;
        }

        //public void ExcludeFromBuild()
        //{
        //    foreach (var item in this.Items)
        //    {
        //        Debug.Assert(item != null);
        //        Debug.Assert(item is BuildConfigurationType); // FileType 도 가능한데 그런 경우를 실제로 보지 못 했기에 방어적 차원에서...

        //        var configuration = (BuildConfigurationType)item;
        //        configuration.ExcludedFromBuild = true;
        //    }
        //} 

        public void IncludeInBuild(string buildConfigurationName)
        {
            ExcludeOrInclude(buildConfigurationName, false);
        }
    }
}
