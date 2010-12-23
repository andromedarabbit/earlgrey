using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public partial class FileType : IFilterOrFile
    {
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

        internal PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatform)
        {
            if(IsSrcFile == false)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);

            BuildConfigurationType buildConfiguration = GetBuildConfiguration(configurationPlatform);
            if (buildConfiguration == null)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.InheritFromProject);

            IEnumerable<ConfigurationTypeTool> tools = buildConfiguration.Tool.Where(item => item.Name == "VCCLCompilerTool");
            if(tools.Count() == 0)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.InheritFromProject);

            Debug.Assert(tools.Count() == 1);

            ConfigurationTypeTool tool = tools.First();

            return PrecompiledHeaderOptions.CreateInstance(tool);            
        }

        internal
            void SetPrecompiledHeaderOption(string configurationPlatform, PrecompiledHeaderOptions options)
        {
            if (IsSrcFile == false)
                throw new ApplicationException();


            BuildConfigurationType buildConfiguration = GetBuildConfiguration(configurationPlatform);
            if (buildConfiguration == null)
                buildConfiguration = AddBuildConfiguration(configurationPlatform);

            IEnumerable<ConfigurationTypeTool> tools = buildConfiguration.Tool.Where(item => item.Name == "VCCLCompilerTool");
            ConfigurationTypeTool vcclcCompiler = null;
            if (tools.Count() > 0)
            {
                Debug.Assert(tools.Count() == 1);
                vcclcCompiler = tools.First();
            }

            if (vcclcCompiler == null)
            {
                vcclcCompiler = new ConfigurationTypeTool();
                vcclcCompiler.Name = "VCCLCompilerTool";
                vcclcCompiler.NameSpecified = true;
                buildConfiguration.Tool.Add(vcclcCompiler);
            }

            vcclcCompiler.UsePrecompiledHeader = ((int)options.UsePrecompiledHeader).ToString();

            if (string.IsNullOrEmpty(options.PrecompiledHeaderFile) == false)
            {
                vcclcCompiler.PrecompiledHeaderFile = options.PrecompiledHeaderFile;
                vcclcCompiler.PrecompiledHeaderFileSpecified = true;
            }

            if (string.IsNullOrEmpty(options.PrecompiledHeaderThrough) == false)
            {
                vcclcCompiler.PrecompiledHeaderThrough = options.PrecompiledHeaderThrough;
                vcclcCompiler.PrecompiledHeaderThroughSpecified = true;
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

        public void IncludeInBuild(string buildConfigurationName)
        {
            ExcludeOrInclude(buildConfigurationName, false);
        }

        public string FileName
        {
            get { return Path.GetFileName(this.RelativePath); }
        }


   
    }
}
