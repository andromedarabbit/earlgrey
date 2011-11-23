using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.SevenZip
{
    /// <summary>
    /// Describes the compression rate.
    /// </summary>
    /// <remarks>Higher number, higher compression rate.</remarks>
    public enum CompressionLevel
    {
        Level0 = 0,
        None = 0,
        Level1 = 1,
        BestSpeed = 1,        
        Level3 = 3,
        Level5 = 5,
        Default = 5,
        Level7 = 7,
        BestCompression = 9,
        Level9 = 9,
    }
}
