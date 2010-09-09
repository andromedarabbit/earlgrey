using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.SevenZip
{
    public class Pack : ToolTask, IPack
    {
        private string _password;
        private ITaskItem _srcFolder;

        public Pack()
        {
            Overwrite = false;
            ContainsRootDir = true;
            ZipLevel = FromCompressionLevel(CompressionLevel.Default);
            UseLargePages = true;
        }

        protected override bool ValidateParameters()
        {
            if (base.ValidateParameters() == false)
                return false;

            if (TaskItemHelper.FolderExists(SrcFolder) == false)
            {
                Log.LogError(
                    string.Format("Source folder path '{0}' not found!", SrcFolder.ItemSpec)
                    );
                return false;
            }

            if (TaskItemHelper.FileExists(ZipFilePath) == true && Overwrite == false)
            {
                Log.LogError(
                    string.Format("Destination file path '{0}' already exists!", ZipFilePath.ItemSpec)
                    );
                return false;
            }

            return true;
        }

        protected override string GenerateFullPathToTool()
        {
            return ToolsSearch.FindExternalTool(ToolName);
        }

        protected override string GenerateCommandLineCommands()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
            builder.AppendSwitch("a");

            builder.AppendFileNameIfNotNull(InternalZipFilePath);
            if (ContainsRootDir)
                builder.AppendFileNameIfNotNull(InternalSrcFolder);
            else
                builder.AppendFileNameIfNotNull(InternalSrcFolder + @"\*");

            builder.AppendSwitch(UseLargePagesSwitch);

            builder.AppendSwitch("-mx=" + ZipLevel);
            
            // 7z a archive.7z -psecret);
            if (string.IsNullOrEmpty(Password) == false)
            {                
                builder.AppendSwitch("-p" + Password);
            }

            if (HasExcludePattern())
            {
                builder.AppendSwitch("-r0");

                foreach (var exclude in Excludes)
                {
                    builder.AppendSwitch("-x!" + exclude);
                }
            }

            return builder.ToString();
        }

        public override bool Execute()
        {
            if (TaskItemHelper.FileExists(ZipFilePath) == true && Overwrite == true)
            {
                TaskItemHelper.FileDelete(ZipFilePath);
            }

            return base.Execute();
        }

        private CompressionLevel InternalZipLevel
        {
            get { return ToCompressionLevel(ZipLevel); }
        }

        private static int FromCompressionLevel(CompressionLevel level)
        {
            return (int)level;
        }

        private static CompressionLevel ToCompressionLevel(int level)
        {
            return (CompressionLevel)level;
        }

        private string InternalSrcFolder
        {
            get { return TaskItemHelper.GetFullPath(SrcFolder); }
        }

        private string InternalZipFilePath
        {
            get { return TaskItemHelper.GetFullPath(ZipFilePath); }
        }

        protected override string ToolName
        {
            get { return @"7za.exe"; }
        }

        private bool HasExcludePattern()
        {
            return Excludes != null && Excludes.Length > 0;
        }

        [Required]
        public virtual ITaskItem SrcFolder
        {
            get { return _srcFolder; } 
            set 
            { 
                value.ItemSpec = value.ItemSpec.TrimEnd('\\');
                _srcFolder = value;
            }
        }

        public virtual string[] Excludes { get; set; }

        [Required]
        public virtual ITaskItem ZipFilePath { get; set; }

        public virtual bool Overwrite { get; set; }

        public virtual int ZipLevel { get; set; }

        // TODO: 공백 못 들어가게 막자.
        public virtual string Password 
        { 
            get { return _password; }
            set 
            {
                _password = value.Trim();
            }
        }

        public virtual bool ContainsRootDir { get; set; }

        public bool UseLargePages { get; set; }

        private string UseLargePagesSwitch
        {
            get
            {
                if (UseLargePages)
                    return "-slp";
                return "-slp-";
            }
        }
    }
}
