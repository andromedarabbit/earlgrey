using System;
using System.Collections.Generic;
using System.IO;
using Microsoft.Build.Framework;

namespace MSBuild.Earlgrey.Tasks.IO
{
    /// <summary>
    /// Make the target folder as a shared one. You can also
    /// </summary>
    /// <example>
    /// <code title="Create a shared folder and delete it." lang="xml" source=".\Samples\msbuild-share-and-delete-the-shared-folder.xml" />
    /// </example>
    /// <remarks></remarks>
    public class ShareFolder : AbstractTask
    {
        private readonly List<string> _users;

        /// <inheritdoc />
        public ShareFolder()
        {
            _users = new List<string>();

            MakeFolderWhenNotExist = true;
            ResetExistingOne = false;
            MaximumUsers = 10;
            InternalPrivilege = Permission.Read;
        }

        /// <inheritdoc />
        protected override bool ExecuteCommand()
        {
            return CreateLocalFolder() && Share();
        }

        /// <summary>
        /// Creates the local folder.
        /// </summary>
        /// <returns></returns>
        /// <remarks></remarks>
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
                Privilege = (Win32Share.AccessMaskType)this.InternalPrivilege
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

        /// <inheritdoc />
        protected override bool ValidateParameters()
        {
            return true;
        }

        /// <summary>
        /// Gets the full path of the <see cref="LocalFolder"/>.
        /// </summary>
        /// <remarks></remarks>
        internal string LocalPath
        {
            get { return LocalFolder.GetMetadata("FullPath"); }
        }

        /// <summary>
        /// Gets the URI.
        /// </summary>
        /// <remarks></remarks>
        internal string Uri
        {
            get { return string.Format(@"\\{0}\{1}", Environment.MachineName, Name); }
        }

        /// <summary>
        /// [Required] Gets or sets the local folder to be shared.
        /// </summary>
        /// <value>The local folder.</value>
        /// <remarks></remarks>
        [Required]
        public ITaskItem LocalFolder { get; set; }

        /// <summary>
        /// Gets or sets the description.
        /// </summary>
        /// <value>The description.</value>
        /// <remarks></remarks>
        public string Description { get; set; }

        /// <summary>
        /// [Required] Gets or sets the name of the shared folder.
        /// </summary>
        /// <value>The name.</value>
        /// <remarks></remarks>
        [Required]
        public string Name { get; set; }

        /// <summary>
        /// Describes how to act if the local machine already have the shared folder named <see cref="Name"/>, 
        /// </summary>
        /// <value><c>true</c> if delete the old one and create an new shared folder named <see cref="Name"/>. If <c>false</c>, this task will fail with an error message.</value>
        /// <remarks></remarks>
        public bool ResetExistingOne { get; set; }

        /// <summary>
        /// Describes how to act If <see cref="LocalFolder"/> is not found. 
        /// </summary>
        /// <value>If <c>true</c>, it create a new <see cref="LocalFolder"/>. If <c>false</c>, this task will fail with an error message.</value>
        /// <remarks></remarks>
        public bool MakeFolderWhenNotExist { get; set; }

        /// <summary>
        /// The maximum number of users who are able to access the shared folder <see cref="Name"/> at the same time.
        /// </summary>
        /// <value>The maximum users.</value>
        /// <remarks></remarks>
        public uint MaximumUsers { get; set; }

        /// <summary>
        /// The users who have right to access the target shared folder <see cref="Name"/>.
        /// </summary>
        /// <value>Gets or sets the users</value>
        /// <remarks></remarks>
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
        /// <summary>
        /// Describes read/write privilege of the shared folder.  
        /// </summary>
        /// <remarks></remarks>
        public enum Permission : uint
        {
            /// <summary>
            /// Readable.
            /// </summary>
            Read = Win32Share.AccessMaskType.FILE_READ_DATA | Win32Share.AccessMaskType.FILE_LIST_DIRECTORY | Win32Share.AccessMaskType.FILE_READ_EA | Win32Share.AccessMaskType.FILE_EXECUTE | Win32Share.AccessMaskType.FILE_TRAVERSE | Win32Share.AccessMaskType.FILE_READ_ATTRIBUTES,
            
            /// <summary>
            /// Writable.
            /// </summary>
            Change = Read | Win32Share.AccessMaskType.FILE_WRITE_DATA | Win32Share.AccessMaskType.FILE_ADD_FILE | Win32Share.AccessMaskType.FILE_APPEND_DATA | Win32Share.AccessMaskType.FILE_ADD_SUBDIRECTORY | Win32Share.AccessMaskType.FILE_WRITE_EA | Win32Share.AccessMaskType.FILE_DELETE_CHILD | Win32Share.AccessMaskType.FILE_WRITE_ATTRIBUTES | Win32Share.AccessMaskType.DELETE | Win32Share.AccessMaskType.SYNCHRONIZE,
            
            /// <summary>
            /// Readable and Writable.
            /// </summary>
            Full = Read | Change | Win32Share.AccessMaskType.READ_CONTROL | Win32Share.AccessMaskType.WRITE_DAC | Win32Share.AccessMaskType.WRITE_OWNER
        }

        private Permission InternalPrivilege { get; set; }

        //! \todo PASSWORD
        /// <summary>
        /// Gets or sets the privilege.
        /// <list type="bullet">
        /// <item>
        /// <description>Read</description>
        /// </item>
        /// <item>
        /// <description>Change</description>
        /// </item>
        /// <item>
        /// <description>Fulle</description>
        /// </item>
        /// </list>
        /// </summary>
        /// <value>The privilege.</value>
        /// <remarks></remarks>
        public string Privilege
        {
            get { return Enum.GetName(typeof(Permission), InternalPrivilege); }
            set { InternalPrivilege = (Permission)Enum.Parse(typeof(Permission), value); }
        }
    }
}
