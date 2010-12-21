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
        private readonly Project _project;
        // private readonly FilterType _parentFilter;
        private readonly ICollection<FileType> _files;
        private readonly Dictionary<string, List<FileType>> _filesByPath;

        private readonly List<string> _buildConfigurationExcluded;

        // public FilesMerge(Project project, FilterType parentFilter, ICollection<FileType> files)
        public FilesMerge(Project project, ICollection<FileType> files)
        {
            Debug.Assert(project != null);
            // Debug.Assert(parentFilter != null);
            Debug.Assert(files != null);
            
            _project = project;
            // _parentFilter = parentFilter;
            _files = files;

            _filesByPath = new Dictionary<string, List<FileType>>();

            _buildConfigurationExcluded = new List<string>();
            
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

        public void ExcludeBuildConfiguration(string buildConfiguration)
        {
            _buildConfigurationExcluded.Add(buildConfiguration);
        }

        public void ExcludeBuildConfigurations(IEnumerable<string> buildConfigurations)
        {
            _buildConfigurationExcluded.AddRange(buildConfigurations);
        }

        private string GetNextFileName()
        {
            NumberProvider.Instance.Next();
            return string.Format("UnityBuild-{0}.cpp", NumberProvider.Instance.NoString);
        }

        private string GetNextFilePath(string directory)
        {
            return Path.Combine(directory, GetNextFileName());
        }

        private string ProjectDir
        {
            get
            {
                return Path.GetDirectoryName(_project.FullPath);
            }
        }

        private string GetAbsolutePath(string relativePath)
        {
            return Path.GetFullPath(Path.Combine(ProjectDir, relativePath));
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
                using (SrcFileAppend merger = new SrcFileAppend(absolutePath, ProjectDir)) // , true))
                {
                    merger.Open();

                    foreach (var file in _filesByPath[relativeDir])
                    {
                        if (file.IsSrcFile == false)
                            continue;

                        //IEnumerable<string> buildConfigurations = file.BuildConfigurationsWhenExcludedFromBuild;
                        //if (buildConfigurations.Count() > 0)
                        //{

                        //    continue;
                        //}

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

            foreach (string buildConfigurationExcluded in _buildConfigurationExcluded)
            {
                newFile.ExcludeFromBuild(buildConfigurationExcluded);
            }
            return newFile;
        }
    }
}
