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
    public class WinScpDownload : AbstractWinScp
    {
        public const string LocalDirKeyName = "LocalDir";
        public const string SwitchesKeyName = "Switches";
        public const string CreateLocalFolderKeyName = "CreateLocalFolder";

        private readonly WinScp _winScp;
        private ITaskItem[] _files;

        public WinScpDownload()
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
            return file.GetMetadata(LocalDirKeyName);
        }

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
                string.Format("get \"{0}\" \"{1}\" ", srcPath, dstDir)
                );

            if (string.IsNullOrEmpty(putSwitches))
                builder.Append(putSwitches);

            builder.AppendLine();

            return builder.ToString();
        }

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


        public override bool Execute()
        {
            foreach (var file in _files)
            {
                if (NeedToCreateLocalFolder(file) == true)
                {
                    string dstDir = GetRemoteDirectory(file);
                    Directory.CreateDirectory(dstDir);                    
                }
            }
            return base.Execute();                        
        }
    }
}
