using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.Net
{
    internal class WinScpMkDir : AbstractWinScp
    {        
        [Required]
        public ITaskItem[] Directories { get; set; }

        public WinScpMkDir()
        {
            Batch = "continue";
        }

        private static string GetCreateRemoteDirectoryCommand(string dstDir)
        {
            StringBuilder builder = new StringBuilder();
            // builder.AppendLine("option batch continue");

            string[] paths = dstDir.Split(
                new[] { Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar }
                , StringSplitOptions.RemoveEmptyEntries
                );
            string currentDir = string.Empty;
            foreach (string path in paths)
            {
                currentDir = Path.Combine(currentDir, path);
                currentDir = currentDir.Replace(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar);

                builder.AppendLine(
                    string.Format("mkdir \"{0}\"", currentDir)
                    );
            }


            // builder.AppendLine(Options);
            return builder.ToString();
        }
        

        protected override string Scripts
        {
            get
            {
                StringBuilder sb = new StringBuilder();
                foreach (var directory in Directories)
                {
                    sb.AppendLine(GetCreateRemoteDirectoryCommand(directory.ItemSpec));
                }
                return sb.ToString();
            }
        }
    }
}
