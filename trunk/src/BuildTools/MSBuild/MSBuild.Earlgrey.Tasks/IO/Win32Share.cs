using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Management;
using System.Security.Principal;


namespace MSBuild.Earlgrey.Tasks.IO
{

    //! \ref http://www.gamedev.net/community/forums/topic.asp?topic_id=408923
    internal class Win32Share : IDisposable
    {
        public enum MethodStatus : uint
        {
            Success = 0, 	//Success
            AccessDenied = 2, 	//Access denied
            UnknownFailure = 8, 	//Unknown failure
            InvalidName = 9, 	//Invalid name
            InvalidLevel = 10, 	//Invalid level
            InvalidParameter = 21, 	//Invalid parameter
            DuplicateShare = 22, 	//Duplicate share
            RedirectedPath = 23, 	//Redirected path
            UnknownDevice = 24, 	//Unknown device or directory
            NetNameNotFound = 25 	//Net name not found
        }

        public enum ShareType : uint
        {
            DiskDrive = 0x0, 	//Disk Drive
            PrintQueue = 0x1, 	//Print Queue
            Device = 0x2, 	//Device
            IPC = 0x3, 	//IPC
            DiskDriveAdmin = 0x80000000, 	//Disk Drive Admin
            PrintQueueAdmin = 0x80000001, 	//Print Queue Admin
            DeviceAdmin = 0x80000002, 	//Device Admin
            IpcAdmin = 0x80000003 	//IPC Admin
        }

        //! \ref http://msdn.microsoft.com/en-us/library/aa394063%28VS.85%29.aspx
        [Flags]
        internal enum AccessMaskType : uint
        {
            // Grants the right to read data from the file. For a directory, this value grants the right to list the contents of the directory.
            FILE_READ_DATA = 0x1,
            FILE_LIST_DIRECTORY = FILE_READ_DATA,

            // Grants the right to write data to the file. For a directory, this value grants the right to create a file in the directory.
            FILE_WRITE_DATA = 0x2,
            FILE_ADD_FILE = FILE_WRITE_DATA,

            // Grants the right to append data to the file. For a directory, this value grants the right to create a subdirectory.
            FILE_APPEND_DATA = 0x4,
            FILE_ADD_SUBDIRECTORY = FILE_APPEND_DATA,

            // Grants the right to read extended attributes.
            FILE_READ_EA = 0x8,

            // Grants the right to write extended attributes.
            FILE_WRITE_EA = 0x10,

            // Grants the right to execute a file. For a directory, the directory can be traversed.
            FILE_EXECUTE = 0x20,
            FILE_TRAVERSE = FILE_EXECUTE,

            // Grants the right to delete a directory and all the files it contains (its children), even if the files are read-only.
            FILE_DELETE_CHILD = 0x40,

            // Grants the right to read file attributes.
            FILE_READ_ATTRIBUTES = 0x80,

            // Grants the right to change file attributes.
            FILE_WRITE_ATTRIBUTES = 0x100,

            // Grants delete access.
            DELETE = 0x10000,

            // Grants read access to the security descriptor and owner.
            READ_CONTROL = 0x20000,

            // Grants write access to the discretionary access control list (ACL).
            WRITE_DAC = 0x40000,

            // Assigns the write owner.
            WRITE_OWNER = 0x80000,

            // Synchronizes access and allows a process to wait for an object to enter the signaled state.
            SYNCHRONIZE = 0x100000
        }

        [Flags]
        private enum AceFlagsType : uint
        {
            OBJECT_INHERIT_ACE = 0x1,
            CONTAINER_INHERIT_ACE = 0x2,
            NO_PROPAGATE_INHERIT_ACE = 0x4,
            INHERIT_ONLY_ACE = 0x8,
            INHERITED_ACE = 0x16,

            SUCCESSFUL_ACCESS_ACE_FLAG = 0x40,
            FAILED_ACCESS_ACE_FLAG = 0x80
        }

        private enum AceType : uint
        {
            AccessAllowed = 0,
            AccessDenied = 1,
            Audit = 2
        }



        private readonly ManagementObject _winShareObject;

        private Win32Share(ManagementObject obj) { _winShareObject = obj; }

        #region IDisposable

        // Track whether Dispose has been called.
        private bool _disposed = false;

        // Implement IDisposable.
        // Do not make this method virtual.
        // A derived class should not be able to override this method.
        public void Dispose()
        {
            Dispose(true);
            // This object will be cleaned up by the Dispose method.
            // Therefore, you should call GC.SupressFinalize to
            // take this object off the finalization queue
            // and prevent finalization code for this object
            // from executing a second time.
            GC.SuppressFinalize(this);
        }

