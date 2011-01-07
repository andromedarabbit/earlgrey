using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class TextReaders : IEnumerator<TextReader>
    {
        private readonly TextReader[] _srcFiles;
        private readonly string _delimiter;
        private int _count;

        public TextReaders(IEnumerable<TextFile> srcFiles, string delimiter)
        {
            Debug.Assert(srcFiles != null);

            this._srcFiles = srcFiles.Select(file => file.GetTextReader()).ToArray();
            this._delimiter = delimiter;
            this._count = -1;
        }

        public TextReader Current
        {
            get
            {
                Debug.Assert(_srcFiles.Length > 0);

                if (_delimiter == null)
                    return _srcFiles[_count];

                if (_count%2 == 0)
                    return _srcFiles[_count/2];

                return new StringReader(_delimiter);
            }
        }

        object System.Collections.IEnumerator.Current
        {
            get { throw new NotImplementedException(); }
        }

        // TODO: 땜빵
        public bool MoveNext()
        {
            if (_srcFiles.Length == 0)
                return false;

            if (string.IsNullOrEmpty(_delimiter) == true && _srcFiles.Length <= (_count + 1))
                return false;

            if (string.IsNullOrEmpty(_delimiter) == false)
            {
                if (_count >= (_srcFiles.Length*2 - 1) - 1)
                    return false;
            }

            _count++;
            return true;
        }

        public void Reset()
        {
            _count = -1;
        }

        public void Dispose()
        {
            foreach (var file in _srcFiles)
            {
                file.Dispose();
            }
        }
    }
}