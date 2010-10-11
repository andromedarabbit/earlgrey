using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.SevenZip
{
    internal enum ExitCode
    {
        NoError = 0,
        WarningNonFataError = 1, // For example, one or more files were locked by some other application, so they were not compressed. 
        FatalError = 2, 
        CommandLineError = 7,
        NotEnoughMemoryForOperation = 8,
        UserStoppedTheProcess = 255
    }
}
