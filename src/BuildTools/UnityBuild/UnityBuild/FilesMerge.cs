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
        private readonly VisualStudioVersions _version;
        private readonly string _projectDirectory;
        private readonly ICollection<IFileType> _files;

        private readonly List<string> _buildConfigurations;
        private readonly List<string> _buildConfigurationsExcluded;

        private readonly int _maxFilesPerFile;

        public FilesMerge(
            VisualStudioVersions version,
            string projectDirectory,
            ICollection<IFileType> files,
            IEnumerable<string> buildConfigurations
            )
            : this(version, projectDirectory, files, buildConfigurations, new List<string>(), 0)
        {
        }

        public FilesMerge(
            VisualStudioVersions version,
            string projectDirectory,
            ICollection<IFileType> files,
            IEnumerable<string> buildConfigurations,
            IEnumerable<string> buildConfigurationsExcluded
            )
            : this(version, projectDirectory, files, buildConfigurations, buildConfigurationsExcluded, 0)
        {
        }

        public FilesMerge(
            VisualStudioVersions version,
            string projectDirectory,
            ICollection<IFileType> files,
            IEnumerable<string> buildConfigurations,
            IEnumerable<string> buildConfigurationsExcluded,
            int maxFilesPerFile
            )
        {
            Debug.Assert(version != VisualStudioVersions.None);
            Debug.Assert(string.IsNullOrEmpty(projectDirectory) == false);
            Debug.Assert(files != null);
            Debug.Assert(maxFilesPerFile >= 0);

            _version = version;
            _projectDirectory = projectDirectory;
            _files = files;

            _buildConfigurations = new List<string>();
            _buildConfigurations.AddRange(buildConfigurations);

            _buildConfigurationsExcluded = new List<string>();
            _buildConfigurationsExcluded.AddRange(buildConfigurationsExcluded);

            _maxFilesPerFile = maxFilesPerFile;
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

        public List<IFileType> Merge()
        {
            if (_files.Count == 0)
                return new List<IFileType>();

            IEnumerable<KeyValuePair<FilesMergeKey, IFileType>> keyValues
                = _files.Select(file => new KeyValuePair<FilesMergeKey, IFileType>(new FilesMergeKey(file), file));

            IEnumerable<IGrouping<FilesMergeKey, KeyValuePair<FilesMergeKey, IFileType>>> filesByPaths
                = keyValues.GroupBy(pair => pair.Key);


            List<IFileType> newFiles = new List<IFileType>();

            foreach (IGrouping<FilesMergeKey, KeyValuePair<FilesMergeKey, IFileType>> filesByPath in filesByPaths)
            {
                int numberOfFiles = _maxFilesPerFile;
                if (_maxFilesPerFile == 0)
                    numberOfFiles = filesByPath.Count();

                var fileGroup = GetEnumerableOfEnumerables(filesByPath, numberOfFiles);
                foreach (IEnumerable<KeyValuePair<FilesMergeKey, IFileType>> item in fileGroup)
                {
                    MergeFiles(item, newFiles);
                }
            }

            foreach (var file in newFiles)
            {
                _files.Add(file);
            }

            return newFiles;
        }

        private void MergeFiles(IEnumerable<KeyValuePair<FilesMergeKey, IFileType>> filesByPath, List<IFileType> newFiles)
        {
            if (filesByPath.Count() == 0)
                return;

            FilesMergeKey key = filesByPath.First().Key;
            string relativeDir = key.RelativeDir;
            IFileType newFile = GetNewFile(relativeDir, key.PrecompiledHeaderOptions);

            string absolutePath = GetAbsolutePath(newFile.RelativePath);
            using (
                SrcFileAppend merger = new SrcFileAppend(key.PrecompiledHeaderOptions, absolutePath, _projectDirectory, _buildConfigurations, _buildConfigurationsExcluded)
                )
            {
                merger.Open();

                foreach (KeyValuePair<FilesMergeKey, IFileType> fileByPath in filesByPath)
                {
                    IFileType file = fileByPath.Value;
                    if (file.IsSrcFile == false)
                        continue;

                    merger.AddSrcFile(file);
                }

                if (merger.Merge() == false)
                    return;
            }

            newFiles.Add(newFile);
        }

        private static IEnumerable<IEnumerable<T>> GetEnumerableOfEnumerables<T>(IEnumerable<T> enumerable, int groupSize)
        {
            // The list to return.
            List<T> list = new List<T>(groupSize);

            // Cycle through all of the items.
            foreach (T item in enumerable)
            {
                // Add the item.
                list.Add(item);

                // If the list has the number of elements, return that.
                if (list.Count == groupSize)
                {
                    // Return the list.
                    yield return list;

                    // Set the list to a new list.
                    list = new List<T>(groupSize);
                }
            }

            // Return the remainder if there is any,
            if (list.Count != 0)
            {
                // Return the list.
                yield return list;
            }
        }

        private IFileType GetNewFile(string relativeDir, IEnumerable<KeyValuePair<string, PrecompiledHeaderOptions>> options)
        {
            // FileType newFile = new FileType();
            // newFile.RelativePath = GetNextFilePath(relativeDir);
            // newFile.RelativePathSpecified = true;
            IFileType newFile = FileTypeFactory.CreateInstance(_version, GetNextFilePath(relativeDir));

            foreach (string buildConfigurationExcluded in _buildConfigurationsExcluded)
            {
                newFile.ExcludeFromBuild(buildConfigurationExcluded);
            }

            foreach (KeyValuePair<string, PrecompiledHeaderOptions> keyValuePair in options)
            {
                if (keyValuePair.Value.UsePrecompiledHeader == UsePrecompiledHeaderOptions.Create)
                {
                    PrecompiledHeaderOptions newOptions = new PrecompiledHeaderOptions(keyValuePair.Value);
                    newOptions.UsePrecompiledHeader = UsePrecompiledHeaderOptions.Use;
                    continue;
                }
                newFile.SetPrecompiledHeaderOption(keyValuePair.Key, keyValuePair.Value);
            }

            return newFile;
        }
    }
}