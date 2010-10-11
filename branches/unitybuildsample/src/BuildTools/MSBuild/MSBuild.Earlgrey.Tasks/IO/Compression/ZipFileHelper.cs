using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Ionic.Zip;

namespace MSBuild.Earlgrey.Tasks.IO.Compression
{
    internal class ZipFileHelper
    {
        public static bool HasRootDir(ZipFile zip)
        {
            int numberOfRootFolders = zip.Entries.Count(
                    entry => entry.IsDirectory == true && entry.FileName.Count(ch => ch == '/') == 1
                    );
            int numberOfRootFiles = zip.Entries.Count(
                entry => entry.IsDirectory == false && entry.FileName.Count(ch => ch == '/') == 0
                );

            return numberOfRootFolders == 1
                   && numberOfRootFiles == 0
                   ;
        }

        public static bool HasRootDir(string zipFilePath)
        {
            using (ZipFile zip = ZipFile.Read(zipFilePath))
            {
                return HasRootDir(zip);
            }
        }
    }
}
