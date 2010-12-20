﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public class Builder : IDisposable
    {
        private readonly string _solutionFilePath;
        private readonly VcSolution _vcSolution;

        private AbstractSolutionConfigurationNameConverter _solutionConverter;
        private AbstractProjectConfigurationNameConverter _projectConverter;

        private readonly List<string> _projectNamesExcluded;

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
        }

        public Builder(
            string solutionFilePath
            )
            : this(solutionFilePath, new SolutionConfigurationNameConverter(), new ProjectConfigurationNameConverter())
        {
        }

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

        public void Open()
        {
            _vcSolution.Load();

            VcSolutionCopy copy = new VcSolutionCopy(_vcSolution, _solutionConverter, _projectConverter);
            if(_projectNamesExcluded.Count > 0)
            {
                
            }

            copy.CopySolutionConfigurationPlatform();

            foreach (VcProject project in _vcSolution.VcProjects)
            {
                project.ExcludeFromBuild(_projectConverter);

                // VcProjectMerge projectMerge = new VcProjectMerge(project);
                // projectMerge.Merge();
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