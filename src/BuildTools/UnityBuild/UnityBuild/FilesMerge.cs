using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class FilesMerge
    {
        private readonly string _projectDirectory;
        private readonly ICollection<FileType> _files;
        
        private readonly List<string> _buildConfigurations;
        private readonly List<string> _buildConfigurationsExcluded;

        public FilesMerge(string projectDirectory, ICollection<FileType> files, IEnumerable<string> buildConfigurations)
            : this(projectDirectory, files, buildConfigurations, new List<string>())
        {
            
        }

        public FilesMerge(string projectDirectory, ICollection<FileType> files, IEnumerable<string> buildConfigurations, IEnumerable<string> buildConfigurationsExcluded)
        {
            Debug.Assert(string.IsNullOrEmpty(projectDirectory) == false);
            Debug.Assert(files != null);
            
            _projectDirectory = projectDirectory;
            _files = files;

            _buildConfigurations = new List<string>();
            _buildConfigurations.AddRange(buildConfigurations);

            _buildConfigurationsExcluded = new List<string>();
            _buildConfigurationsExcluded.AddRange(buildConfigurationsExcluded);
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

        private string GetAbsolutePath(string relativePath)
        {
            return Path.GetFullPath(Path.Combine(_projectDirectory, relativePath));
        }

        public List<FileType> Merge()
        {
            if (_files.Count == 0)
                return new List<FileType>();


            IEnumerable<IGrouping<string, FileType>> filesByPaths = _files.GroupBy(file => file.RelativeDir);

            List<FileType> newFiles = new List<FileType>();
            foreach (IGrouping<string, FileType> filesByPath in filesByPaths)
            {
                // FileType newFile = GetNewFile(relativeDir);                
                string relativeDir = filesByPath.Key;
                FileType newFile = GetNewFile(relativeDir);
                
                string absolutePath = GetAbsolutePath(newFile.RelativePath);
                using (SrcFileAppend merger = new SrcFileAppend(absolutePath, _projectDirectory, _buildConfigurations, _buildConfigurationsExcluded))
                {
                    merger.Open();

                    foreach (FileType file in filesByPath)
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
