using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Earlgrey.EncodingTools;

namespace IniParser
{
    public class AdvancedFileIniDataParser : FileIniDataParser
    {
        private Encoding _fileEncoding;

        #region Public Methods

        public AdvancedFileIniDataParser()
            : base()
        {
            _fileEncoding = Encoding.Default;
        }

        public Encoding FileEncoding
        {
            get { return _fileEncoding; }
            set { _fileEncoding = value; }
        }

        protected override StreamReader CreateStreamReader(FileStream fs)
        {
            StreamReader sr = EncodingHelper.OpenTextStream(fs);
            _fileEncoding = sr.CurrentEncoding;
            return sr;
        }

        protected override StreamWriter CreateStreamWriter(FileStream fs)
        {
            return new StreamWriter(fs, _fileEncoding);
        }
       

        #endregion
    }
}
