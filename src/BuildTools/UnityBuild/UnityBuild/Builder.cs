using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    public class Builder : IDisposable
    {
        private readonly string _solutionFilePath;
        private readonly VcSolution _vcSolution;

        private AbstractSolutionConfigurationNameConverter _solutionConverter;
        private AbstractProjectConfigurationNameConverter _projectConverter;

        private readonly List<string> _projectNamesExcluded;

        // private bool _preservePrecompiledHeaders;

        public Builder(
            string solutionFilePath
            , AbstractSolutionConfigurationNameConverter solutionConverter
            , AbstractProjectConfigurationNameConverter projectConverter
            )
        {
            Debug.Assert(string.IsNullOrEmpty(solutionFilePath) == false);
            Debug.Assert(solutionConverter != null);
            Debug.Assert(projectConverter != null);
            
            _solutionFilePath = solutionFilePath;
            _solutionConverter = solutionConverter;
            _projectConverter = projectConverter;

            _vcSolution = new VcSolution(_solutionFilePath);

            _projectNamesExcluded = new List<string>();

        //    _preservePrecompiledHeaders = true;
        }

        public Builder(
            string solutionFilePath
            )
            : this(solutionFilePath, new SolutionConfigurationNameConverter(), new ProjectConfigurationNameConverter())
        {
        }

        //public bool PreservePrecompiledHeaders
        //{
        //    get { return _preservePrecompiledHeaders; }
        //    set { _preservePrecompiledHeaders = value; }
        //}

        public string SolutionFilePath
        {
            get { return _solutionFilePath; }
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

        private bool IsExcluded(VcProject project)
        {
            string projectName = project.Summary.ProjectName;

            int count = _projectNamesExcluded.Count(
                name => name.Equals(projectName, StringComparison.CurrentCultureIgnoreCase) == true
                );

            return count > 0;
        }

        public void Open()
        {
            _vcSolution.Load();

            VcSolutionCopy copy = new VcSolutionCopy(_vcSolution, _solutionConverter, _projectConverter);
            if(_projectNamesExcluded.Count > 0)
            {
                copy.ExcludeProjects(_projectNamesExcluded);
            }
            copy.CopySolutionConfigurationPlatform();

            foreach (VcProject project in _vcSolution.VcProjects)
            {
                if (IsExcluded(project) == true)
                    continue;

                // UnityBuild 시 기존 소스 코드를 빌드 대상에서 제외함
                project.ExcludeFromBuild(_projectConverter);

                // UnityBuild 용 소스 코드는 기존 빌드의 빌드 대상에서 제외함
                VcProjectMerge projectMerge = new VcProjectMerge(project);
                projectMerge.ExcludeFromBuild(_projectConverter);
                List<IFilterOrFile> itemsAdded = projectMerge.Merge();
            }
            
        }

        #region IDisposable

        // Track whether Dispose has been called.
        private bool _disposed = false;

        // Implement IDisposable.
        // Do not make this method virtual.
        // A derived class should not be able to override this method.
        public void Dispose()
        {
            Dispose(true);
            // This object will be cleaned up by the Dispose method.
            // Therefore, you should call GC.SupressFinalize to
            // take this object off the finalization queue
            // and prevent finalization code for this object
            // from executing a second time.
            GC.SuppressFinalize(this);
        }

        // Dispose(bool disposing) executes in two distinct scenarios.
        // If disposing equals true, the method has been called directly
        // or indirectly by a user's code. Managed and unmanaged resources
        // can be disposed.
        // If disposing equals false, the method has been called by the
        // runtime from inside the finalizer and you should not reference
        // other objects. Only unmanaged resources can be disposed.
        private void Dispose(bool disposing)
        {
            // Check to see if Dispose has already been called.
            if (!this._disposed)
            {
                // If disposing equals true, dispose all managed
                // and unmanaged resources.
                if (disposing)
                {
                    // Dispose managed resources.
                    _vcSolution.Save();
                }

                // Call the appropriate methods to clean up
                // unmanaged resources here.
                // If disposing is false,
                // only the following code is executed.


                // Note disposing has been done.
                _disposed = true;
            }
        }

        #endregion
    }
}
