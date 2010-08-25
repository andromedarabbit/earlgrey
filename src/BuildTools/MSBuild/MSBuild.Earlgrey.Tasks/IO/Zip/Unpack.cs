using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Ionic.Zip;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO.Zip
{
    public class Unpack : AbstractTask
    {
        public Unpack()
        {
            Overwrite = false;
        }
            
        protected override bool ValidateParameters()
        {
            if (FileExists(ZipFilePath) == false)
            {
                Log.LogError(
                    string.Format("Source file path '{0}' not found!", ZipFilePath.ItemSpec)
                    );
                return false;
            }

            if (FileExists(TargetDir) == true && Overwrite == false)
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
                zip.ExtractAll(InternalTargetDir, OverwritePolicy);
            }

            return true;
        }

        private static bool FileExists(ITaskItem taskItem)
        {
            if(taskItem == null)
                return false;

            return File.Exists(GetFullPath(taskItem));
        }

        [Required]
        public ITaskItem TargetDir { get; set; }

        private string InternalTargetDir
        {
            get { return GetFullPath(TargetDir); }
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

        // TODO: 중복 코드 제거하기 (Pack 확인)
        [Required]
        public ITaskItem ZipFilePath { get; set; }

        private string InternalZipFilePath
        {
            get { return GetFullPath(ZipFilePath); }
        }

        private static string GetFullPath(ITaskItem taskItem)
        {
            return taskItem.GetMetadata("FullPath");
        }
    }
}
