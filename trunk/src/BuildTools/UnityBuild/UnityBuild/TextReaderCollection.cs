using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{

    internal class TextReaderCollection : IEnumerable<TextReader>, IDisposable
    {
        private readonly TextReaders _internalCollection;

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

}
