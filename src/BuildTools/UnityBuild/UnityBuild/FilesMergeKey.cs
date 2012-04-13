using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class FilesMergeKey : IEquatable<FilesMergeKey>
    {
        private readonly string _relativeDir;
        private readonly List<KeyValuePair<string, PrecompiledHeaderOptions>> _precompiledHeaderOptions;

        public FilesMergeKey(IFileType file)
            : this(file.RelativeDir, file.GetPrecompiledHeaderOptions())
        {
        }

        public FilesMergeKey(string relativeDir, IDictionary<string, PrecompiledHeaderOptions> precompiledHeaderOptions)
        {
            Debug.Assert(string.IsNullOrEmpty(relativeDir) == false);

            _relativeDir = relativeDir;

            _precompiledHeaderOptions =
                new List<KeyValuePair<string, PrecompiledHeaderOptions>>(precompiledHeaderOptions);
        }

        public string RelativeDir
        {
            get { return _relativeDir; }
        }

        public IList<KeyValuePair<string, PrecompiledHeaderOptions>> PrecompiledHeaderOptions
        {
            get { return _precompiledHeaderOptions; }
        }

        public void AddPrecompiledHeaderOptions(string configurationPlatform, PrecompiledHeaderOptions options)
        {
            _precompiledHeaderOptions.Add(
                new KeyValuePair<string, PrecompiledHeaderOptions>(configurationPlatform, options)
                );
        }

        public bool Equals(FilesMergeKey other)
        {
            if (other == null)
                return false;

            if (this._relativeDir.Equals(other._relativeDir, StringComparison.CurrentCultureIgnoreCase) == false)
                return false;

            if (_precompiledHeaderOptions.SequenceEqual(other._precompiledHeaderOptions) == false)
                return false;

            return true;
        }

        public override bool Equals(object obj)
        {
            FilesMergeKey key = obj as FilesMergeKey;
            if (key == null)
                return false;

            return Equals(key);
        }

        public override int GetHashCode()
        {
            int hashCode = _relativeDir.ToLower().GetHashCode();

            foreach (KeyValuePair<string, PrecompiledHeaderOptions> precompiledHeaderOption in _precompiledHeaderOptions
                )
            {
                hashCode = hashCode
                           & precompiledHeaderOption.Key.ToLower().GetHashCode()
                           & precompiledHeaderOption.Value.GetHashCode();
            }

            return hashCode;
        }
    }
}