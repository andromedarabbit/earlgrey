using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.IO;
using Earlgrey.EncodingTools;

namespace UnityBuild
{
    public class TextFile
    {
        private string _filePath;
        private Encoding _fileEncoding;

        private static readonly Encoding DefaultEncoding;

        static TextFile()
        {
            DefaultEncoding = Encoding.UTF8;
        }

        public TextFile()
            : this(string.Empty, null)
        {
        }

        public TextFile(string filePath)
            : this(filePath, null)
        {

        }

        public TextFile(string filePath, Encoding fileEncoding)
        {
            _filePath = filePath;
            _fileEncoding = fileEncoding;
        }

        public string FilePath
        {
            get { return _filePath; }
            set { _filePath = value; }
        }

        private static Encoding DetectFIleEncoding(string filePath)
        {
            using (Stream stream = File.Open(filePath, FileMode.Open, FileAccess.Read, FileShare.Read))
            {
                stream.Seek(0, SeekOrigin.Begin);

                // buffer for preamble and up to 512b sample text for dection
                byte[] buf = new byte[System.Math.Min(stream.Length, 512)];

                stream.Read(buf, 0, buf.Length);
                return EncodingHelper.DetectInputCodepage(buf);
            }
        }

        public Encoding FileEncoding
        {
            get
            {
                if (_fileEncoding != null)
                    return _fileEncoding;

                if (File.Exists(_filePath))
                {
                    _fileEncoding = DetectFIleEncoding(FilePath);
                    return _fileEncoding;
                }

                _fileEncoding = DefaultEncoding;

                return _fileEncoding;
            }
            set
            {
                _fileEncoding = value;
            }
        }

        //public bool IsValid()
        //{
        //    if(string.IsNullOrEmpty(_filePath))
        //        return false;

        //    if(File.Exists(_filePath) == false)
        //        return false;

        //    // Debug.Assert(_fileEncoding != null);

        //    return true;
        //}

        public TextReader GetTextReader()
        {
            return new StreamReader(FilePath, FileEncoding, true);
        }
    }
}