        // Dispose(bool disposing) executes in two distinct scenarios.
        // If disposing equals true, the method has been called directly
        // or indirectly by a user's code. Managed and unmanaged resources
        // can be disposed.
        // If disposing equals false, the method has been called by the
        // runtime from inside the finalizer and you should not reference
        // other objects. Only unmanaged resources can be disposed.
        private void Dispose(bool disposing)
        {
            // Check to see if Dispose has already been called.
            if (!this._disposed)
            {
                // If disposing equals true, dispose all managed
                // and unmanaged resources.
                if (disposing)
                {
                    // Dispose managed resources.
                    _winShareObject.Dispose();
                }

                // Call the appropriate methods to clean up
                // unmanaged resources here.
                // If disposing is false,
                // only the following code is executed.


                // Note disposing has been done.
                _disposed = true;
            }
        }

        #endregion

        #region Wrap Win32_Share properties

        public AccessMaskType AccessMask
        {
            get { return (AccessMaskType)Convert.ToUInt32(_winShareObject["AccessMask"]); }
        }

        public bool AllowMaximum
        {
            get { return Convert.ToBoolean(_winShareObject["AllowMaximum"]); }
        }

        public string Caption
        {
            get { return Convert.ToString(_winShareObject["Caption"]); }
        }

        public string Description
        {
            get { return Convert.ToString(_winShareObject["Description"]); }
        }

        public DateTime InstallDate
        {
            get { return Convert.ToDateTime(_winShareObject["InstallDate"]); }
        }

        public uint MaximumAllowed
        {
            get { return Convert.ToUInt32(_winShareObject["MaximumAllowed"]); }
        }

        public string Name
        {
            get { return Convert.ToString(_winShareObject["Name"]); }
        }

        public string Path
        {
            get { return Convert.ToString(_winShareObject["Path"]); }
        }

        public string Status
        {
            get { return Convert.ToString(_winShareObject["Status"]); }
        }

        public ShareType Type
        {
            get { return (ShareType)Convert.ToUInt32(_winShareObject["Type"]); }
        }

        #endregion

        #region Wrap Methods

        public MethodStatus Delete()
        {
            object result = _winShareObject.InvokeMethod("Delete", new object[] { });
            uint r = Convert.ToUInt32(result);

            return (MethodStatus)r;
        }

        public class DomainUser
        {
            private string _username;
            private string _domain;

            public DomainUser(string user)
            {
                this.User = user;
            }

