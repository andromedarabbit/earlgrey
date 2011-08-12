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
    public class WinScpRmDirTest
    {
        [Ignore]
        [Test]
        public void RmDir()
        {
            WinScpMkDirTest mkDirTest = new WinScpMkDirTest();
            mkDirTest.MkDir();

            InternRmDirTest();
        }

        internal void InternRmDirTest()
        {
            WinScpRmDir instance = new WinScpRmDir();
            WinScpTestHelper.Initialize(instance);


            List<TaskItem> folders = new List<TaskItem>();

            TaskItem winScpTest = new TaskItem();
            winScpTest.ItemSpec = "./EarlGreyTest";
            folders.Add(winScpTest);

            instance.Directories = folders.ToArray();


            Assert.IsTrue(instance.Execute());
        }
    }
}
