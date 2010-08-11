using System;
using System.Collections.Generic;
using System.Linq;
using System.Management;
using System.Text;

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

        public uint AccessMask
        {
            get { return Convert.ToUInt32(_winShareObject["AccessMask"]); }
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

        public static MethodStatus Create(string path, string name, ShareType type, uint maximumAllowed, string description, string password)
        {
            using (ManagementClass mc = new ManagementClass("Win32_Share"))
            {
                object[] parameters = new object[]
                                          {path, name, (uint) type, maximumAllowed, description, password, null};
                
                object result = mc.InvokeMethod("Create", parameters);
                uint r = Convert.ToUInt32(result);

                return (MethodStatus) r;
            }
        }

        // TODO: Implement here GetAccessMask and SetShareInfo similarly to the above
        #endregion

        public static IList<Win32Share> GetAllShares()
        {
            IList<Win32Share> result = new List<Win32Share>();
            
            using(ManagementClass mc = new ManagementClass("Win32_Share"))
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
