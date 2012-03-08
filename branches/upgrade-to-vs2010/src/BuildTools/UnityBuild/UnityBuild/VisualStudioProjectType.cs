using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public partial class VisualStudioProjectType
    {
        [System.Xml.Serialization.XmlIgnore]
        public IEnumerable<string> ConfigurationPlatformNames
        {
            get { return this.Configurations.Select(item => item.Name); }
        }

        internal ConfigurationType GetConfiguration(string srcName)
        {
            return this.Configurations.Find(
                item => item.Name.Equals(srcName, StringComparison.CurrentCultureIgnoreCase)
                );
        }

        public bool HasConfigurationPlatform(string configurationPlatformName)
        {
            return
                this.Configurations.Count(
                    item => item.Name.Equals(configurationPlatformName, StringComparison.CurrentCultureIgnoreCase)
                    ) > 0;
        }


        internal
            void SetPrecompiledHeaderOption(string configurationPlatform, PrecompiledHeaderOptions options,
                                            FileType file)
        {
            if (file.IsSrcFile == false)
                throw new ApplicationException();

            PrecompiledHeaderOptions projectOptions = GetPrecompiledHeaderOption(configurationPlatform);
            if (options == projectOptions)
            {
                file.SetPrecompiledHeaderOption(
                    configurationPlatform, new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.InheritFromProject)
                    );
                return;
            }


            PrecompiledHeaderOptions newOptions =
                new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.InheritFromProject);
            if (options.UsePrecompiledHeader != projectOptions.UsePrecompiledHeader)
            {
                newOptions.UsePrecompiledHeader = options.UsePrecompiledHeader;
            }

            if (
                options.PrecompiledHeaderFile.Equals(projectOptions.PrecompiledHeaderFile,
                                                     StringComparison.CurrentCultureIgnoreCase) == false)
            {
                newOptions.PrecompiledHeaderFile = options.PrecompiledHeaderFile;
            }

            if (
                options.PrecompiledHeaderThrough.Equals(projectOptions.PrecompiledHeaderThrough,
                                                        StringComparison.CurrentCultureIgnoreCase) == false)
            {
                newOptions.PrecompiledHeaderThrough = options.PrecompiledHeaderThrough;
            }

            file.SetPrecompiledHeaderOption(
                configurationPlatform, newOptions
                );
        }


        internal PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatform)
        {
            ConfigurationType configuration = GetConfiguration(configurationPlatform);
            if (configuration == null)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);

            IEnumerable<ConfigurationTypeTool> tools = from item in configuration.Tool
                                                       where item.Name == "VCCLCompilerTool"
                                                       select item;

            if (tools.Count() == 0)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);

            Debug.Assert(tools.Count() == 1);

            ConfigurationTypeTool tool = tools.First();

            PrecompiledHeaderOptions options = PrecompiledHeaderOptions.CreateInstance(tool);
            if (options.UsePrecompiledHeader != UsePrecompiledHeaderOptions.InheritFromProject)
            {
                if (string.IsNullOrEmpty(options.PrecompiledHeaderFile))
                    options.PrecompiledHeaderFile = @"$(IntDir)\$(TargetName).pch";

                if (string.IsNullOrEmpty(options.PrecompiledHeaderThrough))
                    options.PrecompiledHeaderThrough = "StdAfx.h";

                return options;
            }

            return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);
        }

        internal PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatform, IFileType file)
        {
            if (file.IsSrcFile == false)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);

            PrecompiledHeaderOptions fileOption = file.GetPrecompiledHeaderOption(configurationPlatform);
            if (fileOption.UsePrecompiledHeader == UsePrecompiledHeaderOptions.None)
            {
                return fileOption;
            }

            PrecompiledHeaderOptions projectOptions = GetPrecompiledHeaderOption(configurationPlatform);

            if (fileOption.UsePrecompiledHeader == UsePrecompiledHeaderOptions.InheritFromProject)
                fileOption.UsePrecompiledHeader = projectOptions.UsePrecompiledHeader;

            if (string.IsNullOrEmpty(fileOption.PrecompiledHeaderFile))
                fileOption.PrecompiledHeaderFile = projectOptions.PrecompiledHeaderFile;
            if (string.IsNullOrEmpty(fileOption.PrecompiledHeaderThrough))
                fileOption.PrecompiledHeaderThrough = projectOptions.PrecompiledHeaderThrough;

            return fileOption;
        }

        // 이름은 Headers 인데 실제론 /Yc 옵션이 적용된 .cpp 파일을 반환한다.
        public IEnumerable<IFileType> GetPrecompiledHeaders(string configurationPlatform)
        {
            Debug.Assert(string.IsNullOrEmpty(configurationPlatform) == false);

            List<IFileType> filesFound = new List<IFileType>();
            GetPrecompiledHeaders(configurationPlatform, this.Files, filesFound);
            return filesFound;
        }

        private void GetPrecompiledHeaders(string configurationPlatform, IEnumerable<object> items,
                                           List<IFileType> filesFound)
        {
            Debug.Assert(filesFound != null);

            if (items.Count() == 0)
                return;

            foreach (object item in items)
            {
                Debug.Assert((item is IFileType) || (item is FilterType));
                if (item is IFileType)
                {
                    IFileType file = (IFileType)item;
                    PrecompiledHeaderOptions options = GetPrecompiledHeaderOption(configurationPlatform, file);
                    if (options.UsePrecompiledHeader != UsePrecompiledHeaderOptions.Create)
                        continue;

                    filesFound.Add(file);
                }

                if (item is FilterType)
                {
                    FilterType filter = (FilterType) item;
                    GetPrecompiledHeaders(configurationPlatform, filter.Items, filesFound);
                }
            }
        }

        public void Initialize()
        {
            ExtendFiles(this.Files);
        }

        private void ExtendFiles(List<object> srcFiles)
        {
            if (srcFiles == null)
                return;

            foreach (object item in srcFiles)
            {
                Debug.Assert(item is IFileType || item is FilterType);

                if (item is FilterType)
                {
                    FilterType filter = item as FilterType;
                    ExtendFiles(filter.Items);
                    continue;
                }

                IFileType file = (IFileType)item;

                foreach (string configurationPlatform in ConfigurationPlatformNames)
                {
                    PrecompiledHeaderOptions precompiledHeaderOptions =
                        this.GetPrecompiledHeaderOption(configurationPlatform, file);
                    file.AddPrecompiledHeaderOptions(configurationPlatform, precompiledHeaderOptions);
                }
            }
        }


        private static void ExcludeFromBuild(string configurationPlatformName, IEnumerable<object> items)
        {
            foreach (object item in items)
            {
                Debug.Assert((item is IFileType) || (item is FilterType));
                if (item is IFileType)
                {
                    IFileType file = (IFileType)item;
                    if (file.IsSrcFile == false)
                        continue;

                    PrecompiledHeaderOptions options = file.GetPrecompiledHeaderOption(configurationPlatformName);
                    if (options.UsePrecompiledHeader == UsePrecompiledHeaderOptions.Create)
                        continue;

                    file.ExcludeFromBuild(configurationPlatformName);
                }

                if (item is FilterType)
                {
                    FilterType filter = (FilterType) item;
                    ExcludeFromBuild(configurationPlatformName, filter.Items);
                }
            }
        }

        private void ExcludeFromBuild(string configurationPlatformName)
        {
            ExcludeFromBuild(configurationPlatformName, this.Files);
        }

        // TODO: projectConverter 대신 깔끔하게 delegate 를 매개변수로 받는 게 낫지 않을까?
        public void ExcludeFromBuild(AbstractProjectConfigurationNameConverter projectConverter)
        {
            foreach (ConfigurationType configuration in this.Configurations)
            {
                string configurationName = AbstractConfigurationNameConverter.GetConfiguration(configuration.Name);
                if (projectConverter.IsNewName(configurationName) == false)
                    continue;

                ExcludeFromBuild(configuration.Name);
            }
        }

        public void DeleteConfigurationPlatform(string name)
        {
            Trace.Assert(string.IsNullOrEmpty(name) == false);

            ConfigurationType configuration = GetConfiguration(name);
            if (configuration == null)
                throw new ArgumentException();

            // 개별 파일 속성
            DeleteConfigurationPlatformInFiles(this.Files, name);

            // 공통 속성
            IEnumerable<ConfigurationType> configurationsFound = this.Configurations.Where(
                item => item.Name.Equals(name, StringComparison.CurrentCultureIgnoreCase)
                );

            Debug.Assert(configurationsFound.Count() == 1);

            this.Configurations.Remove(
                configurationsFound.First()
                );
        }

        // TODO: 그러고 보니 프로젝트 지우면 파일 속성은 함께 날아가는 것 아닌가?
        private static void DeleteConfigurationPlatformInFiles(IEnumerable<object> items, string name)
        {
            foreach (object item in items)
            {
                Debug.Assert((item is IFileType) || (item is FilterType));
                if (item is FileType)
                {
                    FileType file = (FileType) item;
                    DeleteConfigurationPlatformoInFileBuildConfiguration(file.Items, name);
                }

                if (item is FilterType)
                {
                    FilterType filter = (FilterType) item;
                    DeleteConfigurationPlatformInFiles(filter.Items, name);
                }
            }
        }

        private static void DeleteConfigurationPlatformoInFileBuildConfiguration(List<object> fileBuildConfigurations,
                                                                                 string name)
        {
            int count = fileBuildConfigurations.RemoveAll(
                item => ((BuildConfigurationType) item).Name.Equals(name, StringComparison.CurrentCultureIgnoreCase)
                );
        }
    }
}