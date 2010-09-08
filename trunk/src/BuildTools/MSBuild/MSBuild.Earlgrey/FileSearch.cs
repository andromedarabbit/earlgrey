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

        /*
        private static bool ExecuteCommandSync(string command)
        {
            // create the ProcessStartInfo using "cmd" as the program to be run,
            // and "/c " as the parameters.
            // Incidentally, /c tells cmd that we want it to execute the command that follows,
            // and then exit.
            System.Diagnostics.ProcessStartInfo procStartInfo =
                new System.Diagnostics.ProcessStartInfo("cmd", "/c " + command);

            // The following commands are needed to redirect the standard output.
            // This means that it will be redirected to the Process.StandardOutput StreamReader.
            procStartInfo.RedirectStandardOutput = true;
            procStartInfo.UseShellExecute = false;
            // Do not create the black window.
            procStartInfo.CreateNoWindow = true;
            // Now we create a process, assign its ProcessStartInfo and start it
            System.Diagnostics.Process proc = new System.Diagnostics.Process();
            proc.StartInfo = procStartInfo;
            if( proc.Start() == false)
                return false;

            return true;
            // Get the output into a string
            // string result = proc.StandardOutput.ReadToEnd();
        }
         * */

        // SImple way to find out if the file exists.
        public static bool Exists(string fileName)
        {
            // if (ExecuteCommandSync(fileName) == false)
                // return false;

            // return true;
            if(File.Exists(fileName))
                return true;

            string pathFromEnv = Environment.GetEnvironmentVariable("PATH");
            if(string.IsNullOrEmpty(pathFromEnv))
                return false;

            string[] paths = pathFromEnv.Split(';');
            foreach (var path in paths)
            {
                if(string.IsNullOrEmpty(path))
                    continue;

                string fullPath = Path.Combine(path, fileName);
                if(File.Exists(fullPath))
                    return true;
            }
            return false;
        }
    }
}
