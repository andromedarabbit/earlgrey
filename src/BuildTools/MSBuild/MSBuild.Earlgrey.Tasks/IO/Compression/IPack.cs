using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using Ionic.Zip;
using Ionic.Zlib;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Compression
{
    public interface IPack
    {
        [Required]        
        ITaskItem SrcFolder { get; set; }

        string[] Excludes { get; set; }

        [Required]
        ITaskItem ZipFilePath { get; set; }

        bool Overwrite { get; set; }

        int ZipLevel { get; set; }

        string Password { get; set; }

        bool ContainsRootDir { get; set; }

        bool Execute();
    }
}
