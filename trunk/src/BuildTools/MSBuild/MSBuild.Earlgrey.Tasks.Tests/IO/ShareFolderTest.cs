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
    public class ShareFolderTest
    {
        private static readonly string SharedFolderName;

        static ShareFolderTest()
        {
            SharedFolderName = "SharedFolderInWonderland";
        }

        public static string LocalPath
        {
            get
            {
                return Path.Combine(
                    TaskUtility.ThisAssemblyDirectory
                    , "SharedFolderForTesting"
                );
            }
        }

        [Test]
        public void NotMakeFolderWhenNotExist()
        {
            try
            {
                if(Directory.Exists(LocalPath))
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
        public void ShareAFolder()
        {
            try
            {            
                ShareFolder share = CreateShare();
                Assert.IsTrue(share.Execute());
            }
            finally
            {
                DeleteSharedFolder delete = CreateDelete();
                Assert.IsTrue(delete.Execute());

                Assert.IsFalse(Directory.Exists(LocalPath));
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
    }
}
