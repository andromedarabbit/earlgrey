﻿using System;
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

        private readonly List<FileType> _srcFiles;


        public SrcFileAppend(string dstFilePath, string projectDir) //, bool deleteZeroSizeFile)
        {
            Debug.Assert(string.IsNullOrEmpty(dstFilePath) == false);
            Debug.Assert(Directory.Exists(projectDir));

            _dstFilePath = dstFilePath;
            _projectDir = projectDir;
            _srcFiles = new List<FileType>();
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
                }

                // Call the appropriate methods to clean up
                // unmanaged resources here.
                // If disposing is false,
                // only the following code is executed.


                // Note disposing has been done.
                _disposed = true;
            }
        }

        #endregion

        public void AddSrcFile(FileType file)
        {
            Debug.Assert(file != null);
            Debug.Assert(file.IsSrcFile);

            _srcFiles.Add(file);
        }

        public bool Merge()
        {
            if (_srcFiles.Count == 0)
                return false;

            IEnumerable<string> srcFileNames = _srcFiles.Select(
                    file => file.FileName
                    );

            if(File.Exists(_dstFilePath))
                File.Delete(_dstFilePath);

            using (StreamWriter sw = new StreamWriter(_dstFilePath, true, Encoding.Default))
            {
                // TODO: 하드코딩
                // sw.WriteLine("#include \"stdafx.h\"");

                foreach (string srcFileName in srcFileNames)
                {
                    sw.WriteLine("#include \"" + srcFileName + "\"");
                }
            }

            return true;
        }
    }

}