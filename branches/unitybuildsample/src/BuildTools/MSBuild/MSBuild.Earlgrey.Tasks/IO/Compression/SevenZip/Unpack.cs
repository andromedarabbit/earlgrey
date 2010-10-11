using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.SevenZip
{
    public class Unpack : AbstractToolTask, IUnpack
    {
        public Unpack()
        {
            Overwrite = false;
        }

        protected override bool ValidateParameters()
        {
            if (base.ValidateParameters() == false)
                return false;

            return true;
        }

        protected override string GenerateCommandLineCommands()
        {
            CommandLineBuilder builder = new CommandLineBuilder();
            builder.AppendSwitch("x");

            builder.AppendFileNameIfNotNull(InternalZipFilePath);
            builder.AppendSwitch("-o\"" + InternalTargetDir + "\"");
            // builder.AppendFileNameIfNotNull(InternalTargetDir);

            builder.AppendSwitch(OverwriteSwitch);

            // 7z a archive.7z -psecret
            if (string.IsNullOrEmpty(Password) == false)
            {
                builder.AppendSwitch("-p" + Password);
            }

            return builder.ToString();
        }

        protected override string ToolName
        {
            get { return @"7za.exe"; }
        }

        private string InternalTargetDir
        {
            get { return TaskItemHelper.GetFullPath(TargetDir); }
        }

        [Required]
        public virtual ITaskItem TargetDir { get; set; }

        public virtual bool Overwrite { get; set; }

        private ExtractExistingFileAction OverwritePolicy
        {
            get
            {
                if (Overwrite)
                    return ExtractExistingFileAction.OverwriteAll;
                return ExtractExistingFileAction.Skip;
            }
        }

        private string OverwriteSwitch
        {
            get
            {
                if (OverwritePolicy == ExtractExistingFileAction.OverwriteAll)
                    return "-aoa";
                if (OverwritePolicy == ExtractExistingFileAction.Skip)
                    return "-aos";
                if (OverwritePolicy == ExtractExistingFileAction.AutoRenameExtractingFile)
                    return "-aou";
                if (OverwritePolicy == ExtractExistingFileAction.AutoRenameExistingFile)
                    return "-aot";

                throw new Exception();
            }
        }
        // TODO: 중복 코드 제거하기 (Pack 확인)
        [Required]
        public virtual ITaskItem ZipFilePath { get; set; }

        private string InternalZipFilePath
        {
            get { return TaskItemHelper.GetFullPath(ZipFilePath); }
        }


        public virtual string Password { get; set; }
    }

}
