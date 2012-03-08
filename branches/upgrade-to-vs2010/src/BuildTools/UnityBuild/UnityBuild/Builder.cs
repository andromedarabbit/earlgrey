using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public class Builder : IDisposable
    {
        private readonly BuilderOptions _options;
        private readonly string _solutionFilePath;

        private AbstractSolutionConfigurationNameConverter _solutionConverter;
        private AbstractProjectConfigurationNameConverter _projectConverter;

        public Builder(
            string solutionFilePath
            , AbstractSolutionConfigurationNameConverter solutionConverter
            , AbstractProjectConfigurationNameConverter projectConverter
            , BuilderOptions options
            )
        {
            Debug.Assert(string.IsNullOrEmpty(solutionFilePath) == false);
            Debug.Assert(solutionConverter != null);
            Debug.Assert(projectConverter != null);
            Debug.Assert(options != null);

            _solutionFilePath = solutionFilePath;
            _solutionConverter = solutionConverter;
            _projectConverter = projectConverter;
            _options = options;
        }

        public Builder(
            string solutionFilePath
            , BuilderOptions options
            )
            : this(
                solutionFilePath, new SolutionConfigurationNameConverter(), new ProjectConfigurationNameConverter(),
                options)
        {
        }


        public Builder(
            string solutionFilePath
            )
            : this(solutionFilePath, new BuilderOptions())
        {
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

        private bool IsExcluded(VcProject project)
        {
            string projectName = project.Name;
            return _options.IsExcluded(projectName);
        }

        private string CopySolution()
        {
            if (_options.CopySolution == false)
                return _solutionFilePath;

            VcSolutionFileCopy fileCopy = new VcSolutionFileCopy(_solutionFilePath);
            return fileCopy.Copy();
        }

        public void Run()
        {
            string solutionFilePath = CopySolution();

            DeleteExistingUnityBuildCOnfigurationPlatforms(solutionFilePath);

            CreateUnityBuildConfigurationPlatforms(solutionFilePath);

            VcSolution vcSolution = new VcSolution(solutionFilePath);
            vcSolution.Load();

            foreach (VcProject project in vcSolution.VcProjects)
            {
                if (IsExcluded(project) == true)
                    continue;

                // UnityBuild 시 기존 소스 코드를 빌드 대상에서 제외함
                project.ExcludeFromBuild(_projectConverter);

                //// UnityBuild 용 소스 코드는 기존 빌드의 빌드 대상에서 제외함
                VcProjectMerge projectMerge = new VcProjectMerge(project, _projectConverter, _options.GroupByFilter,
                                                                 _options.MaxFilesPerFile);
                List<IFilterOrFile> itemsAdded = projectMerge.Merge();
            }

            vcSolution.Save();
        }

        private void CreateUnityBuildConfigurationPlatforms(string solutionFilePath)
        {
            VcSolution solution = new VcSolution(solutionFilePath);
            solution.Load();

            VcSolutionCopy copy 
                = new VcSolutionCopy(solution, _solutionConverter, _projectConverter);
            
            if (_options.ExcludedProjects.Count() > 0)
            {
                copy.ExcludeProjects(_options.ExcludedProjects);
            }
            copy.CopySolutionConfigurationPlatform();

            solution.Save();
        }

        private void DeleteExistingUnityBuildCOnfigurationPlatforms(string solutionFilePath)
        {
            VcSolution solution = new VcSolution(solutionFilePath);
            solution.Load();

            VcSolutionDelete solutionDelete = 
                new VcSolutionDelete(solution, _solutionConverter, _projectConverter);

            if (_options.ExcludedProjects.Count() > 0)
            {
                solutionDelete.ExcludeProjects(_options.ExcludedProjects);
            }
            solutionDelete.DeleteSolutionConfigurationPlatform();

            solution.Save();
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