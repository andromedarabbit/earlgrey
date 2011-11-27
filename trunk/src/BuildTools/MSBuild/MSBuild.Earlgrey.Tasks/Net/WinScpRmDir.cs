using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.Net
{
    /// <summary>
    /// Remove the remote directory by using WinSCP.
    /// </summary>
    /// <example>
    /// <code title="Create the remote directory and delete it." lang="xml" source=".\Samples\msbuild-WinScpMkDir-WindScpRmDir.xml" />
    /// </example>
    /// <remarks></remarks>
    public class WinScpRmDir : AbstractWinScp
    {
        /// <summary>
        /// [Required] Gets or sets the remote directories to remove.
        /// </summary>
        /// <value>The remote directories to remove.</value>
        /// <remarks></remarks>
        [Required]
        public ITaskItem[] Directories { get; set; }

        public WinScpRmDir()
        {
            // Batch = "continue";
        }

        private static string GetDeleteRemoteDirectoryCommand(string dstDir)
        {
            /*
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
                    string.Format("rmdir \"{0}\"", currentDir)
                    );
            }


            // builder.AppendLine(Options);
            return builder.ToString();
             * */
            return string.Format("rmdir \"{0}\"", dstDir);
        }
        

        protected override string Scripts
        {
            get
            {
                StringBuilder sb = new StringBuilder();
                foreach (var directory in Directories)
                {
                    sb.AppendLine(GetDeleteRemoteDirectoryCommand(directory.ItemSpec));
                }
                return sb.ToString();
            }
        }
    }
}
