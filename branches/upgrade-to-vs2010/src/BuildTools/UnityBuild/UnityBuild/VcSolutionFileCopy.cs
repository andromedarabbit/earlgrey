using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    internal class VcSolutionFileCopy
    {
        // private readonly VcSolution _solution;
        private readonly bool _overwrite;
        private readonly string _solutionFilePath;
        private IProjectFileNameConverter _projectFileNameConverter;

        private readonly List<string> _projectNamesExcluded;

        internal VcSolutionFileCopy(string solutionFilePath)
            : this(solutionFilePath, new ProjectFileNameConverter(), true)
        {
        }

        internal VcSolutionFileCopy(
            string solutionFilePath
            , IProjectFileNameConverter projectFileNameConverter
            , bool overwrite
            )
        {
            Debug.Assert(string.IsNullOrEmpty(solutionFilePath) == false);
            Debug.Assert(projectFileNameConverter != null);

            this._solutionFilePath = solutionFilePath;
            this._projectFileNameConverter = projectFileNameConverter;

            this._projectNamesExcluded = new List<string>();
            this._overwrite = overwrite;
        }

        public IProjectFileNameConverter ProjectConverter
        {
            get { return _projectFileNameConverter; }
            set { _projectFileNameConverter = value; }
        }

        public void ExcludeProject(string projectName)
        {
            _projectNamesExcluded.Add(projectName);
        }

        public void ExcludeProjects(IEnumerable<string> projectNames)
        {
            _projectNamesExcluded.AddRange(projectNames);
        }

        private bool IsExcluded(Project project)
        {
            if (project.ProjectTypeGuid != KnownProjectTypeGuid.VisualC)
                return true;

            int count = _projectNamesExcluded.Count(
                name => name.Equals(project.ProjectName, StringComparison.CurrentCultureIgnoreCase)
                );

            return count > 0;
        }

        public string Copy()
        {
            SolutionFile solution = GetNewSolution();

            foreach (Project project in solution.Projects)
            {
                if (project.IsVcProjectRelated == false)
                    continue;

                string newRelativePath = _projectFileNameConverter.GetNewName(project.RelativePath);

                string solutionDirectory = Path.GetDirectoryName(_solutionFilePath);
                string oldProjectFilePath = Path.GetFullPath(Path.Combine(solutionDirectory, project.RelativePath));
                string newProjectFilePath = Path.GetFullPath(Path.Combine(solutionDirectory, newRelativePath));
                File.Copy(oldProjectFilePath, newProjectFilePath, _overwrite);

                project.RelativePath = newRelativePath;
            }

            solution.Save();
            return solution.SolutionFullPath;
        }

        private SolutionFile GetNewSolution()
        {
            if (File.Exists(_solutionFilePath) == false)
                throw new FileNotFoundException();

            string directory = Path.GetDirectoryName(_solutionFilePath);
            string newSolutionFileName = _projectFileNameConverter.GetNewName(Path.GetFileName(_solutionFilePath));
            string newSolutionFilePath = Path.Combine(directory, newSolutionFileName);
            File.Copy(_solutionFilePath, newSolutionFilePath, _overwrite);

            return SolutionFile.FromFile(newSolutionFilePath);
        }
    }
}