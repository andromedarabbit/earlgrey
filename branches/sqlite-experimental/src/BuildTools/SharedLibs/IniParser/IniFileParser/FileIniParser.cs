using System;
using System.IO;
using System.Text;

namespace IniParser
{
    /// <summary>
    /// Represents an INI data parser for files.
    /// </summary>
    public class FileIniDataParser : StreamIniDataParser
    {
        #region Public Methods

        /// <summary>
        /// Implements loading a file from disk.
        /// </summary>
        /// <param name="fileName">Name of the file.</param>
        public IniData LoadFile(string fileName)
        {
            return LoadFile(fileName, false);
        }

        /// <summary>
        /// Implements loading a file from disk.
        /// </summary>
        /// <param name="fileName">Name of the file.</param>
        /// <param name="relaxedIniRead">True to try reading bad formed INI files</param>
        public IniData LoadFile(string fileName, bool relaxedIniRead)
        {
            if (string.IsNullOrEmpty(fileName))
            {
                throw new ArgumentException("Bad filename.");
            }

            try
            {
                using (FileStream fs = File.Open(fileName, FileMode.Open, FileAccess.Read))
                {
                    using (StreamReader sr = CreateStreamReader(fs))
                    {
                        return ReadData(sr, relaxedIniRead);
                    }
                }
            }
            catch (IOException ex)
            {
                throw new ParsingException(String.Format("Could not parse file {0}", fileName), ex);
            }

        }

        /// <summary>
        /// Create a StreamReader instance of the ini file.
        /// </summary>
        /// <param name="fs">FileStream instance of the ini file</param>
        /// <returns>StreamReader instance of the ini file</returns>
        protected virtual StreamReader CreateStreamReader(FileStream fs)
        {
            return new StreamReader(fs);
        }

        /// <summary>
        /// Implements saving a file from disk.
        /// </summary>
        /// <param name="fileName">Name of the file.</param>
        /// <param name="parsedData">IniData to be saved as an INI file.</param>
        public void SaveFile(string fileName, IniData parsedData)
        {
            if (string.IsNullOrEmpty(fileName))
                throw new ArgumentException("Bad filename.");

            if (parsedData == null)
                throw new ArgumentNullException("parsedData");

            try
            {
                using (FileStream fs = File.Open(fileName, FileMode.Create, FileAccess.Write))
                {
                    using (StreamWriter sr = CreateStreamWriter(fs))
                    {
                        WriteData(sr, parsedData);
                    }
                }
            }
            catch (IOException ex)
            {
                throw new ParsingException(String.Format("Could not save data to file {0}", fileName), ex);
            }

        }

        /// <summary>
        /// Create a StreamWriter instance of the ini file.
        /// </summary>
        /// <param name="fs">FileStream instance of the ini file</param>
        /// <returns>StreamWriter instance of the ini file</returns>
        protected virtual StreamWriter CreateStreamWriter(FileStream fs)
        {
            return new StreamWriter(fs);
        }

        #endregion
    }
}
