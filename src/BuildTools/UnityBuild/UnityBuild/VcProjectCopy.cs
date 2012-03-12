using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;


namespace UnityBuild
{
    using VS2008;

    internal class VcProjectCopy
    {
        private readonly VcProject _vcProject;
        private readonly string _srcConfigurationPlatform;
        private readonly string _dstConfigurationPlatform;

        private readonly List<string> _additionalPreprocessorDefinitions;

        public VcProjectCopy(VcProject vcProject, string srcConfigurationPlatform, string dstConfigurationPlatform)
        {
            Debug.Assert(vcProject != null);
            Debug.Assert(string.IsNullOrEmpty(srcConfigurationPlatform) == false);
            Debug.Assert(string.IsNullOrEmpty(dstConfigurationPlatform) == false);

            _vcProject = vcProject;
            _srcConfigurationPlatform = srcConfigurationPlatform;
            _dstConfigurationPlatform = dstConfigurationPlatform;

            _additionalPreprocessorDefinitions = new List<string>();
        }

        public void AddPreprocessorDefinition(string definition)
        {
            _additionalPreprocessorDefinitions.Add(definition);
        }

        public void CopyConfigurationPlatform()
        {
            IConfigurationType configuration = _vcProject.GetConfiguration(_srcConfigurationPlatform);
            if (configuration == null)
                throw new ArgumentException();

            IConfigurationType dstConfiguration = _vcProject.GetConfiguration(_dstConfigurationPlatform);
            if (dstConfiguration != null)
                throw new ArgumentException();

            // 공통 속성
            IConfigurationType newConfiguration = configuration.Copy();
            newConfiguration.Name = _dstConfigurationPlatform;

            if (_additionalPreprocessorDefinitions.Count > 0)
                AddPreprocessorDefinitions(newConfiguration as ConfigurationType); // VS2008 전용 코드


            _vcProject.AddConfiguration(newConfiguration);

            // 개별 파일 속성
            CopyConfigurationPlatformInFiles(_vcProject.Files);
        }

        private void AddPreprocessorDefinitions(ConfigurationType newConfiguration)
        {
            ConfigurationTypeTool vcclCompiler = GetVCCLCompiler(newConfiguration);
            if (vcclCompiler == null)
            {
                vcclCompiler = new ConfigurationTypeTool();
                vcclCompiler.Name = "VCCLCompilerTool";
                vcclCompiler.NameSpecified = true;
            }
            vcclCompiler.AddPreprocessorDefinitions(_additionalPreprocessorDefinitions);
        }

        private static ConfigurationTypeTool GetVCCLCompiler(ConfigurationType configuration)
        {
            var result = configuration.Tool.Where(item => item.Name == "VCCLCompilerTool");
            if (result.Count() == 0)
                return null;

            Debug.Assert(result.Count() == 1);

            return result.First();
        }

        private void CopyConfigurationPlatformInFiles(IEnumerable<object> items)
        {
            foreach (object item in items)
            {
                Debug.Assert((item is IFileType) || (item is IFilterType));
                if (item is IFileType)
                {
                    IFileType file = (IFileType)item;
                    CopyConfigurationPlatformoInFileBuildConfiguration(file.Items);
                }

                if (item is IFilterType)
                {
                    IFilterType filter = (IFilterType)item;
                    CopyConfigurationPlatformInFiles(filter.Items);
                }
            }
        }

        private void CopyConfigurationPlatformoInFileBuildConfiguration(List<object> fileBuildConfigurations)
        {
            // TODO: 이름이 같은 구성 값은 하나 뿐이어야 할 것 같지만 안전하게 List 로 관리한다. 일단은....
            List<object> newBuildConfigurations = new List<object>();

            foreach (var item in fileBuildConfigurations)
            {
                Debug.Assert(item is IBuildConfigurationType);

                IBuildConfigurationType buildConfiguration = (IBuildConfigurationType)(item);
                if (
                    buildConfiguration.Name.Equals(_srcConfigurationPlatform, StringComparison.CurrentCultureIgnoreCase) ==
                    false)
                    continue;

                IBuildConfigurationType newBuildConfiguration = (IBuildConfigurationType)buildConfiguration.Copy();
                newBuildConfiguration.Name = _dstConfigurationPlatform;
                newBuildConfigurations.Add(newBuildConfiguration);
            }


            fileBuildConfigurations.AddRange(newBuildConfigurations);
        }
    }
}