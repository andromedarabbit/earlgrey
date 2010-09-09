using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Ionic.Zip;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Compression.Zip
{
    public class Unpack : AbstractTask, IUnpack
    {
        public Unpack()
        {
            Overwrite = false;
        }

        protected override bool ValidateParameters()
        {
            if (TaskItemHelper.FileExists(ZipFilePath) == false)
            {
                Log.LogError(
                    string.Format("Source file path '{0}' not found!", ZipFilePath.ItemSpec)
                    );
                return false;
            }

            if (TaskItemHelper.FolderExists(TargetDir) == true && Overwrite == false)
            {
                Log.LogError(
                    string.Format("Target directory path '{0}' already exists!", TargetDir.ItemSpec)
                    );
                return false;
            }
            
            return true;
        }
        
        protected override bool ExecuteCommand()
        {
            using(ZipFile zip = ZipFile.Read(InternalZipFilePath))
            {
                if(string.IsNullOrEmpty(Password) == false)
                {
                    zip.Password = Password;
                }
                zip.ExtractAll(InternalTargetDir, OverwritePolicy);
            }

            return true;
        }

        [Required]
        public ITaskItem TargetDir { get; set; }

        private string InternalTargetDir
        {
            get { return TaskItemHelper.GetFullPath(TargetDir); }
        }

        public bool Overwrite { get; set; }

        private ExtractExistingFileAction OverwritePolicy
        {
            get
            {
                if (Overwrite)
                    return ExtractExistingFileAction.OverwriteSilently;
                return ExtractExistingFileAction.Throw;
            }
        }

        public virtual string Password { get; set; }

        // TODO: 중복 코드 제거하기 (Pack 확인)
        [Required]
        public ITaskItem ZipFilePath { get; set; }

        private string InternalZipFilePath
        {
            get { return TaskItemHelper.GetFullPath(ZipFilePath); }
        }
    }
}
