using System;
using System.Collections.Generic;
using System.Linq;
using System.IO;
using Earlgrey;
using Microsoft.Build.Framework;
using MSBuild.Earlgrey.Tasks.IO;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.IO
{
    [TestFixtureAttribute]
    public class ForFilesTest
    {
    
        [Test]
        public void SimpleExecution()
        {
            ForFiles instance = new ForFiles();
            instance.BuildEngine = new MockBuildEngine();            

            string dir = Path.Combine(BuildScripts.TestRootFolder, @"Net\WinScpUploadTest");

            instance.PathName = dir;
            instance.Recursive = true;
                       
            Assert.IsTrue(instance.Execute());

            ITaskItem[] filesFound = instance.FilesFound;
            Assert.IsNotEmpty(filesFound);
        }

        [Test]
        public void GetFullPaths()
        {
            ForFiles instance = new ForFiles();
            instance.BuildEngine = new MockBuildEngine();

            string dir = Path.Combine(BuildScripts.TestRootFolder, @"Net\WinScpUploadTest");

            instance.PathName = dir;
            instance.Recursive = true;
            instance.Command = "cmd /c echo @path";

            Assert.IsTrue(instance.Execute());

            ITaskItem[] filesFound = instance.FilesFound;
            Assert.IsNotEmpty(filesFound);
        }
    }
}
