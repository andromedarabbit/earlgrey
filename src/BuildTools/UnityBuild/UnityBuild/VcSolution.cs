using System;
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

        public bool HasConfigurationPlatform(string configurationPlatformName)
        {
            return ConfigurationPlatforms.Contains(configurationPlatformName);
        }
    }
}
