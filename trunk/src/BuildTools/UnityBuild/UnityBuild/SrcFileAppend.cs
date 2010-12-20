using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    internal class SrcFileAppend : IDisposable
    {
        private readonly string _dstFilePath;
        private readonly string _projectDir;
        private FileStream _dstFileStream;
        private readonly bool _deleteZeroSizeFile;

        public SrcFileAppend(string dstFilePath, string projectDir)
            : this(dstFilePath, projectDir, true)
        {

        }

        public SrcFileAppend(string dstFilePath, string projectDir, bool deleteZeroSizeFile)
        {
            Debug.Assert(string.IsNullOrEmpty(dstFilePath) == false);
            Debug.Assert(Directory.Exists(projectDir));

            _dstFilePath = dstFilePath;
            _projectDir = projectDir;
            _dstFileStream = null;
            _deleteZeroSizeFile = deleteZeroSizeFile;
        }

        public string DstFilePath
        {
            get { return _dstFilePath; }
        }

        public string ProjectDir                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
        {
            get { return _projectDir; }
        }

        public void Open()
        {
            Debug.Assert(Directory.Exists(_projectDir));

            _dstFileStream = File.Open(_dstFilePath, FileMode.Create);
        }

        public bool IsOpen
        {
            get { return _dstFileStream != null; }
        }

        public bool DeleteZeroSizeFile
        {
            get { return _deleteZeroSizeFile; }
        }

        #region IDisposable

        // Track whether Dispose has been called.
        private bool _disposed = false;

        // Implement IDisposable.
        // Do not make this method virtual.
        // A derived class should not be able to override this method.
        public void Dispose()
        {
            Dispose(true);
            // This object will be cleaned up by the Dispose method.
            // Therefore, you should call GC.SupressFinalize to
            // take this object off the finalization queue
            // and prevent finalization code for this object
            // from executing a second time.
            GC.SuppressFinalize(this);
        }

        // Dispose(bool disposing) executes in two distinct scenarios.
        // If disposing equals true, the method has been called directly
        // or indirectly by a user's code. Managed and unmanaged resources
        // can be disposed.
        // If disposing equals false, the method has been called by the
        // runtime from inside the finalizer and you should not reference
        // other objects. Only unmanaged resources can be disposed.
        private void Dispose(bool disposing)
        {
            // Check to see if Dispose has already been called.
            if (!this._disposed)
            {
                // If disposing equals true, dispose all managed
                // and unmanaged resources.
                if (disposing)
                {
                    // Dispose managed resources.
                    DisposeDstFileStream();
                }

                // Call the appropriate methods to clean up
                // unmanaged resources here.
                // If disposing is false,
                // only the following code is executed.


                // Note disposing has been done.
                _disposed = true;
            }
        }

        private void DisposeDstFileStream()
        {
            if (_dstFileStream == null)
                return;

            _dstFileStream.Dispose();


            if (_deleteZeroSizeFile == false)
                return;

            FileInfo fileInfo = new FileInfo(_dstFilePath);
            if (fileInfo.Exists == false)
                return;

            if (fileInfo.Length > 0)
                return;

            File.Delete(_dstFilePath);
        }

        #endregion

        public void MergeSrcFile(FileType file)
        {
            Debug.Assert(file != null);
            Debug.Assert(file.IsSrcFile);

            string filePath = Path.GetFullPath(Path.Combine(_projectDir, file.RelativePath));
            Debug.Assert(File.Exists(filePath));

            using (FileStream src = File.Open(filePath, FileMode.Open))
            {
                StreamAppend append = new StreamAppend(src, _dstFileStream);
                append.Merge();
            }
        }
    }

}
