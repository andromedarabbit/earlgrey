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
        public static string FindFirst(string fileName)
        {
            if (File.Exists(fileName))
                return fileName;

            string pathFromEnv = Environment.GetEnvironmentVariable("PATH");
            if (string.IsNullOrEmpty(pathFromEnv))
                throw new FileNotFoundException();

            string[] paths = pathFromEnv.Split(';');
            foreach (var path in paths)
            {
                if (string.IsNullOrEmpty(path))
                    continue;

                string fullPath = Path.Combine(path, fileName);
                if (File.Exists(fullPath))
                    return fullPath;
            }

            throw new FileNotFoundException();
        }

        public static string FindFirst(string rootDir, string pattern)
        {
            foreach (string dir in Directory.GetDirectories(rootDir, "*", SearchOption.AllDirectories))
            {
                foreach (string file in Directory.GetFiles(dir, pattern))
                {
                    return file;
                }
            }
            throw new FileNotFoundException();
        }


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

        // Simple way to find out if the file exists.
        public static bool Exists(string fileName)
        {
            try
            {
                FindFirst(fileName);
                return true;
            }
            catch (FileNotFoundException)
            {
                return false;
            }

        }

    }
}