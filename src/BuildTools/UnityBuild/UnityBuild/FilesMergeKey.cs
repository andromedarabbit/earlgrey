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
        private readonly List<KeyValuePair<string, PrecompiledHeaderOptions>> _precompiledHeaderOptions;
        // private readonly Dictionary<string, PrecompiledHeaderOptions> _precompiledHeaderOptions;
        // private readonly string _precompileHeaderThrough;

        public FilesMergeKey(FileType file)
            : this(file.RelativeDir, file.GetPrecompiledHeaderOptions())
        {
            
        }

        // public FilesMergeKey(string relativeDir, string precompileHeaderThrough)
        public FilesMergeKey(string relativeDir, IDictionary<string, PrecompiledHeaderOptions> precompiledHeaderOptions)
        {
            Debug.Assert(string.IsNullOrEmpty(relativeDir) == false);
            // Debug.Assert(string.IsNullOrEmpty(precompileHeaderThrough) == false);
            // Debug.Assert(precompiledHeaderOptions != null);

            _relativeDir = relativeDir;
            // _precompileHeaderThrough = precompileHeaderThrough;
            // _precompiledHeaderOptions =  new Dictionary<string, PrecompiledHeaderOptions>(precompiledHeaderOptions);
            _precompiledHeaderOptions = 
                new List<KeyValuePair<string, PrecompiledHeaderOptions>>(precompiledHeaderOptions);
            
        }

        public string RelativeDir
        {
            get { return _relativeDir; }
        }

        /*
        public string PrecompileHeaderThrough
        {
            get { return _precompileHeaderThrough; }
        }
         * */
        // public IDictionary<string, PrecompiledHeaderOptions> PrecompiledHeaderOptions
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

            // if (this._precompileHeaderThrough.Equals(other._precompileHeaderThrough, StringComparison.CurrentCultureIgnoreCase) == false)
                // return false;
            // if(_precompiledHeaderOptions.SequenceEqual(other._precompiledHeaderOptions) == false)
                // return false;
            /*
            IOrderedEnumerable<KeyValuePair<string, PrecompiledHeaderOptions>> thisOrdered = 
                _precompiledHeaderOptions.OrderBy(keyValue => keyValue.Key);

            IOrderedEnumerable<KeyValuePair<string, PrecompiledHeaderOptions>> thatOrdered =
                other._precompiledHeaderOptions.OrderBy(keyValue => keyValue.Key);
             * */

            if (_precompiledHeaderOptions.SequenceEqual(other._precompiledHeaderOptions) == false)
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
            int hashCode = _relativeDir.ToLower().GetHashCode();

            foreach (KeyValuePair<string, PrecompiledHeaderOptions> precompiledHeaderOption in _precompiledHeaderOptions)
            {
                hashCode = hashCode 
                    & precompiledHeaderOption.Key.ToLower().GetHashCode() 
                    & precompiledHeaderOption.Value.GetHashCode();
            }

            return hashCode;
        }

      
    }
}

