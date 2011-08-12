using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    public class VcSolutionDelete : AbstractVcSolutionModify
    {
        public VcSolutionDelete(VcSolution solution)
            : base(solution)
        {
        }

        public VcSolutionDelete(
            VcSolution solution
            , AbstractSolutionConfigurationNameConverter solutionConverter
            , AbstractProjectConfigurationNameConverter projectConverter
        )
            : base(solution, solutionConverter, projectConverter)
        {
        }

        public void DeleteSolutionConfigurationPlatform()
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

                if (SolutionConverter.IsNewName(configurationName) == false)
                    continue;

                DeleteSolutionConfigurationPlatform(configurationName, platformName);
            }
        }

        private void DeleteSolutionConfigurationPlatform(
            string solutionConfigurationName
            , string solutionPlatformName
            )
        {
            Trace.Assert(string.IsNullOrEmpty(solutionConfigurationName) == false);
            Trace.Assert(string.IsNullOrEmpty(solutionPlatformName) == false);

            Trace.Assert(SolutionConverter != null);
            Trace.Assert(ProjectConverter != null);

            string configurationPlatformName =
                AbstractConfigurationNameConverter.GetConfigurationPlatform(solutionConfigurationName,
                                                                            solutionPlatformName);

            if (HasSolutionConfigurationPlatform(configurationPlatformName) == false)
                throw new ArgumentException();


            if(ConfigurationPlatforms.Remove(configurationPlatformName) == false)
                throw new ApplicationException();

            for (int i = 0; i < Solution.Summary.Projects.Count; i++ )
            {
                var projectSummary = Solution.Summary.Projects[i];
            
                if (projectSummary.ProjectTypeGuid != KnownProjectTypeGuid.VisualC)
                    continue;

                // 원본 SolutionConfigurationPlatform 에 해당하는 활성화된 ProjectConfigurationPlatform 을 찾는다.
                // - SolutionConfigurationPlatform.Build0 값이 있으면 활성화된 것이다.
                // - 활성화되지 않은 경우에도 SolutionConfigurationPlatform.ActiveCfg 는 복사해야 한다.
                List<string> activeConfigurationNames =
                    projectSummary.ProjectConfigurationPlatformsLines
                        .Where(
                            propertyLine =>
                            propertyLine.Name.StartsWith(configurationPlatformName,
                                                         StringComparison.CurrentCultureIgnoreCase)
                        )
                        .Select(propertyLine => propertyLine.Name)
                        .ToList()
                    ;

                foreach (string activeConfigurationName in activeConfigurationNames)
                {
                    projectSummary.ProjectConfigurationPlatformsLines.Remove(
                        activeConfigurationName
                        );
                }

                var projectConfigurationPlatforms =
                    projectSummary.ProjectConfigurationPlatformsLines.Select(configItem => configItem.Value).Distinct();

                VcProject project = Solution.FindVcProject(projectSummary);

                foreach (var projectConfigurationPlatform in projectConfigurationPlatforms)
                {
                    string projectConfiguration =
                        AbstractConfigurationNameConverter.GetConfiguration(projectConfigurationPlatform);

                    if (ProjectConverter.IsNewName(projectConfiguration) == false)
                        continue;

                    if (project.HasConfiguration(projectConfigurationPlatform) == false)
                        continue;

                    VcProjectDelete projectDelete =
                        new VcProjectDelete(project, projectConfigurationPlatform, ProjectConverter);

                    projectDelete.DeleteConfigurationPlatform();
                }
            }
        }


    }
}