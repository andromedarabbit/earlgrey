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
    public class WinScpMkDirTest
    {        
        [Ignore]
        [Test]
        public void MkDir()
        {
            WinScpMkDir instance = new WinScpMkDir();
            WinScpTestHelper.Initialize(instance);

            List<TaskItem> folders = new List<TaskItem>();

            TaskItem winScpTest = new TaskItem();
            winScpTest.ItemSpec = "./EarlGreyTest/A";
            folders.Add(winScpTest);

            TaskItem winScpUploadTest = new TaskItem();
            winScpUploadTest.ItemSpec = @"./EarlGreyTest/B/C";
            folders.Add(winScpUploadTest);


            instance.Directories = folders.ToArray();


            Assert.IsTrue(instance.Execute());
        }
    }
}
