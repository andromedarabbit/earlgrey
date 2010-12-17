using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class FilesMerge
    {
        private readonly IEnumerable<FileType> _files;
        private readonly Dictionary<string, FileType> _filesByPath;

        public FilesMerge(IEnumerable<FileType> files)
        {
            Debug.Assert(_files != null);
            _files = files;
            _filesByPath = _files.ToDictionary(item => item.RelativePath);
        }

        public void Merge()
        {

        }
    }
}