            public string User
            {
                get
                {
                    // if(string.IsNullOrEmpty(_domain) || string.IsNullOrEmpty(_username))
                    if (string.IsNullOrEmpty(_username))
                        return string.Empty;

                    if (string.IsNullOrEmpty(_domain))
                        return _username;

                    return _domain + @"\" + _username;
                }
                set
                {
                    string user = value;
                    if (user == null)
                    {
                        _username = string.Empty;
                        return;
                    }

                    if (user.Contains(@"\") == false)
                    {
                        _username = user;
                        return;
                    }

                    int index = user.IndexOf(@"\");
                    _domain = user.Substring(0, index);
                    _username = user.Substring(index + 1);
                }
            }

            public string Domain
            {
                get { return _domain; }
            }

            public string Username
            {
                get { return _username; }
            }
        }

        public class Options
        {
            private readonly List<DomainUser> _domainUsers;

            public Options()
            {
                _domainUsers = new List<DomainUser>();

                Type = ShareType.DiskDrive;
                MaximumAllowed = 10;
            }

            public string Path { get; set; }
            public string Name { get; set; }
            public ShareType Type { get; set; }
            public uint MaximumAllowed { get; set; }
            public string Description { get; set; }
            public string Password { get; set; }


            public AccessMaskType Privilege { get; set; }

            public List<DomainUser> DomainUsers
            {
                get { return _domainUsers; }
                set
                {
                    Debug.Assert(value != null);

                    foreach (DomainUser user in value)
                    {
                        _domainUsers.Add(user);
                    }
                }
            }

            public void AddUsers(IEnumerable<string> users)
            {
                Debug.Assert(users != null);

                foreach (string user in users)
                {
                    _domainUsers.Add(new DomainUser(user));
                }
            }
        }

        public static MethodStatus Create(Options options) // string path, string name, ShareType type, uint maximumAllowed, string description, string password)
        {
            MethodStatus shareStatus = Share(options);
            if(shareStatus != MethodStatus.Success)
                return shareStatus;

            return GrantPermission(options);
        }

        private static MethodStatus Share(Options options)
        {
            using (ManagementClass mc = new ManagementClass("Win32_Share"))
            {
                object[] parameters = new object[]
                                          {
                                              options.Path
                                              , options.Name
                                              , (uint) options.Type
                                              , options.MaximumAllowed
                                              , options.Description
                                              , options.Password
                                              , null
                                          };

                object result = mc.InvokeMethod("Create", parameters);
                uint r = Convert.ToUInt32(result);

                return (MethodStatus)r;
            }
        }

        private static MethodStatus GrantPermission(Options options)
        {
            foreach (DomainUser domainUser in options.DomainUsers)
            {
                MethodStatus status = GrantPermission(domainUser, options.Name, options.Description, options.Privilege, options);
                if (status != MethodStatus.Success)
                    return status;
            }

            return MethodStatus.Success;
        }

        //! \ref http://blogs.msdn.com/b/helloworld/archive/2008/06/06/programmatically-configuring-permissions-on-a-share-in-c.aspx?PageIndex=2
        private static MethodStatus GrantPermission(DomainUser domainUser, string sharedFolderName, string description, AccessMaskType accessMask, Options options)
        {
            if (string.IsNullOrEmpty(domainUser.User))
                return MethodStatus.Success;

            // Create a WMI instance of the principal (Win32_Trustee). 
            NTAccount account = new NTAccount(domainUser.Domain, domainUser.Username);
            SecurityIdentifier sid = (SecurityIdentifier)account.Translate(typeof(SecurityIdentifier));
            byte[] sidArray = new byte[sid.BinaryLength];
            sid.GetBinaryForm(sidArray, 0);

            using (ManagementObject trustee = new ManagementClass(new ManagementPath("Win32_Trustee"), null))
            {
                trustee["Domain"] = domainUser.Domain;
                trustee["Name"] = domainUser.Username;
                trustee["SID"] = sidArray;

                // Create a WMI instance of Win32_Ace, assign the Trustee to this Win32_Ace instance.
                using (ManagementObject adminAce = new ManagementClass(new ManagementPath("Win32_Ace"), null))
                {
                    adminAce["AccessMask"] = Convert.ToUInt32(accessMask);
                    adminAce["AceFlags"] =
                        Convert.ToUInt32(AceFlagsType.OBJECT_INHERIT_ACE | AceFlagsType.CONTAINER_INHERIT_ACE);
                    adminAce["AceType"] = Convert.ToUInt32(AceType.AccessAllowed);
                    adminAce["Trustee"] = trustee;

                    // Create a WMI instance of the security descriptor (Win32_SecurityDescriptor)
                    using (ManagementObject secDescriptor = new ManagementClass(new ManagementPath("Win32_SecurityDescriptor"), null))
                    {
                        secDescriptor["ControlFlags"] = 4; //SE_DACL_PRESENT
                        secDescriptor["DACL"] = new object[] { adminAce };

                        /*
                        using (ManagementClass mc = new ManagementClass("Win32_Share"))
                        {
                            object[] parameters = new object[]
                                          {
                                              options.Path
                                              , options.Name
                                              , (uint) options.Type
                                              , options.MaximumAllowed
                                              , options.Description
                                              , options.Password
                                              , secDescriptor
                                          };

                            object result = mc.InvokeMethod("Create", parameters);
                            uint r = Convert.ToUInt32(result);

                            return (MethodStatus)r;
                        }
                         * */

                        // Now, create a WMI instance of Win32_Share, and setup the security.
                        string managementPath = string.Format(@"\\{0}\root\cimv2:Win32_Share.Name='{1}'", Environment.MachineName, sharedFolderName);
                        using (ManagementObject share = new ManagementObject(managementPath))
                        {
                            object result = share.InvokeMethod("SetShareInfo",
                                                               new object[] { Int32.MaxValue, description, secDescriptor });
                            uint r = Convert.ToUInt32(result);

                            return (MethodStatus)r;
                        }
                    }
                }
            }
        }

        // TODO: Implement here GetAccessMask and SetShareInfo similarly to the above
        #endregion

        public static IList<Win32Share> GetAllShares()
        {
            IList<Win32Share> result = new List<Win32Share>();

            using (ManagementClass mc = new ManagementClass("Win32_Share"))
            {
                using (ManagementObjectCollection moc = mc.GetInstances())
                {
                    foreach (ManagementObject mo in moc)
                    {
                        Win32Share share = new Win32Share(mo);
                        result.Add(share);
                    }
                }
            }

            return result;
        }

        public static Win32Share GetNamedShare(string name)
        {
            // Not a very efficient implementation obviously, but heck... This is sample code. ;)
            IList<Win32Share> shares = GetAllShares();

            foreach (Win32Share s in shares)
            {
                if (s.Name == name)
                    return s;

                s.Dispose();
            }

            return null;
        }
    }
}
