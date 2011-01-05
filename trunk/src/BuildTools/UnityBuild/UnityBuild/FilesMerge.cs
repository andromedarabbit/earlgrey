using System;
using System.Collections;
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
            // _keys = new List<FilesMergeKey>(_files.Count);

            _buildConfigurations = new List<string>();
            _buildConfigurations.AddRange(buildConfigurations);

            _buildConfigurationsExcluded = new List<string>();
            _buildConfigurationsExcluded.AddRange(buildConfigurationsExcluded);
        }

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

            IEnumerable<KeyValuePair<FilesMergeKey, FileType>> keyValues
                = _files.Select(file => new KeyValuePair<FilesMergeKey, FileType>(new FilesMergeKey(file), file));

            IEnumerable<IGrouping<FilesMergeKey, KeyValuePair<FilesMergeKey, FileType>>> filesByPaths
                = keyValues.GroupBy(pair => pair.Key);


            List<FileType> newFiles = new List<FileType>();
            foreach (IGrouping<FilesMergeKey, KeyValuePair<FilesMergeKey, FileType>> filesByPath in filesByPaths)
            {
                FilesMergeKey key = filesByPath.Key;           
                string relativeDir = key.RelativeDir;
                FileType newFile = GetNewFile(relativeDir, key.PrecompiledHeaderOptions);
                
                string absolutePath = GetAbsolutePath(newFile.RelativePath);
                using (SrcFileAppend merger = new SrcFileAppend(key.PrecompiledHeaderOptions, absolutePath, _projectDirectory, _buildConfigurations, _buildConfigurationsExcluded))
                {
                    merger.Open();

                    foreach (KeyValuePair<FilesMergeKey, FileType> fileByPath in filesByPath)
                    {
                        FileType file = fileByPath.Value;
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

        private FileType GetNewFile(string relativeDir, IList<KeyValuePair<string, PrecompiledHeaderOptions>> options)
        {
            FileType newFile = new FileType();
            newFile.RelativePath = GetNextFilePath(relativeDir);
            newFile.RelativePathSpecified = true;

            foreach (string buildConfigurationExcluded in _buildConfigurationsExcluded)
            {
                newFile.ExcludeFromBuild(buildConfigurationExcluded);
            }

            foreach (KeyValuePair<string, PrecompiledHeaderOptions> keyValuePair in options)
            {
                newFile.SetPrecompiledHeaderOption(keyValuePair.Key, keyValuePair.Value);
            }

            return newFile;
        }
    }
}
