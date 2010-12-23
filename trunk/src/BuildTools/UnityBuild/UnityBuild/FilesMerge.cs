using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using CWDev.SLNTools.Core;

namespace UnityBuild
{
    internal class FilesMerge
    {
        // private readonly Project _project;
        // private readonly FilterType _parentFilter;
        private readonly string _projectDirectory;
        private readonly ICollection<FileType> _files;
        private readonly Dictionary<string, List<FileType>> _filesByPath;

        private readonly List<string> _buildConfigurations;
        private readonly List<string> _buildConfigurationsExcluded;

        // public FilesMerge(Project project, FilterType parentFilter, ICollection<FileType> files)
        // public FilesMerge(Project project, ICollection<FileType> files)
        public FilesMerge(string projectDirectory, ICollection<FileType> files, IEnumerable<string> buildConfigurations)
            : this(projectDirectory, files, buildConfigurations, new List<string>())
        {
            
        }

        public FilesMerge(string projectDirectory, ICollection<FileType> files, IEnumerable<string> buildConfigurations, IEnumerable<string> buildConfigurationsExcluded)
        {
            // Debug.Assert(project != null);
            // Debug.Assert(parentFilter != null);
            Debug.Assert(string.IsNullOrEmpty(projectDirectory) == false);
            Debug.Assert(files != null);
            
            // _project = project;
            // _parentFilter = parentFilter;
            _projectDirectory = projectDirectory;
            _files = files;

            _filesByPath = new Dictionary<string, List<FileType>>();

            _buildConfigurations = new List<string>();
            _buildConfigurations.AddRange(buildConfigurations);

            _buildConfigurationsExcluded = new List<string>();
            _buildConfigurationsExcluded.AddRange(buildConfigurationsExcluded);
            
            // FilesByPath
            var result = from file in _files
                               select Path.GetDirectoryName(file.RelativePath)
                ;

            foreach (string directory in result.Distinct())
            {
                _filesByPath.Add(directory, new List<FileType>());
            }

            foreach (FileType file in _files)
            {
                string directory = Path.GetDirectoryName(file.RelativePath);
                _filesByPath[directory].Add(file);
            }
        }

        //public void ExcludeBuildConfiguration(string buildConfiguration)
        //{
        //    _buildConfigurationsExcluded.Add(buildConfiguration);
        //}

        //public void ExcludeBuildConfigurations(IEnumerable<string> buildConfigurations)
        //{
        //    _buildConfigurationsExcluded.AddRange(buildConfigurations);
        //}

        private static string GetNextFileName()
        {
            NumberProvider.Instance.Next();
            return string.Format("UnityBuild-{0}.cpp", NumberProvider.Instance.NoString);
        }

        private static string GetNextFilePath(string directory)
        {
            return Path.Combine(directory, GetNextFileName());
        }

        //private string ProjectDir
        //{
        //    get
        //    {
        //        // return Path.GetDirectoryName(_project.FullPath);
        //        return _projectDirectory;
        //    }
        //}

        private string GetAbsolutePath(string relativePath)
        {
            return Path.GetFullPath(Path.Combine(_projectDirectory, relativePath));
        }

        public List<FileType> Merge()
        {
            if (_files.Count == 0)
                return new List<FileType>();

            List<FileType> newFiles = new List<FileType>(_filesByPath.Keys.Count * 2);

            foreach(string relativeDir in _filesByPath.Keys)
            {                
                FileType newFile = GetNewFile(relativeDir);                
                
                string absolutePath = GetAbsolutePath(newFile.RelativePath);
                using (SrcFileAppend merger = new SrcFileAppend(absolutePath, _projectDirectory, _buildConfigurations, _buildConfigurationsExcluded)) // , true))
                {
                    merger.Open();

                    foreach (var file in _filesByPath[relativeDir])
                    {
                        if (file.IsSrcFile == false)
                            continue;                        

                        merger.AddSrcFile(file);
                    }

                    if(merger.Merge() == false)
                        continue;
                }

                newFiles.Add(newFile);
            }

            foreach (var file in newFiles)
            {
                string key = Path.GetDirectoryName(file.RelativePath);
                _filesByPath[key].Add(file);
                _files.Add(file);
            }

            return newFiles;
        }

        private FileType GetNewFile(string relativeDir)
        {
            FileType newFile = new FileType();
            newFile.RelativePath = GetNextFilePath(relativeDir);
            newFile.RelativePathSpecified = true;

            foreach (string buildConfigurationExcluded in _buildConfigurationsExcluded)
            {
                newFile.ExcludeFromBuild(buildConfigurationExcluded);
            }

            PrecompiledHeaderOptions options = new PrecompiledHeaderOptions(UsePrecompiledHeaderOptions.None);
            foreach (string configurationPlatform in _buildConfigurations)
            {
                newFile.SetPrecompiledHeaderOption(configurationPlatform, options);
            }

            return newFile;
        }
    }
}
