using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    public class AbstractVcSolutionModify
    {
        private readonly VcSolution _solution;
        private AbstractSolutionConfigurationNameConverter _solutionConverter;
        private AbstractProjectConfigurationNameConverter _projectConverter;

        private readonly List<string> _projectNamesExcluded;

        protected AbstractVcSolutionModify(VcSolution solution)
            : this(solution, new SolutionConfigurationNameConverter(), new ProjectConfigurationNameConverter())
        {
        }

        protected AbstractVcSolutionModify(
            VcSolution solution
            , AbstractSolutionConfigurationNameConverter solutionConverter
            , AbstractProjectConfigurationNameConverter projectConverter
            )
        {
            Debug.Assert(solution != null);
            Debug.Assert(solutionConverter != null);
            Debug.Assert(projectConverter != null);

            this._solution = solution;
            this._solutionConverter = solutionConverter;
            this._projectConverter = projectConverter;

            this._projectNamesExcluded = new List<string>();
        }

        protected VcSolution Solution
        {
            get { return _solution; }
        }

        public AbstractSolutionConfigurationNameConverter SolutionConverter
        {
            get { return _solutionConverter; }
            set { _solutionConverter = value; }
        }

        public AbstractProjectConfigurationNameConverter ProjectConverter
        {
            get { return _projectConverter; }
            set { _projectConverter = value; }
        }

        public void ExcludeProject(string projectName)
        {
            _projectNamesExcluded.Add(projectName);
        }

        public void ExcludeProjects(IEnumerable<string> projectNames)
        {
            _projectNamesExcluded.AddRange(projectNames);
        }

        internal IEnumerable<string> ConfigurationPlatformNames
        {
            get { return _solution.ConfigurationPlatformNames; }
        }

        internal PropertyLineHashList ConfigurationPlatforms
        {
            get { return _solution.ConfigurationPlatforms; }
        }

        internal bool HasSolutionConfigurationPlatform(string configurationPlatformName)
        {
            return _solution.HasSolutionConfigurationPlatform(configurationPlatformName);
        }

        protected bool IsExcluded(Project project)
        {
            if (project.ProjectTypeGuid != KnownProjectTypeGuid.VisualC)
                return true;

            int count = _projectNamesExcluded.Count(
                name => name.Equals(project.ProjectName, StringComparison.CurrentCultureIgnoreCase)
                );

            return count > 0;
        }

    }
}