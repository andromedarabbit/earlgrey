using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks.IO;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.IO
{
    [TestFixture]
    public class SharedFolderTest
    {
        private static readonly string SharedFolderName;

        static SharedFolderTest()
        {
            SharedFolderName = "MySharedFolderInWonderland";
        }

        private static string LocalPath
        {
            get
            {
                return Path.Combine(
                    TaskUtility.ThisAssemblyDirectory
                    , "MySharedFolderForTesting"
                );
                // return @"d:\a";
                // return Path.GetTempPath();
            }
        }

        private static DeleteSharedFolder CreateDelete()
        {
            DeleteSharedFolder delete = new DeleteSharedFolder();
            delete.BuildEngine = new MockBuildEngine();

            delete.TreatAsErrorWhenNotExist = false;
            delete.DeleteLocalFolder = true;
            delete.Name = SharedFolderName;
            return delete;
        }

        private static ShareFolder CreateShare()
        {
            ShareFolder share = new ShareFolder();
            share.BuildEngine = new MockBuildEngine();

            share.LocalFolder = new TaskItem(LocalPath);
            share.Name = SharedFolderName;
            share.ResetExistingOne = false;
            share.MakeFolderWhenNotExist = true;
            return share;
        }

        [Test]
        [ExpectedException(typeof(UnauthorizedAccessException))]
        public void ShareWithoutAuthentication()
        {
            try
            {
                ShareFolder share = CreateShare();
                // share.ResetExistingOne = true;
                Assert.IsTrue(share.Execute());

                string textFile = Path.Combine(share.Uri, "ShareWithoutAuthenticationTest.txt");
                using (StreamWriter sw = File.CreateText(textFile))
                {
                    sw.WriteLine("Wow!");
                }
            }
            finally
            {
                DeleteSharedFolder delete = CreateDelete();
                Assert.IsTrue(delete.Execute());
            }
        }

        [Test]
        [ExpectedException(typeof(UnauthorizedAccessException))]
        public void ShareWithReadPermissionOnly()
        {
            try
            {
                ShareFolder share = CreateShare();
                share.Privilege = Enum.GetName(typeof(ShareFolder.Permission), ShareFolder.Permission.Read);
                // share.ResetExistingOne = true;
                Assert.IsTrue(share.Execute());

                string textFile = Path.Combine(share.Uri, "ShareWithoutAuthenticationTest.txt");
                using (StreamWriter sw = File.CreateText(textFile))
                {
                    sw.WriteLine("Wow!");
                }
            }
            finally
            {
                DeleteSharedFolder delete = CreateDelete();
                Assert.IsTrue(delete.Execute());
            }
        }

        [Test]
        public void ShareWIthChangePermission()
        {
            try
            {
                ShareFolder share = CreateShare();
                // share.ResetExistingOne = true;
                share.Privilege = Enum.GetName(typeof(ShareFolder.Permission), ShareFolder.Permission.Change);
                share.Users = new string[] 
                                {
                                    string.Format(@"{0}\{1}", Environment.UserDomainName, Environment.UserName)
                                };

                Assert.IsTrue(share.Execute());

                string textFile = Path.Combine(share.Uri, "ShareWithoutAuthenticationTest.txt");
                using (FileStream fs = File.Open(textFile, FileMode.CreateNew, FileAccess.Write, FileShare.None))
                {
                    using (StreamWriter sw = new StreamWriter(fs))
                    {
                        sw.WriteLine("Wow!");
                    }
                }

                File.Exists(textFile);
            }
            finally
            {
                DeleteSharedFolder delete = CreateDelete();
                Assert.IsTrue(delete.Execute());
            }
        }

        [Test]
        public void ShareWIthFullPermission()
        {
            try
            {
                ShareFolder share = CreateShare();
                share.ResetExistingOne = true;
                share.Privilege = Enum.GetName(typeof(ShareFolder.Permission), ShareFolder.Permission.Full); ;
                share.Users = new string[]
                                  {
                                      string.Format(@"{0}\{1}", Environment.UserDomainName, Environment.UserName)
                                  };
                Assert.IsTrue(share.Execute());

                string textFile = Path.Combine(share.Uri, "ShareWithoutAuthenticationTest.txt");
                using (StreamWriter sw = File.CreateText(textFile))
                {
                    sw.WriteLine("Wow!");
                }

                File.Exists(textFile);
            }
            finally
            {
                DeleteSharedFolder delete = CreateDelete();
                Assert.IsTrue(delete.Execute());
            }
        }



        [Test]
        public void NotMakeFolderWhenNotExist()
        {
            try
            {
                if (Directory.Exists(LocalPath))
                    Directory.Delete(LocalPath);

                ShareFolder share = CreateShare();
                share.MakeFolderWhenNotExist = false;
                Assert.IsFalse(share.Execute());
            }
            finally
            {
                DeleteSharedFolder delete = CreateDelete();
                Assert.IsTrue(delete.Execute());
            }
        }

        [Test]
        public void DeleteLocalFolderAutomatically()
        {
            try
            {
                ShareFolder share = CreateShare();
                share.ResetExistingOne = true;
                share.Users = new string[]             
                    {
                        string.Format(@"{0}\{1}", Environment.UserDomainName, Environment.UserName)
                    };
                Assert.IsTrue(share.Execute());
            }
            finally
            {
                DeleteSharedFolder delete = CreateDelete();
                delete.DeleteLocalFolder = true;

                Assert.IsTrue(delete.Execute());

                Assert.IsFalse(Directory.Exists(LocalPath));
            }
        }

        [Test]
        public void TreatAsErrorWhenNotExist()
        {
            DeleteSharedFolder delete = new DeleteSharedFolder();
            delete.BuildEngine = new MockBuildEngine();

            delete.Name = "SharedFolderInWonderland";
            delete.TreatAsErrorWhenNotExist = true;

            Assert.IsFalse(delete.Execute());
        }

        [Test]
        public void NotTreatAsErrorWhenNotExist()
        {
            DeleteSharedFolder delete = new DeleteSharedFolder();
            delete.BuildEngine = new MockBuildEngine();

            delete.Name = "SharedFolderInWonderland";
            delete.TreatAsErrorWhenNotExist = false;

            Assert.IsTrue(delete.Execute());
        }


    }
}
