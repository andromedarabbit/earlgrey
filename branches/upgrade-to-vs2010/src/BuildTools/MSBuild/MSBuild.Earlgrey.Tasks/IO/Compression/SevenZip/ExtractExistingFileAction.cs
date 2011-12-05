using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.SevenZip
{
    internal enum ExtractExistingFileAction
    {
        OverwriteAll
        , Skip
        , AutoRenameExtractingFile
        , AutoRenameExistingFile
    }
}
