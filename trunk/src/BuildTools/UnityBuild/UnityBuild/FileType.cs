using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public partial class FileType : IFilterOrFile
    {
        //public bool ExcludedFromBuildInSomeCase
        //{
        //    get
        //    {
        //        return this.Items.Any(
        //            configurationType => ((BuildConfigurationType)configurationType).ExcludedFromBuild == true
        //            );
        //    }
        //}

        public IEnumerable<string> BuildConfigurationsWhenExcludedFromBuild
        {
            get
            {
                var result = from configuration in this.Items
                             where ((BuildConfigurationType) configuration).ExcludedFromBuild == true
                             select ((BuildConfigurationType) configuration).Name
                    ;

                return result;
            }
        }


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

        public bool CreatePrecompiledHeader(string configurationBuild)
        {
            IEnumerable<BuildConfigurationType> buildConfigurations = from item in this.Items
                         where item is BuildConfigurationType
                         && ((BuildConfigurationType) item).Name.Equals(configurationBuild, StringComparison.CurrentCultureIgnoreCase) == true
                         select (BuildConfigurationType) item
                ;

            if(buildConfigurations.Count() == 0)
                return false;

            
            foreach(BuildConfigurationType buildConfiguration in buildConfigurations)
            {
                if(buildConfiguration.Tool == null || buildConfiguration.Tool.Count == 0)
                    continue;
                
                foreach(ConfigurationTypeTool tool in buildConfiguration.Tool)
                {
                    if(tool.AnyAttr == null || tool.AnyAttr.Count == 0)
                        continue;

                    int indexFound = tool.AnyAttr.FindIndex(
                        xmlAttribute => xmlAttribute.Name == "UsePrecompiledHeader"
                        );

                    if(indexFound < 0)
                        continue;

                    return tool.AnyAttr[indexFound].Value == "1";
                }
            }

            return false;
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
