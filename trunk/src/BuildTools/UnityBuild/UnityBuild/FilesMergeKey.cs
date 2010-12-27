using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    class FilesMergeKey : IEquatable<FilesMergeKey>
    {
        private readonly string _relativeDir;
        private readonly string _precompileHeaderThrough;

        public FilesMergeKey(string relativeDir, string precompileHeaderThrough)
        {
            Debug.Assert(string.IsNullOrEmpty(relativeDir) == false);
            Debug.Assert(string.IsNullOrEmpty(precompileHeaderThrough) == false);

            _relativeDir = relativeDir;
            _precompileHeaderThrough = precompileHeaderThrough;
        }

        public string RelativeDir
        {
            get { return _relativeDir; }
        }

        public string PrecompileHeaderThrough
        {
            get { return _precompileHeaderThrough; }
        }

        public bool Equals(FilesMergeKey other)
        {
            if (other == null)
                return false;

            if (this._relativeDir.Equals(other._relativeDir, StringComparison.CurrentCultureIgnoreCase) == false)
                return false;

            if (this._precompileHeaderThrough.Equals(other._precompileHeaderThrough, StringComparison.CurrentCultureIgnoreCase) == false)
                return false;

            return true;
        }

        public override bool Equals(object obj)
        {
            FilesMergeKey key = obj as FilesMergeKey;
            if(key == null)
                return false;

            return Equals(key);
        }

        public override int GetHashCode()
        {
            return _relativeDir.ToLower().GetHashCode()
                   & _precompileHeaderThrough.ToLower().GetHashCode()
                ;
        }

      
    }
}

