using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    public class VcProject
    {
        private readonly Project _projectSummary;
        private VisualStudioProjectType _projectDetails;

        public VcProject(Project projectSummary) 
        {
            Debug.Assert(projectSummary != null);
            Debug.Assert(
                projectSummary.ProjectTypeGuid == KnownProjectTypeGuid.VisualC
                );

            _projectSummary = projectSummary;
        }

        public void Load()
        {
            Debug.Assert(File.Exists(_projectSummary.FullPath));
            Debug.Assert(_projectDetails == null);
            
            _projectDetails = VisualStudioProjectType.LoadFromFile(_projectSummary.FullPath);
            Debug.Assert(_projectDetails != null);
        }

        public void Save()
        {
            _projectDetails.SaveToFile(_projectSummary.FullPath);
        }

        public VisualStudioProjectType Details
        {
            get
            {
                Debug.Assert(_projectDetails != null);
                return _projectDetails;
            }
        }

        public Project Summary
        {
            get 
            { 
                Debug.Assert(_projectSummary != null);
                return _projectSummary;
            }
        }

        public PropertyLineHashList ConfigurationPlatforms
        {
            get
            {
                return this.Summary.ProjectConfigurationPlatformsLines;
            }
        }

        public bool HasConfigurationPlatform(string configurationPlatformName)
        {
            return
                _projectDetails.Configurations.Count(
                    item => item.Name.Equals(configurationPlatformName, StringComparison.CurrentCultureIgnoreCase)
                    ) > 0;
        }

        internal PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationPlatformName)
        {
            ConfigurationType configuration = GetConfiguration(configurationPlatformName);
            if (configuration == null)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);

            IEnumerable<ConfigurationTypeTool> tools = from item in configuration.Tool
                                                       where item.Name == "VCCLCompilerTool"
                                                       select item;

            if(tools.Count() == 0)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);

            Debug.Assert(tools.Count() == 1);

            ConfigurationTypeTool tool = tools.First();

            PrecompiledHeaderOptions options = PrecompiledHeaderOptions.CreateInstance(tool);
            if (options.UsePrecompiledHeader != UsePrecompiledHeaderOptions.InheritFromProject)
            {
                if(string.IsNullOrEmpty(options.PrecompiledHeaderFile))
                    options.PrecompiledHeaderFile = @"$(IntDir)\$(TargetName).pch";
                
                if(string.IsNullOrEmpty(options.PrecompiledHeaderThrough))
                    options.PrecompiledHeaderThrough = "StdAfx.h";

                return options;
            }

            return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);
        }

        internal PrecompiledHeaderOptions GetPrecompiledHeaderOption(string configurationBuild, FileType file)
        {
            if (file.IsSrcFile == false)
                return new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);

            PrecompiledHeaderOptions fileOption = file.GetPrecompiledHeaderOption(configurationBuild);
            if (fileOption.UsePrecompiledHeader == UsePrecompiledHeaderOptions.None)
            {
                return fileOption;
            }

            PrecompiledHeaderOptions projectOptions = GetPrecompiledHeaderOption(configurationBuild);

            if (fileOption.UsePrecompiledHeader == UsePrecompiledHeaderOptions.InheritFromProject)
                fileOption.UsePrecompiledHeader = projectOptions.UsePrecompiledHeader;

            if (string.IsNullOrEmpty(fileOption.PrecompiledHeaderFile))
                fileOption.PrecompiledHeaderFile = projectOptions.PrecompiledHeaderFile;
            if (string.IsNullOrEmpty(fileOption.PrecompiledHeaderThrough))
                fileOption.PrecompiledHeaderThrough = projectOptions.PrecompiledHeaderThrough;

            return fileOption;            
        }

        // 이름은 Headers 인데 실제론 /Yc 옵션이 적용된 .cpp 파일을 반환한다.
        public IEnumerable<FileType> GetPrecompiledHeaders(string configurationPlatform)
        {
            Debug.Assert(string.IsNullOrEmpty(configurationPlatform) == false);

            List<FileType> filesFound = new List<FileType>();
            GetPrecompiledHeaders(configurationPlatform, _projectDetails.Files, filesFound);
            return filesFound;
        }

        private void GetPrecompiledHeaders(string configurationPlatform, IEnumerable<object> items, List<FileType> filesFound)
        {
            Debug.Assert(filesFound != null);

            if(items.Count() == 0)
                return;

            foreach (object item in items)
            {
                Debug.Assert((item is FileType) || (item is FilterType));
                if (item is FileType)
                {
                    FileType file = (FileType)item;
                    PrecompiledHeaderOptions options = GetPrecompiledHeaderOption(configurationPlatform, file);
                    if(options.UsePrecompiledHeader != UsePrecompiledHeaderOptions.Create)
                        continue;
                    
                    filesFound.Add(file);
                }

                if (item is FilterType)
                {
                    FilterType filter = (FilterType)item;
                    GetPrecompiledHeaders(configurationPlatform, filter.Items, filesFound);
                }
            }
        }

        public void DeleteConfigurationPlatform(string name)
        {
            Trace.Assert(string.IsNullOrEmpty(name) == false);

            ConfigurationType configuration = GetConfiguration(name);
            if (configuration == null)
                throw new ArgumentException();

            // 개별 파일 속성
            DeleteConfigurationPlatformInFiles(_projectDetails.Files, name);

            // 공통 속성
            IEnumerable<ConfigurationType> configurationsFound = _projectDetails.Configurations.Where(
                item => item.Name.Equals(name, StringComparison.CurrentCultureIgnoreCase)
                );

            Debug.Assert(configurationsFound.Count() == 1);

            _projectDetails.Configurations.Remove(
                configurationsFound.First()
                );

        }

        // TODO: 그러고 보니 프로젝트 지우면 파일 속성은 함께 날아가는 것 아닌가?
        private static void DeleteConfigurationPlatformInFiles(IEnumerable<object> items, string name)
        {
            foreach (object item in items)
            {
                Debug.Assert((item is FileType) || (item is FilterType));
                if (item is FileType)
                {
                    FileType file = (FileType)item;
                    DeleteConfigurationPlatformoInFileBuildConfiguration(file.Items, name);
                }

                if (item is FilterType)
                {
                    FilterType filter = (FilterType)item;
                    DeleteConfigurationPlatformInFiles(filter.Items, name);
                }
            }
        }
        
        private static void DeleteConfigurationPlatformoInFileBuildConfiguration(List<object> fileBuildConfigurations, string name)
        {
            int count = fileBuildConfigurations.RemoveAll(
                item => ((BuildConfigurationType)item).Name.Equals(name, StringComparison.CurrentCultureIgnoreCase)                    
                );
        }

        public void CopyConfigurationPlatform(string srcName, string dstName)
        {
            Trace.Assert(string.IsNullOrEmpty(srcName) == false);
            Trace.Assert(string.IsNullOrEmpty(dstName) == false);

            ConfigurationType configuration = GetConfiguration(srcName);
            if(configuration == null)
                throw new ArgumentException();

            ConfigurationType dstConfiguration = GetConfiguration(dstName);
            if (dstConfiguration != null)
                throw new ArgumentException();
            
            // 공통 속성
            ConfigurationType newConfiguration = configuration.Clone();
            newConfiguration.Name = dstName;
            _projectDetails.Configurations.Add(newConfiguration);

            // 개별 파일 속성
            CopyConfigurationPlatformInFiles(_projectDetails.Files, srcName, dstName);

        }

        private static void CopyConfigurationPlatformInFiles(IEnumerable<object> items, string srcName, string dstName)
        {
            foreach (object item in items)
            {
                Debug.Assert( (item is FileType) || (item is FilterType) );
                if (item is FileType)
                {
                    FileType file = (FileType)item;
                    CopyConfigurationPlatformoInFileBuildConfiguration(file.Items, srcName, dstName);
                }

                if (item is FilterType)
                {
                    FilterType filter = (FilterType) item;
                    CopyConfigurationPlatformInFiles(filter.Items, srcName, dstName);
                }
            }
        }

        private static void CopyConfigurationPlatformoInFileBuildConfiguration(List<object> fileBuildConfigurations, string srcName, string dstName)
        {
            // TODO: 이름이 같은 구성 값은 하나 뿐이어야 할 것 같지만 안전하게 List 로 관리한다. 일단은....
            List<object> newBuildConfigurations = new List<object>();

            foreach (var item in fileBuildConfigurations)
            {
                Debug.Assert(item is BuildConfigurationType);
                
                BuildConfigurationType buildConfiguration = (BuildConfigurationType) (item);
                if(buildConfiguration.Name.Equals(srcName,StringComparison.CurrentCultureIgnoreCase) == false)
                    continue;

                BuildConfigurationType newBuildConfiguration = (BuildConfigurationType)buildConfiguration.Clone();
                newBuildConfiguration.Name = dstName;
                newBuildConfigurations.Add(newBuildConfiguration);
            }


            fileBuildConfigurations.AddRange(newBuildConfigurations);
            
        }

        internal bool HasConfiguration(string name)
        {
            return GetConfiguration(name) != null;
        }

        private ConfigurationType GetConfiguration(string srcName)
        {
            return _projectDetails.Configurations.Find(
                item => item.Name.Equals(srcName, StringComparison.CurrentCultureIgnoreCase)
                );
        }

        private static void ExcludeFromBuild(string configurationPlatformName, IEnumerable<object> items)
        {
            foreach (object item in items)
            {
                Debug.Assert((item is FileType) || (item is FilterType));
                if (item is FileType)
                {
                    FileType file = (FileType)item;
                    if (file.IsSrcFile == false)
                        continue;

                    PrecompiledHeaderOptions options = file.GetPrecompiledHeaderOption(configurationPlatformName);
                    if(options.UsePrecompiledHeader == UsePrecompiledHeaderOptions.Create)
                        continue;

                    file.ExcludeFromBuild(configurationPlatformName);
                }

                if (item is FilterType)
                {
                    FilterType filter = (FilterType)item;
                    ExcludeFromBuild(configurationPlatformName, filter.Items);
                }
            }
        }

        private void ExcludeFromBuild(string configurationPlatformName)
        {
            ExcludeFromBuild(configurationPlatformName, _projectDetails.Files);
        }

        // TODO: projectConverter 대신 깔끔하게 delegate 를 매개변수로 받는 게 낫지 않을까?
        public void ExcludeFromBuild(AbstractProjectConfigurationNameConverter projectConverter)
        {
            foreach(ConfigurationType configuration in _projectDetails.Configurations)
            {
                string configurationName = AbstractConfigurationNameConverter.GetConfiguration(configuration.Name);
                if(projectConverter.IsNewName(configurationName) == false)
                    continue;
                
                ExcludeFromBuild(configuration.Name);
            }
        }
    }
}
