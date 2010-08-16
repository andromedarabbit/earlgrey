using System;
using System.Collections.Generic;
using System.IO;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO
{
    public class ShareFolder : AbstractTask
    {
        private readonly List<string> _users;

        public ShareFolder()
        {
            _users = new List<string>();

            MakeFolderWhenNotExist = true;
            ResetExistingOne = false;
            MaximumUsers = 10;
            Privilege = Permission.Read;
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

            Win32Share.Options options = new Win32Share.Options()
            {
                Path = LocalPath,
                Name = this.Name,
                Type = Win32Share.ShareType.DiskDrive,
                MaximumAllowed = MaximumUsers,
                Description = this.Description,
                // DomainUsers = this.Users,
                Privilege = (Win32Share.AccessMaskType)this.Privilege
                // Permission = this.Permission
            };
            options.AddUsers(this.Users);

            Win32Share.MethodStatus result = Win32Share.Create(options); // LocalPath, Name, Win32Share.ShareType.DiskDrive, Users, Description, null);
            if (result != Win32Share.MethodStatus.Success)
            {
                string errorCode = Enum.Format(typeof(Win32Share.MethodStatus), result, "G");
                Log.LogError(string.Format("Couldn't share the folder! Reason: {0}", errorCode));
                return false;
            }
            return true;
        }

        protected override bool ValidateParameters()
        {
            return true;
        }

        internal string LocalPath
        {
            get { return LocalFolder.GetMetadata("FullPath"); }
        }

        internal string Uri
        {
            get { return string.Format(@"\\{0}\{1}", Environment.MachineName, Name); }
        }

        [Required]
        public ITaskItem LocalFolder { get; set; }

        public string Description { get; set; }

        [Required]
        public string Name { get; set; }

        public bool ResetExistingOne { get; set; }

        public bool MakeFolderWhenNotExist { get; set; }

        public uint MaximumUsers { get; set; }

        public string[] Users
        {
            get { return _users.ToArray(); }
            set
            {
                foreach (string user in value)
                {
                    _users.Add(user);
                }
            }
        }

        // \todo 제대로 권한 배분한 건지......
        public enum Permission : uint
        {
            Read = Win32Share.AccessMaskType.FILE_READ_DATA | Win32Share.AccessMaskType.FILE_LIST_DIRECTORY | Win32Share.AccessMaskType.FILE_READ_EA | Win32Share.AccessMaskType.FILE_EXECUTE | Win32Share.AccessMaskType.FILE_TRAVERSE | Win32Share.AccessMaskType.FILE_READ_ATTRIBUTES,
            Change = Read | Win32Share.AccessMaskType.FILE_WRITE_DATA | Win32Share.AccessMaskType.FILE_ADD_FILE | Win32Share.AccessMaskType.FILE_APPEND_DATA | Win32Share.AccessMaskType.FILE_ADD_SUBDIRECTORY | Win32Share.AccessMaskType.FILE_WRITE_EA | Win32Share.AccessMaskType.FILE_DELETE_CHILD | Win32Share.AccessMaskType.FILE_WRITE_ATTRIBUTES | Win32Share.AccessMaskType.DELETE | Win32Share.AccessMaskType.SYNCHRONIZE,
            Full = Read | Change | Win32Share.AccessMaskType.READ_CONTROL | Win32Share.AccessMaskType.WRITE_DAC | Win32Share.AccessMaskType.WRITE_OWNER
        }

        public Permission Privilege { get; set; }

        //! \todo PASSWORD

    }
}
