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
    public class BetterBetterRoboCopyTest
    {
        private static readonly string SrcDir;
        private static readonly string DstDir;

        static BetterBetterRoboCopyTest()
        {
            SrcDir = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"IO\BetterRoboCopySrcFolder1"
                );
            DstDir = Path.Combine(
                TaskUtility.ThisAssemblyDirectory
                , @"IO\BetterRoboCopyDstFolder1"
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
            BetterRoboCopy roboCopy = new BetterRoboCopy();
            roboCopy.BuildEngine = new MockBuildEngine();

            Assert.IsTrue(Directory.Exists(SrcDir));
            Assert.IsFalse(Directory.Exists(DstDir));

            roboCopy.SourceFolder = SrcDir;
            roboCopy.DestinationFolder = DstDir;
            roboCopy.SourceFiles = new []
                                 {
                                     "*.*"
                                 };
            Assert.IsTrue(roboCopy.Execute());
            Assert.IsTrue(Directory.Exists(DstDir));
        }

        [Test]
        public void SucessWhenSourceFileNotFound()
        {
            BetterRoboCopy roboCopy = new BetterRoboCopy();
            roboCopy.BuildEngine = new MockBuildEngine();

            const string srcFileName = "NOT_EXISTS____.txt";

            roboCopy.SourceFolder = SrcDir;
            roboCopy.DestinationFolder = DstDir;
            roboCopy.SourceFiles = new []
                                 {
                                     srcFileName
                                 };

            // 다소 이상하지만 robocopy 자체가 이런 식으로 구현됐음
            Assert.IsTrue(roboCopy.Execute());
        }
    }
}
