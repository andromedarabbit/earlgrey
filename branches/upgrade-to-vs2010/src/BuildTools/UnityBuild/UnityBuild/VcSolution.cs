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

    	public VisualStudioVersions Version
    	{
    		get
    		{
    			if(Summary.Headers.Exists(item => item.Trim().StartsWith("# Visual Studio 2008")) == true)
    			{
    				return VisualStudioVersions.V2008;
    			}

				if (Summary.Headers.Exists(item => item.Trim().StartsWith("# Visual Studio 2010")) == true)
				{
					return VisualStudioVersions.V2010;
				}

				throw new ApplicationException(
					string.Format("Could not identify the version of this solution file: {0}", _solutionFilePath)
					);
    		}
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

            if (CacheVcProjects() == 0)
            {
                throw new ApplicationException("The solution file has no Visual C++ projects.");
            }
        }

        private int CacheVcProjects()
        {
            Debug.Assert(_vcProjects.Count == 0);

            foreach (var projectSummary in _summary.Projects)
            {
                if (projectSummary.ProjectTypeGuid != KnownProjectTypeGuid.VisualC)
                    continue;

                var vcProject = new VcProject(projectSummary);
                vcProject.Load();

                _vcProjects.Add(vcProject);
            }

            return _vcProjects.Count;
        }

        internal VcProject FindVcProject(Project project)
        {
            Debug.Assert(project != null);

            IEnumerable<VcProject> result = _vcProjects.FindAll(item => item.Summary == project);
            Debug.Assert(result.Count() == 1);

            return result.First();
        }

        internal IEnumerable<VcProject> VcProjects
        {
            get { return _vcProjects; }
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
            get { return this.Summary.GlobalSections["SolutionConfigurationPlatforms"].PropertyLines; }
        }

        public bool HasSolutionConfigurationPlatform(string configurationPlatformName)
        {
            return ConfigurationPlatforms.Contains(configurationPlatformName);
        }

        internal IEnumerable<string> ConfigurationPlatformNames
        {
            get { return ConfigurationPlatforms.Select(item => item.Name); }
        }
    }
}