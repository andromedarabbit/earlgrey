using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Compression
{
    interface IUnpack
    {
        [Required]
        ITaskItem TargetDir { get; set; }

        bool Overwrite { get; set; }

        [Required]
        ITaskItem ZipFilePath { get; set; }

        string Password { get; set; }

        bool Execute();
    }
}
