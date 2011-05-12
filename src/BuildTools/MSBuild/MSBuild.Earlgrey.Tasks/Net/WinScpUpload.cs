using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.Net
{
    // \ref http://winscp.net/eng/docs/script_commands
    public class WinScpUpload : AbstractWinScp
    {
        public const string RemoteDirKeyName = "RemoteDir";
        public const string SwitchesKeyName = "Switches";
        public const string CreateRemoteFolderKeyName = "CreateRemoteFolder";

        private readonly WinScp _winScp;
        private ITaskItem[] _files;

        public WinScpUpload()
        {
            _winScp = new WinScp();
            _winScp.BuildEngine = this.BuildEngine;
        }

        
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
            return file.GetMetadata(RemoteDirKeyName);
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
                string.Format("put \"{0}\" \"{1}\" ", srcPath, dstDir)
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
