using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

using CWDev.SLNTools.Core;

namespace UnityBuild
{
    public class VcSolution
    {
        private readonly string _solutionFilePath;
        private SolutionFile _summary;
        private readonly List<VcProject> _vcProjects;

        public VcSolution(string solutionFilePath)
        {            
            _solutionFilePath = solutionFilePath;
            _vcProjects = new List<VcProject>();
        }

        public SolutionFile Summary
        {
            get 
            {
                Debug.Assert(_summary != null);
                return _summary;
            }
        }

        public void Load()
        {
            _summary = SolutionFile.FromFile(_solutionFilePath);

            CacheVcProjects();
        }

        private void CacheVcProjects()
        {
            foreach(var projectSummary in _summary.Projects)
            {
                if (projectSummary.ProjectTypeGuid != KnownProjectTypeGuid.VisualC)
                    continue;

                var vcProject = new VcProject(projectSummary);
                vcProject.Load();

                _vcProjects.Add(vcProject);
            }
        }


        public void Save()
        {
            foreach (var project in _vcProjects)
            {
                project.Save();
            }
            _summary.Save();
        }

        public PropertyLineHashList ConfigurationPlatforms
        {
            get
            {
                return this.Summary.GlobalSections["SolutionConfigurationPlatforms"].PropertyLines;
            }
        }

        public bool HasSolutionConfigurationPlatform(string configurationPlatformName)
        {
            return ConfigurationPlatforms.Contains(configurationPlatformName);
        }

        internal IEnumerable<string> ConfigurationPlatformNames
        {
            get
            {
                return ConfigurationPlatforms.Select(item => item.Name);
            }
        }

        /*
        public void DeleteSolutionConfigurationPlatform(
            string solutionConfigurationName
            , string solutionPlatformName
            )
        {
            Trace.Assert(string.IsNullOrEmpty(solutionConfigurationName) == false);
            Trace.Assert(string.IsNullOrEmpty(solutionPlatformName) == false);

            string configurationPlatformName = AbstractConfigurationNameConverter.GetConfigurationPlatform(solutionConfigurationName, solutionPlatformName);

            if (HasSolutionConfigurationPlatform(configurationPlatformName) == false)
                throw new ArgumentException(); // TODO: 그냥 정상 상황으로 처리해서 return할까?

            

            // 파일에서 구성 플랫폼 값 제거하기
            // 프로젝트에서 구성 플랫폼 값 제거하기
            // 솔루션 파일에서 구성 플랫폼 값 제거하기

            if(ConfigurationPlatforms.Remove(configurationPlatformName) == false)
                throw new ApplicationException();

        }
         * */

        public void CopySolutionConfigurationPlatform(
            AbstractSolutionConfigurationNameConverter solutionConverter
            , AbstractProjectConfigurationNameConverter projectConverter
            )
        {
            foreach (string configurationPlatform in ConfigurationPlatformNames.ToList())
            {
                string platformName;
                string configurationName; 
                string property;

                AbstractConfigurationNameConverter.SplitConfigurationPlatform(
                    configurationPlatform, out configurationName, out platformName, out property
                    );

                Debug.Assert(string.IsNullOrEmpty(platformName) == false);
                Debug.Assert(string.IsNullOrEmpty(configurationName) == false);
                Debug.Assert(string.IsNullOrEmpty(property));

                CopySolutionConfigurationPlatform(configurationName, platformName, solutionConverter, projectConverter, true);
            }
        }

        public void CopySolutionConfigurationPlatform(
            string srcSolutionConfigurationName
            , string srcSolutionPlatformName
            , AbstractSolutionConfigurationNameConverter solutionConverter
            , AbstractProjectConfigurationNameConverter projectConverter
            )
        {
            CopySolutionConfigurationPlatform(
                srcSolutionConfigurationName
                , srcSolutionPlatformName
                , solutionConverter
                , projectConverter
                , false
                );
        }

