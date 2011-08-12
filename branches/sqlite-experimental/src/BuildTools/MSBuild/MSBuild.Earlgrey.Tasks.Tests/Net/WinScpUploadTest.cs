using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks.Net;
using NUnit.Framework;


namespace MSBuild.Earlgrey.Tasks.Tests.Net
{
    [TestFixture]
    public class WinScpUploadTest
    {
        // WinScpDownloadTest 에서 함께 테스트함
        [Ignore]
        [Test]
        public void Upload()
        {
            InternalUploadTest();

            WinScpRmDirTest rmDirTest = new WinScpRmDirTest();
            rmDirTest.InternRmDirTest();
        }

        internal void InternalUploadTest()
        { 

            WinScpUpload instance = new WinScpUpload();
            WinScpTestHelper.Initialize(instance);


            List<TaskItem> files = new List<TaskItem>();

            //  public const string RemoteDirKeyName = "RemoteDir";
            // public const string SwitchesKeyName = "PutSwitches";
            TaskItem winScpTest = new TaskItem();
            winScpTest.ItemSpec = Path.Combine(TaskUtility.ThisAssemblyDirectory, @"Temp\Net\WinScpTest");
            winScpTest.SetMetadata(WinScpUpload.CreateRemoteFolderKeyName, "true");
            winScpTest.SetMetadata(WinScpUpload.RemoteDirKeyName, "./EarlGreyTest/WinScpTest");
            files.Add(winScpTest);

            TaskItem winScpUploadTest = new TaskItem();
            winScpUploadTest.ItemSpec = Path.Combine(TaskUtility.ThisAssemblyDirectory, @"Temp\Net\WinScpUploadTest");
            winScpUploadTest.SetMetadata(WinScpUpload.RemoteDirKeyName, "./EarlGreyTest/");
            files.Add(winScpUploadTest);


            instance.Files = files.ToArray();


            Assert.IsTrue(instance.Execute());
        }
    }
}
