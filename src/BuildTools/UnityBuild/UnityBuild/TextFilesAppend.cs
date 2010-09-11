using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild
{
    public class TextFilesAppend
    {
        private readonly string _dstFile;
        private readonly string[] _srcFiles;

        public TextFilesAppend(string dstFile, params string[] srcFiles)
        {
            Debug.Assert(string.IsNullOrEmpty(dstFile) == false);
            // Debug.Assert(src string.IsNullOrEmpty(dstFile) == false);


            _dstFile = dstFile;
            _srcFiles = srcFiles;
        }


    }
}