        private void CopySolutionConfigurationPlatform(
            string srcSolutionConfigurationName
            , string srcSolutionPlatformName
            , AbstractSolutionConfigurationNameConverter solutionConverter
            , AbstractProjectConfigurationNameConverter projectConverter
            , bool skipIfConfigurationAlreadyExists
            )
        {
            Trace.Assert(string.IsNullOrEmpty(srcSolutionConfigurationName) == false);
            Trace.Assert(string.IsNullOrEmpty(srcSolutionPlatformName) == false);
            // Trace.Assert(string.IsNullOrEmpty(dstConfigurationName) == false);
            Trace.Assert(solutionConverter != null);
            Trace.Assert(projectConverter != null);

            string dstConfigurationName = solutionConverter.GetNewName(srcSolutionConfigurationName);

            string srcConfigurationPlatformName = 
                AbstractConfigurationNameConverter.GetConfigurationPlatform(srcSolutionConfigurationName, srcSolutionPlatformName);
            string dstConfigurationPlatformName = 
                AbstractConfigurationNameConverter.GetConfigurationPlatform(dstConfigurationName, srcSolutionPlatformName);
            string dstConfigurationPlatformValue = dstConfigurationPlatformName;

            if (HasSolutionConfigurationPlatform(srcConfigurationPlatformName) == false)
                throw new ArgumentException();

            if (HasSolutionConfigurationPlatform(dstConfigurationPlatformName) == true)
                throw new ArgumentException();

            // 원본 SolutionConfigurationPlatform 의 복사본을 만든다.
            ConfigurationPlatforms.Add(
                new PropertyLine(dstConfigurationPlatformName, dstConfigurationPlatformValue)
                );


            // 솔루션 파일 내의 프로젝트 설정 변경
            foreach(var projectSummary in _summary.Projects)
            {
                // TODO: 폴더 안에 프로젝트가 있는 경우는 어떻하려고?
                if (projectSummary.ProjectTypeGuid == KnownProjectTypeGuid.SolutionFolder) 
                    continue;

                // 원본 SolutionConfigurationPlatform 에 해당하는 활성화된 ProjectConfigurationPlatform 을 찾는다.
                // - SolutionConfigurationPlatform.Build0 값이 있으면 활성화된 것이다.
                // - 활성화되지 않은 경우에도 SolutionConfigurationPlatform.ActiveCfg 는 복사해야 한다.
                IEnumerable<PropertyLine> activeConfigurations =
                    projectSummary.ProjectConfigurationPlatformsLines.Where(
                        propertyLine => propertyLine.Name.StartsWith(srcConfigurationPlatformName, StringComparison.CurrentCultureIgnoreCase)
                    );


                // 활성화된 VCProject 의 복사본을 만든다.
                List<PropertyLine> newConfigurations = new List<PropertyLine>(activeConfigurations.Count());
                if (projectSummary.ProjectTypeGuid != KnownProjectTypeGuid.VisualC)
                {
                    foreach (var activeConfiguration in activeConfigurations)
                    {
                        string newSolutionName = AbstractConfigurationNameConverter.GetNewName(
                            activeConfiguration.Name
                            , solutionConverter
                            );

                        newConfigurations.Add(
                            new PropertyLine(newSolutionName, activeConfiguration.Value)
                            );
                    }
                }
                else
                {
                    foreach (var activeConfiguration in activeConfigurations)
                    {
                        PropertyLine newConfiguration = GetNewConfiguration(activeConfiguration, solutionConverter, projectConverter);
                        Debug.Assert(newConfiguration != null);

                        newConfigurations.Add(newConfiguration);
                    }

                }

                projectSummary.ProjectConfigurationPlatformsLines.AddRange(newConfigurations);


                if (projectSummary.ProjectTypeGuid != KnownProjectTypeGuid.VisualC)
                    continue;
                

                // VCProject 파일 수정
                var projectNames = activeConfigurations.Select(configItem => configItem.Value).Distinct();

                
                IEnumerable<VcProject> result = _vcProjects.FindAll(item => item.Summary == projectSummary);
                Debug.Assert(result.Count() == 1);

                VcProject project = result.First();

                foreach (var projectName in projectNames)
                {
                    if (skipIfConfigurationAlreadyExists == true && project.HasConfiguration(projectName) == true)
                        continue;   

                    project.CopyConfigurationPlatform(
                        projectName
                        , AbstractConfigurationNameConverter.GetNewName(projectName, projectConverter)
                        );
                }
                
            }

        }

        private static PropertyLine GetNewConfiguration(
            PropertyLine activeConfiguration
            , AbstractSolutionConfigurationNameConverter solutionConverter
            , AbstractProjectConfigurationNameConverter projectConverter
            )
        {
            Debug.Assert(string.IsNullOrEmpty(activeConfiguration.Value) == false);

            string newSolutionConfigurationPlatformName = AbstractConfigurationNameConverter.GetNewName(
                activeConfiguration.Name
                , solutionConverter
                );
            string newProjectConfigurationPlatformName = AbstractConfigurationNameConverter.GetNewName(
                activeConfiguration.Value
                , projectConverter
                );

            return new PropertyLine(newSolutionConfigurationPlatformName, newProjectConfigurationPlatformName);
        }
    }
}


