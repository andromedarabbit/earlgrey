using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;

namespace UnityBuild
{

    public class TextFileAppend
    {
        private readonly TextFile _dstFile;
        private readonly TextFile[] _srcFiles;

        public TextFileAppend(TextFile dstFile, params TextFile[] srcFiles)
        {
            Debug.Assert(dstFile != null);
            // Debug.Assert(srcFiles != null);

            _dstFile = dstFile;
            _srcFiles = srcFiles;

            CreateEmptyDstFileIfNoSrcFileSpecified = false;
        }

        public bool CreateEmptyDstFileIfNoSrcFileSpecified { get; set; }

        public string Delimiter
        {
            get;
            set;
        }

        class TextReaders : IEnumerator<TextReader>
        {
            private readonly TextReader[] _srcFiles;
            private readonly string _delimiter;
            private int _count;

            public TextReaders(TextFile[] srcFiles, string delimiter)
            {
                Debug.Assert(srcFiles != null);

                _srcFiles = srcFiles.Select(file => file.GetTextReader()).ToArray();
                this._delimiter = delimiter;
                this._count = -1;
            }

            public TextReader Current
            {
                get
                {
                    if(_delimiter == null)
                        return _srcFiles[_count];

                    if(_count % 2 == 0)
                        return _srcFiles[_count / 2];
                    
                    return new StringReader(_delimiter);
                }
            }
            
            object System.Collections.IEnumerator.Current 
            { 
                get
                {
                    throw new NotImplementedException();
                }
            }

            public bool MoveNext()
            {
                if (_delimiter == null && _srcFiles.Length <= _count + 1)
                    return false;

                if(_delimiter != null && _srcFiles.Length <= (_count / 2) + 1)
                    return false;

                _count++;
                return true;
            }

            public void Reset()
            {
                _count = 0;
            }

            public void Dispose()
            {              
                foreach(var file in _srcFiles)
                {
                    file.Dispose();
                }
            }

        }

        class TextReaderCollection : IEnumerable<TextReader>, IDisposable
        {
            private TextReaders _internalCollection;

            public TextReaderCollection(TextFile[] srcFiles, string delimiter)
            {
                _internalCollection = new TextReaders(srcFiles, delimiter);
            }

            public IEnumerator<TextReader> GetEnumerator()
            {
                return _internalCollection;
            }

            System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
            {
                throw new NotImplementedException();
            }

            public void Dispose()
            {
                _internalCollection.Dispose();
            }
        }

        public void Merge()
        {
            if(_srcFiles == null && _srcFiles.Length == 0 && CreateEmptyDstFileIfNoSrcFileSpecified == false)
            {
                return;
            }

            int bufferSize = 1024*1024; // TODO: 임시
            char[] buffer = new char[bufferSize];

            using(var sw = new StreamWriter(_dstFile.FilePath, true, _dstFile.FileEncoding, bufferSize))
            {
                using(TextReaderCollection readers = new TextReaderCollection(_srcFiles, Delimiter))
                {
                    foreach (var reader in readers)
                    {
                        int bytesRead = reader.ReadBlock(buffer, 0, buffer.Length);
                        sw.Write(buffer, 0, bytesRead);
                    }
                }
            }
        }


    }
}
