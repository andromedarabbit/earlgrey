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
        private readonly FilterType _parentFilter;
        private readonly ICollection<FileType> _files;
        private readonly Dictionary<string, List<FileType>> _filesByPath;

        public FilesMerge(Project project, FilterType parentFilter, ICollection<FileType> files)
        {
            Debug.Assert(project != null);
            Debug.Assert(parentFilter != null);
            Debug.Assert(files != null);
            
            _project = project;
            _parentFilter = parentFilter;
            _files = files;

            _filesByPath = new Dictionary<string, List<FileType>>();

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

        public void Merge()
        {
            if (_files.Count == 0)
                return;

            _parentFilter.ItemsSpecified = true;

            List<FileType> newFiles = new List<FileType>(_filesByPath.Keys.Count * 2);

            foreach(string relativeDir in _filesByPath.Keys)
            {                
                FileType newFile = new FileType();
                newFile.RelativePath = GetNextFilePath(relativeDir);
                newFile.RelativePathSpecified = true;

                _parentFilter.Items.Add(newFile);
                newFiles.Add(newFile);

                string absolutePath = GetAbsolutePath(newFile.RelativePath);

                using (SrcFileAppend merger = new SrcFileAppend(absolutePath, ProjectDir, true))
                {
                    merger.Open();

                    foreach (var file in _filesByPath[relativeDir])
                    {
                        if (file.IsSrcFile == false)
                            continue;

                        merger.MergeSrcFile(file);
                    }
                }                
            }

            foreach (var file in newFiles)
            {
                string key = Path.GetDirectoryName(file.RelativePath);
                _filesByPath[key].Add(file);
                _files.Add(file);
            }
        }
    }
}
