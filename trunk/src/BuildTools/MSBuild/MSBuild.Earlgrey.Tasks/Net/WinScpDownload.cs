using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.Net
{
    // \ref http://winscp.net/eng/docs/script_commands


    /// <summary>
    /// Download files from the remote FTP server by using WinSCP.
    /// </summary>
    /// <example>
    /// <code title="Download a file from the remote FTP server and place it in into this directory." lang="xml" source=".\Samples\msbuild-WinScpDownload-download-a-file.xml" />
    /// <code title="Download files from the remote FTP server and place those in into sub-directories." lang="xml" source=".\Samples\msbuild-WinScpDownload-download-files-into-sub-directories.xml" />
    /// </example>
    /// <inheritdoc />
    public class WinScpDownload : AbstractWinScp
    {
        internal const string LocalDirKeyName = "LocalDir";
        internal const string SwitchesKeyName = "Switches";
        internal const string CreateLocalFolderKeyName = "CreateLocalFolder";

        private readonly WinScp _winScp;
        private ITaskItem[] _files;

        /// <summary>
        /// Initializes a new instance of the <see cref="WinScpDownload"/> class.
        /// </summary>
        /// <inheritdoc/>
        /// <remarks></remarks>
        public WinScpDownload()
        {
            _winScp = new WinScp();
            _winScp.BuildEngine = this.BuildEngine;
        }


        /// <summary>
        /// [Required] Gets or sets the files to download from the remote server.
        /// </summary>
        /// <value>The files to download.</value>
        /// <remarks>
        /// There are two different metadata you can use:
        /// 
        /// <list type="bullet">
        /// <listheader>
        /// <term>LocalDir</term>
        /// <description>The local directory where the file to be placed.</description>
        /// </listheader>
        /// <item>
        /// <term>CreateLocalFolder</term>
        /// <description>If <c>true</c>, create 'LocalDir' when it is not found!</description>
        /// </item>
        /// </list>
        /// </remarks>
        [Required]
        public ITaskItem[] Files
        {
            get { return _files; }
            set
            {
                _files = value;

                foreach (var file in _files)
                {
                    if (string.IsNullOrEmpty(GetLocalDirectory(file)) == true)
                    {
                        file.SetMetadata(LocalDirKeyName, @".\");
                    }

                    if (string.IsNullOrEmpty(file.GetMetadata(CreateLocalFolderKeyName)) == true)
                    {
                        file.SetMetadata(CreateLocalFolderKeyName, "true");
                    }
                }
            }
        }

        private static bool NeedToCreateLocalFolder(ITaskItem file)
        {
            string createLocalFolderValue = file.GetMetadata(CreateLocalFolderKeyName);
            return bool.Parse(createLocalFolderValue);
        }

        private static string GetRemoteDirectory(ITaskItem file)
        {
            return file.ItemSpec; 
        }

        private static string GetLocalDirectory(ITaskItem file)
        {
            return file.GetMetadata(LocalDirKeyName).TrimEnd('\\');
        }

        /// <inheritdoc />
        protected override string Scripts
        {
            get
            {
                var builder = new StringBuilder();

                foreach (ITaskItem file in Files)
                {
                    builder.AppendLine(GetDownloadCommands(file));
                }

                return builder.ToString();
            }
        }

        private static string GetDownloadCommands(ITaskItem file)
        {
            StringBuilder builder = new StringBuilder();

            string srcPath = GetRemoteDirectory(file);
            string putSwitches = file.GetMetadata(SwitchesKeyName);

            string dstDir = GetLocalDirectory(file);
            builder.Append(
                string.Format("get \"{0}\" \"{1}\\\" ", srcPath, dstDir)
                );

            if (string.IsNullOrEmpty(putSwitches))
                builder.Append(putSwitches);

            builder.AppendLine();

            return builder.ToString();
        }

        /// <summary>
        /// Validates the parameters.
        /// </summary>
        /// <returns></returns>
        /// <inheritdoc/>
        /// <remarks></remarks>
        protected override bool ValidateParameters()
        {
            bool hasInvalidRemotePath = Files.Any(
                file => string.IsNullOrEmpty(GetRemoteDirectory(file))
                );

            if (hasInvalidRemotePath)
            {
                Log.LogError("All items of the property Files should have their own remote directory!");
                return false;
            }

            return true;
        }


        /// <summary>
        /// Executes this instance.
        /// </summary>
        /// <returns></returns>
        /// <inheritdoc/>
        /// <remarks></remarks>
        public override bool Execute()
        {
            foreach (var file in _files)
            {
                if (NeedToCreateLocalFolder(file) == true)
                {
                    string dstDir = GetLocalDirectory(file);
                    Directory.CreateDirectory(dstDir);                    
                }
            }
            return base.Execute();                        
        }

    }
}
