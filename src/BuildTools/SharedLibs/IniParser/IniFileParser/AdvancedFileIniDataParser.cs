using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Earlgrey.EncodingTools;

namespace IniParser
{
    /// <summary>
    /// Same as FileIniDataParser exception this has ability to detemine encoding of the ini file automatically
    /// </summary>
    public class AdvancedFileIniDataParser : FileIniDataParser
    {
        private Encoding _fileEncoding;

        #region Public Methods

        /// <summary>
        /// Default construct
        /// </summary>
        public AdvancedFileIniDataParser()
            : base()
        {
            _fileEncoding = Encoding.Default;
        }

        /// <summary>
        /// Encoding type of the ini file
        /// </summary>
        public Encoding FileEncoding
        {
            get { return _fileEncoding; }
            set { _fileEncoding = value; }
        }

        /// <summary>
        /// Create a StreamReader instance of the ini file.
        /// </summary>
        /// <param name="fs">FileStream instance of the ini file</param>
        /// <returns>StreamReader instance of the ini file</returns>
        protected override StreamReader CreateStreamReader(FileStream fs)
        {
            StreamReader sr = EncodingHelper.OpenTextStream(fs);
            _fileEncoding = sr.CurrentEncoding;
            return sr;
        }

        /// <summary>
        /// Create a StreamWriter instance of the ini file.
        /// </summary>
        /// <param name="fs">FileStream instance of the ini file</param>
        /// <returns>StreamWriter instance of the ini file</returns>
        protected override StreamWriter CreateStreamWriter(FileStream fs)
        {
            return new StreamWriter(fs, _fileEncoding);
        }
       

        #endregion
    }
}
