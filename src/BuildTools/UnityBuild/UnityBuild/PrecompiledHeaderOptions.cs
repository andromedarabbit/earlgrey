using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Xml;

namespace UnityBuild
{
    public class PrecompiledHeaderOptions : IEquatable<PrecompiledHeaderOptions>
    {
        private UsePrecompiledHeaderOptions _usePrecompiledHeader;
        private string _precompiledHeaderThrough;
        private string _precompiledHeaderFile;

        public PrecompiledHeaderOptions()
            : this(UsePrecompiledHeaderOptions.None)
        {
        }

        public PrecompiledHeaderOptions(UsePrecompiledHeaderOptions usePrecompiledHeader)
        {
            this._usePrecompiledHeader = usePrecompiledHeader;
            _precompiledHeaderThrough = string.Empty;
            _precompiledHeaderFile = string.Empty;
        }

        public PrecompiledHeaderOptions(PrecompiledHeaderOptions obj)
        {
            this._usePrecompiledHeader = obj._usePrecompiledHeader;
            this._precompiledHeaderThrough = obj._precompiledHeaderThrough;
            this._precompiledHeaderFile = obj._precompiledHeaderFile;
        }

        public UsePrecompiledHeaderOptions UsePrecompiledHeader
        {
            get { return _usePrecompiledHeader; }
            set { _usePrecompiledHeader = value; }
        }

        public string PrecompiledHeaderThrough
        {
            get { return _precompiledHeaderThrough; }
            set { _precompiledHeaderThrough = value; }
        }

        public string PrecompiledHeaderFile
        {
            get { return _precompiledHeaderFile; }
            set { _precompiledHeaderFile = value; }
        }

        #region IEquatable implementations

        public bool Equals(PrecompiledHeaderOptions other)
        {
            if (_usePrecompiledHeader != other._usePrecompiledHeader)
                return false;

            if (_precompiledHeaderThrough.Equals(other._precompiledHeaderThrough, StringComparison.CurrentCultureIgnoreCase) == false)
                return false;

            if (_precompiledHeaderFile.Equals(other._precompiledHeaderFile, StringComparison.CurrentCultureIgnoreCase) == false)
                return false;

            return true;
        }

        #endregion

        public override bool Equals(object obj)
        {
            PrecompiledHeaderOptions options = obj as PrecompiledHeaderOptions;
            if (options == null)
                return false;

            return Equals(options);
        }

        public override int GetHashCode()
        {
            return _usePrecompiledHeader.GetHashCode()
                   & _precompiledHeaderThrough.ToLower().GetHashCode()
                   & _precompiledHeaderFile.ToLower().GetHashCode()
                ;
        }
    }
}