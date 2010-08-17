using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace MSBuild.Earlgrey
{
    public static class FileSearch
    {
        //! \todo 이 기능을 여기저기서 쓰니 따로 정리하자.
        public static List<string> Search(string rootDir, string pattern)
        {
            List<string> files = new List<string>();
            Search(rootDir, pattern, files);
            return files;
        }

        private static void Search(string rootDir, string pattern, List<string> files)
        {
            foreach (string dir in Directory.GetDirectories(rootDir))
            {
                foreach (string file in Directory.GetFiles(dir, pattern))
                {
                    files.Add(file);
                }
                Search(dir, pattern, files);
            }
        }
    }
}
