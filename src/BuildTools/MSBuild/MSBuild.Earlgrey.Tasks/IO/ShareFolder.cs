using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Management;
using System.IO;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO
{
    public class ShareFolder : AbstractTask
    {
        public ShareFolder()
        {
            MakeFolderWhenNotExist = true;
            ResetExistingOne = false;
        }

        protected override bool ExecuteCommand()
        {
            return CreateLocalFolder() && Share();
        }

        private bool CreateLocalFolder()
        {
            try
            {
                string localPath = LocalPath;

                if (Directory.Exists(localPath))
                    return true;

                if (MakeFolderWhenNotExist)
                {
                    Directory.CreateDirectory(LocalPath);
                    return true;
                }

                return false;
            }
            catch (Exception ex)
            {
                Log.LogError(ex.Message);
                return false;
            }
        }

        private bool Share()
        {
            // \todo DeleteSharedFolder 클래스를 써서 중복 코드 제거하자
            using (Win32Share sharedFolder = Win32Share.GetNamedShare(Name))
            {
                if (sharedFolder != null)
                {
                    if (ResetExistingOne == false)
                    {
                        Log.LogError("Shared folder with the same name already exists!");
                        return false;
                    }

                    Win32Share.MethodStatus deleteResult = sharedFolder.Delete();
                    if (deleteResult != Win32Share.MethodStatus.Success)
                    {
                        Log.LogError("Couldn't delete the existing folder!");
                        return false;
                    }
                }
            }

            Win32Share.MethodStatus result = Win32Share.Create(LocalPath, Name, Win32Share.ShareType.DiskDrive, 10, Description, null);
            if (result != Win32Share.MethodStatus.Success)
            {
                Log.LogError("Couldn't share the folder!");
                return false;
            }
            return true;
        }

        protected override bool ValidateParameters()
        {
            return true;
        }

        private string LocalPath
        {
            get { return LocalFolder.GetMetadata("FullPath"); }
        }

        [Required]
        public ITaskItem LocalFolder { get; set; }

        public string Description { get; set; }

        [Required]
        public string Name { get; set; }

        public bool ResetExistingOne { get; set; }

        public bool MakeFolderWhenNotExist { get; set; }

        //! \todo USERNAME PASSWORD
    }
}
