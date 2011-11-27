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
    /// Uploads files into the remote FTP server by using WinSCP.
    /// </summary>
    /// <example>
    /// <code title="Upload a file to the remote FTP server and place it in into the root directory." lang="xml" source=".\Samples\msbuild-WinScpUpload-upload-a-file.xml" />
    /// <code title="Upload files to the remote FTP server and place those in into sub-directories." lang="xml" source=".\Samples\msbuild-WinScpUpload-upload-files-into-sub-directories.xml" />
    /// </example>
    /// <inheritdoc />
    public class WinScpUpload : AbstractWinScp
    {
        internal const string RemoteDirKeyName = "RemoteDir";
        internal const string SwitchesKeyName = "Switches";
        internal const string CreateRemoteFolderKeyName = "CreateRemoteFolder";

        private readonly WinScp _winScp;
        private ITaskItem[] _files;

        /// <summary>
        /// Initializes a new instance of the <see cref="WinScpUpload"/> class.
        /// </summary>
        /// <inheritdoc/>
        /// <remarks></remarks>
        public WinScpUpload()
        {
            _winScp = new WinScp();
            _winScp.BuildEngine = this.BuildEngine;
        }

        /// <summary>
        /// [Required] Gets or sets the files to upload into the remote server.
        /// </summary>
        /// <value>The files to upload.</value>
        /// <remarks>
        /// There are two different metadata you can use:
        /// 
        /// <list type="bullet">
        /// <listheader>
        /// <term>RemoteDir</term>
        /// <description>The remote directory where the file to be placed.</description>
        /// </listheader>
        /// <item>
        /// <term>CreateRemoteFolder</term>
        /// <description>If <c>true</c>, create 'RemoteDir' when it is not found!</description>
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
                    if (string.IsNullOrEmpty(GetRemoteDirectory(file)) == true)
                    {
                        file.SetMetadata(RemoteDirKeyName, "./");
                    }

                    if (string.IsNullOrEmpty(file.GetMetadata(CreateRemoteFolderKeyName)) == true)
                    {
                        file.SetMetadata(CreateRemoteFolderKeyName, "true");
                    }
                }
            }
        }

        private static bool NeedToCreateRemoteFolder(ITaskItem file)
        {
            string createRemoteFolderValue = file.GetMetadata(CreateRemoteFolderKeyName);
            return bool.Parse(createRemoteFolderValue);
        }

        private static string GetRemoteDirectory(ITaskItem file)
        {
            return file.GetMetadata(RemoteDirKeyName).TrimEnd('/');
        }

        private static string GetLocalDirectory(ITaskItem file)
        {
            return file.ItemSpec;
        }

        protected override string Scripts
        {
            get
            {
                var builder = new StringBuilder();

                foreach (ITaskItem file in Files)
                {
                    string dstDir = GetRemoteDirectory(file);
                    
                    if(NeedToCreateRemoteFolder(file) == true)
                    {
                        builder.AppendLine(
                            GetCreateRemoteDirectoryCommand(dstDir)
                            );
                    }

                    builder.AppendLine(GetUploadCommands(file));
                }

                return builder.ToString();
            }
        }

        private static string GetUploadCommands(ITaskItem file)
        {
            StringBuilder builder = new StringBuilder();

            string srcPath = GetLocalDirectory(file);                    
            string putSwitches = file.GetMetadata(SwitchesKeyName);

            string dstDir = GetRemoteDirectory(file);
            builder.Append(
                string.Format("put \"{0}\" \"{1}/\" ", srcPath, dstDir)
                );

            if (string.IsNullOrEmpty(putSwitches))
                builder.Append(putSwitches);

            builder.AppendLine();

            return builder.ToString();
        }

        
        private string GetCreateRemoteDirectoryCommand(string dstDir)
        {
            StringBuilder builder = new StringBuilder();
            builder.AppendLine("option batch continue");


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


            builder.AppendLine(Options);
            return builder.ToString();
        }
        

        protected override bool ValidateParameters()
        {            
            bool hasInvalidRemotePath = Files.Any(
                file => string.IsNullOrEmpty(GetRemoteDirectory(file))
                );

            if(hasInvalidRemotePath)
            {
                Log.LogError("All items of the property Files should have their own remote directory!");
                return false;
            }
            
            return true;
        }

    }
}
