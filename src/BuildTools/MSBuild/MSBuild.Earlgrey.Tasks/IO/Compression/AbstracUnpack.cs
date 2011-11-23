using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Ionic.Zip;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Compression
{
    internal abstract class AbstractUnpack : AbstractTask
    {
        protected AbstractUnpack()
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

        protected abstract override bool ExecuteCommand();

        [Required]
        public abstract ITaskItem TargetDir { get; set; }

        protected string InternalTargetDir
        {
            get { return TaskItemHelper.GetFullPath(TargetDir); }
        }

        public abstract bool Overwrite { get; set; }

        // TODO: 중복 코드 제거하기 (Pack 확인)
        [Required]
        public abstract ITaskItem ZipFilePath { get; set; }

        protected string InternalZipFilePath
        {
            get { return TaskItemHelper.GetFullPath(ZipFilePath); }
        }

        
    }
}
