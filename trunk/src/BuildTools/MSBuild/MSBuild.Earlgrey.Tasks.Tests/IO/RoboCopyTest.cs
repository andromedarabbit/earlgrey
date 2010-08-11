using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks.IO;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.IO
{
    [TestFixtureAttribute]
    public class RoboCopyTest
    {
        private static readonly string SrcDir;
        private static readonly string DstDir;

        static RoboCopyTest()
        {
            SrcDir = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"IO\RoboCopySrcFolder1"
                );
            DstDir = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"IO\RoboCopyDstFolder1"
                );
        }

        [SetUp]
        public void SetUp()
        {
            if (Directory.Exists(DstDir))
                Directory.Delete(DstDir, true);
        }

        [Test]
        public void CopyFileWithParentDirectory()
        {
            RoboCopy roboCopy = new RoboCopy();
            roboCopy.BuildEngine = new MockBuildEngine();

            Assert.IsTrue(Directory.Exists(SrcDir));
            Assert.IsFalse(Directory.Exists(DstDir));

            roboCopy.SourceDir = new TaskItem(SrcDir);
            roboCopy.DestinationDir = new TaskItem(DstDir);
            roboCopy.Files = new ITaskItem[]
                                 {
                                     new TaskItem("*.*")
                                 };
            Assert.IsTrue(roboCopy.Execute());
            Assert.IsTrue(Directory.Exists(DstDir));
        }
    }
}
