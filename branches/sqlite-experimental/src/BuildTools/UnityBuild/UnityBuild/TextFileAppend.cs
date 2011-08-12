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

            _dstFile = dstFile;
            _srcFiles = srcFiles;

            CreateEmptyDstFileIfNoSrcFileSpecified = false;
        }

        public bool CreateEmptyDstFileIfNoSrcFileSpecified { get; set; }

        public string Delimiter { get; set; }


        public void Merge()
        {
            if ((_srcFiles == null || _srcFiles.Length == 0) && (CreateEmptyDstFileIfNoSrcFileSpecified == false))
            {
                return;
            }

            const int bufferSize = 1024*1024; // TODO: 임시
            char[] buffer = new char[bufferSize];

            using (var sw = new StreamWriter(_dstFile.FilePath, true, _dstFile.FileEncoding, bufferSize))
            {
                using (TextReaderCollection readers = new TextReaderCollection(_srcFiles, Delimiter))
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