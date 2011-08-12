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
    public class WinScpDownloadTest
    {
        [Ignore]
        [Test]
        public void Download()
        {
            WinScpUploadTest uploadTest = new WinScpUploadTest();
            uploadTest.InternalUploadTest();



            WinScpDownload instance = new WinScpDownload();
            WinScpTestHelper.Initialize(instance);
           

            List<TaskItem> files = new List<TaskItem>();


            TaskItem winScpTest = new TaskItem();
            winScpTest.ItemSpec = "./EarlGreyTest";
            winScpTest.SetMetadata(WinScpDownload.CreateLocalFolderKeyName, "true");
            winScpTest.SetMetadata(
                WinScpDownload.LocalDirKeyName
                , Path.Combine(TaskUtility.ThisAssemblyDirectory, @"Temp\Net\Temporary\WinScpTestDownloaded")
                );
            files.Add(winScpTest);

            instance.Files = files.ToArray();

            Assert.IsTrue(instance.Execute());
            Assert.IsTrue(
                File.Exists(
                    Path.Combine(
                        TaskUtility.ThisAssemblyDirectory
                        , @"Temp\Net\Temporary\WinScpTestDownloaded\WinScpTest\TextFile10.txt"
                        )
                    )
                )
            ;

            Directory.Delete(
                Path.Combine(TaskUtility.ThisAssemblyDirectory, @"Temp\Net\Temporary\WinScpTestDownloaded")
                , true
                );
        }
    }
}
