using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public partial class FileType : IFileType
    {
        private Dictionary<string, PrecompiledHeaderOptions> _precompiledHeaderOptions;

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
            get { return Properties.UnityBuild.Default.IsSourceFile(this.RelativePath); }
        }

        public 
            PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatform)
        {
            if (IsSrcFile == false)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);

            BuildConfigurationType buildConfiguration = GetBuildConfiguration(configurationPlatform);
            if (buildConfiguration == null)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.InheritFromProject);

            IEnumerable<ConfigurationTypeTool> tools =
                buildConfiguration.Tool.Where(item => item.Name == "VCCLCompilerTool");
            if (tools.Count() == 0)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.InheritFromProject);

            Debug.Assert(tools.Count() == 1);

            ConfigurationTypeTool tool = tools.First();

            return PrecompiledHeaderOptions.CreateInstance(tool);
        }

        public
            void SetPrecompiledHeaderOption(string configurationPlatform, PrecompiledHeaderOptions options)
        {
            if (IsSrcFile == false)
                throw new ApplicationException();


            BuildConfigurationType buildConfiguration = GetBuildConfiguration(configurationPlatform);
            if (buildConfiguration == null)
                buildConfiguration = AddBuildConfiguration(configurationPlatform);

            IEnumerable<ConfigurationTypeTool> tools =
                buildConfiguration.Tool.Where(item => item.Name == "VCCLCompilerTool");
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

            vcclcCompiler.UsePrecompiledHeader = ((int) options.UsePrecompiledHeader).ToString();

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

            InternalPrecompiledHeaderOptions.Add(configurationPlatform, options);
        }

        public void ExcludeFromBuild(string buildConfigurationName)
        {
            ExcludeOrInclude(buildConfigurationName, true);
        }

        private void ExcludeOrInclude(string buildConfigurationName, bool exclude)
        {
            BuildConfigurationType buildConfiguration = GetBuildConfiguration(buildConfigurationName);
            if (buildConfiguration == null)
                buildConfiguration = AddBuildConfiguration(buildConfigurationName);
            buildConfiguration.ExcludedFromBuild = exclude;
            buildConfiguration.ExcludedFromBuildSpecified = true;

            this.ItemsSpecified = true;
        }

        private BuildConfigurationType GetBuildConfiguration(string buildConfigurationName)
        {
            var result = from item in this.Items
                         where item is BuildConfigurationType
                               &&
                               ((BuildConfigurationType) item).Name.Equals(buildConfigurationName,
                                                                           StringComparison.CurrentCultureIgnoreCase) ==
                               true
                         select (BuildConfigurationType) item;

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

        public string RelativeDir
        {
            get { return Path.GetDirectoryName(this.RelativePath); }
        }

        private Dictionary<string, PrecompiledHeaderOptions> InternalPrecompiledHeaderOptions
        {
            get
            {
                if (_precompiledHeaderOptions == null)
                    _precompiledHeaderOptions = new Dictionary<string, PrecompiledHeaderOptions>();
                return _precompiledHeaderOptions;
            }
        }

        public void AddPrecompiledHeaderOptions(string configurationPlatform, PrecompiledHeaderOptions options)
        {
            InternalPrecompiledHeaderOptions[configurationPlatform.ToUpper()] = options;
        }

        public PrecompiledHeaderOptions GetPrecompiledHeaderOptions(string configurationPlatform)
        {
            return InternalPrecompiledHeaderOptions[configurationPlatform];
        }

        public IDictionary<string, PrecompiledHeaderOptions> GetPrecompiledHeaderOptions()
        {
            return InternalPrecompiledHeaderOptions;
        }

        #region IFilterOrFile member impletations

        public bool IsFilter
        {
            get { return false; }
        }

        public bool IsFile
        {
            get { return true; }
        }

        #endregion
    }
}