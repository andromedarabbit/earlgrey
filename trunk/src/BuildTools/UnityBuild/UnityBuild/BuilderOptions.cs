using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public class BuilderOptions
    {
        private bool _copySolution;
        private bool _groupByFilter;
        private int _maxFilesPerFile;
        private readonly List<string> _projectNamesExcluded;

        public BuilderOptions()
        {
            _copySolution = false;
            _groupByFilter = true;
            _maxFilesPerFile = 0;

            _projectNamesExcluded = new List<string>();
        }

        public bool CopySolution
        {
            get { return _copySolution; }
            set { _copySolution = value; }
        }

        public bool GroupByFilter
        {
            get { return _groupByFilter; }
            set { _groupByFilter = value; }
        }

        public int MaxFilesPerFile
        {
            get { return _maxFilesPerFile; }
            set
            {
                if(value < 0)
                    throw new ArgumentException();
                _maxFilesPerFile = value;
            }
        }

        public IEnumerable<string> ExcludedProjects
        {
            get { return _projectNamesExcluded; }
        }

        public void ExcludeProject(string projectName)
        {
            _projectNamesExcluded.Add(projectName);
        }

        public void ExcludeProjects(IEnumerable<string> projectNames)
        {
            _projectNamesExcluded.AddRange(projectNames);
        }

        public bool IsExcluded(string projectName)
        {
            int count = this.ExcludedProjects.Count(
                name => name.Equals(projectName, StringComparison.CurrentCultureIgnoreCase) == true
                );

            return count > 0;
        }

    }
}
