using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    internal class VcSolutionCopy : AbstractVcSolutionModify
    {
        internal VcSolutionCopy(VcSolution solution)
            : base(solution)
        {
        }

        internal VcSolutionCopy(
            VcSolution solution
            , AbstractSolutionConfigurationNameConverter solutionConverter
            , AbstractProjectConfigurationNameConverter projectConverter
        )
            : base(solution, solutionConverter, projectConverter)
        {
        }

        public void CopySolutionConfigurationPlatform()
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

                CopySolutionConfigurationPlatform(configurationName, platformName, true);
            }
        }

        public void CopySolutionConfigurationPlatform(string srcSolutionConfigurationName,
                                                      string srcSolutionPlatformName)
        {
            CopySolutionConfigurationPlatform(
                srcSolutionConfigurationName
                , srcSolutionPlatformName
                , false
                );
        }

        internal void CopySolutionConfigurationPlatform(
            string srcSolutionConfigurationName
            , string srcSolutionPlatformName
            , bool skipIfConfigurationAlreadyExists
            )
        {
            Trace.Assert(string.IsNullOrEmpty(srcSolutionConfigurationName) == false);
            Trace.Assert(string.IsNullOrEmpty(srcSolutionPlatformName) == false);
            // Trace.Assert(string.IsNullOrEmpty(dstConfigurationName) == false);
            Trace.Assert(SolutionConverter != null);
            Trace.Assert(ProjectConverter != null);

            string dstConfigurationName = SolutionConverter.GetNewName(srcSolutionConfigurationName);

            string srcConfigurationPlatformName =
                AbstractConfigurationNameConverter.GetConfigurationPlatform(srcSolutionConfigurationName,
                                                                            srcSolutionPlatformName);
            string dstConfigurationPlatformName =
                AbstractConfigurationNameConverter.GetConfigurationPlatform(dstConfigurationName,
                                                                            srcSolutionPlatformName);
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
            foreach (var projectSummary in Solution.Summary.Projects)
            {
                // TODO: 폴더 안에 프로젝트가 있는 경우는 어떻하려고?
                if (projectSummary.ProjectTypeGuid == KnownProjectTypeGuid.SolutionFolder)
                    continue;

                // 원본 SolutionConfigurationPlatform 에 해당하는 활성화된 ProjectConfigurationPlatform 을 찾는다.
                // - SolutionConfigurationPlatform.Build0 값이 있으면 활성화된 것이다.
                // - 활성화되지 않은 경우에도 SolutionConfigurationPlatform.ActiveCfg 는 복사해야 한다.
                IEnumerable<PropertyLine> activeConfigurations =
                    projectSummary.ProjectConfigurationPlatformsLines.Where(
                        propertyLine =>
                        propertyLine.Name.StartsWith(srcConfigurationPlatformName,
                                                     StringComparison.CurrentCultureIgnoreCase)
                        );


                // 활성화된 VCProject 의 복사본을 만든다.
                List<PropertyLine> newConfigurations = new List<PropertyLine>(activeConfigurations.Count());
                // if (projectSummary.ProjectTypeGuid != KnownProjectTypeGuid.VisualC)
                if (IsExcluded(projectSummary))
                {
                    foreach (var activeConfiguration in activeConfigurations)
                    {
                        string newSolutionName = AbstractConfigurationNameConverter.GetNewName(
                            activeConfiguration.Name
                            , SolutionConverter
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
                        PropertyLine newConfiguration = GetNewConfiguration(activeConfiguration, SolutionConverter,
                                                                            ProjectConverter);
                        Debug.Assert(newConfiguration != null);

                        newConfigurations.Add(newConfiguration);
                    }
                }

                projectSummary.ProjectConfigurationPlatformsLines.AddRange(newConfigurations);


                // if (projectSummary.ProjectTypeGuid != KnownProjectTypeGuid.VisualC)
                if (IsExcluded(projectSummary))
                    continue;


                // VCProject 파일 수정
                var projectConfigurationPlatforms =
                    projectSummary.ProjectConfigurationPlatformsLines.Select(configItem => configItem.Value).Distinct();

                VcProject project = Solution.FindVcProject(projectSummary);

                foreach (var projectConfigurationPlatform in projectConfigurationPlatforms)
                {
                    string projectConfiguration =
                        AbstractConfigurationNameConverter.GetConfiguration(projectConfigurationPlatform);

                    if (ProjectConverter.IsNewName(projectConfiguration) == true)
                        continue;

                    string newProjectConfigurationPlatform =
                        AbstractConfigurationNameConverter.GetNewName(projectConfigurationPlatform, ProjectConverter);

                    if (skipIfConfigurationAlreadyExists == true &&
                        project.HasConfiguration(newProjectConfigurationPlatform) == true)
                        continue;

                    VcProjectCopy projectCopy =
                        new VcProjectCopy(project, projectConfigurationPlatform, newProjectConfigurationPlatform);

                    // TODO: 하드코딩
                    string definition = "UNITYBUILD_" + projectConfigurationPlatform.ToUpper();
                    definition = definition.Replace("|", "_");
                    definition = definition.Replace("-", "_");

                    projectCopy.AddPreprocessorDefinition(definition);
                    projectCopy.CopyConfigurationPlatform();
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