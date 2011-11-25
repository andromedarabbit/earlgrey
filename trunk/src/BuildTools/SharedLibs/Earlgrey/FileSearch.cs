using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace Earlgrey
{
    public static class FileSearch
    {
        private static IEnumerable<string> DefaultSearchFolders
        {
            get 
            { 
                List<string> folders = new List<string>();
                folders.Add(AppDomain.CurrentDomain.BaseDirectory);
                folders.Add(Environment.SystemDirectory);

                string winDir = Environment.GetEnvironmentVariable("windir");
                if(string.IsNullOrEmpty(winDir) == false)
                    folders.Add(winDir);

                string pathFromEnv = Environment.GetEnvironmentVariable("PATH");
                if (string.IsNullOrEmpty(pathFromEnv))
                    return folders;

                string[] paths = pathFromEnv.Split(';');
                folders.AddRange(paths);
                
                return folders;
            }
        }

        //! \todo 이 기능을 여기저기서 쓰니 따로 정리하자.
        public static string FindFirst(string fileName)
        {
            foreach (var folder in DefaultSearchFolders)
            {
                string filePath = Path.Combine(folder, fileName);
                if (File.Exists(filePath))
                    return filePath;
            }

            throw new FileNotFoundException();
        }

        public static IEnumerable<string> FindAll(string fileName)
        {
            var searchFilePaths = from folder in DefaultSearchFolders
                         select Path.Combine(folder, fileName)
                ;

            var filesFound = from filePath in searchFilePaths
                             where File.Exists(filePath) == true
                             select filePath
                ;
            return filesFound;
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
            foreach (string file in Directory.GetFiles(rootDir, pattern))
            {
                files.Add(file);
            }

            foreach (string dir in Directory.GetDirectories(rootDir))
            {
                
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