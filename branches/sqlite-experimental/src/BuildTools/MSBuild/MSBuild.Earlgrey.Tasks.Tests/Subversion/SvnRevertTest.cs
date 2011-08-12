using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Framework;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks.Subversion;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.Subversion
{
    [TestFixture]
    public class SvnRevertTest
    {
        [Test]
        public void RevertFileTest()
        {
            // const string repositoryPath = @"https://earlgrey.googlecode.com/svn/trunk/src/msbuild.xml";
            string srcFilePath = Path.GetFullPath(
                Path.Combine(TaskUtility.ThisAssemblyDirectory, @"..\..\UnityBuild\UnitTestSample\src\Earlgrey.sln")
                );

            string backupFilePath = srcFilePath + ".backup";

            File.Copy(srcFilePath, backupFilePath, true);
            FileAssert.AreEqual(backupFilePath, srcFilePath);

            File.AppendAllText(srcFilePath, "Temporary unit-testing text");
            FileAssert.AreNotEqual(backupFilePath, srcFilePath);

            SvnRevert cmd = new SvnRevert();
            cmd.BuildEngine = new MockBuildEngine();

            cmd.Paths = new ITaskItem[1];
            cmd.Paths[0] = new TaskItem(srcFilePath);

            Assert.IsTrue(cmd.Execute());

            FileAssert.AreEqual(backupFilePath, srcFilePath);
        }

        [Test]
        public void RevertRecursivelyTest()
        {
            string srcFilePath = Path.GetFullPath(
               Path.Combine(TaskUtility.ThisAssemblyDirectory, @"..\..\UnityBuild\UnitTestSample\src\Earlgrey.sln")
               );

            string backupFilePath = srcFilePath + ".backup";

            File.Copy(srcFilePath, backupFilePath, true);
            FileAssert.AreEqual(backupFilePath, srcFilePath);

            File.AppendAllText(srcFilePath, "Temporary unit-testing text");
            FileAssert.AreNotEqual(backupFilePath, srcFilePath);

            SvnRevert cmd = new SvnRevert();
            cmd.BuildEngine = new MockBuildEngine();

            cmd.Paths = new ITaskItem[1];
            cmd.Paths[0] = new TaskItem(Directory.GetParent(srcFilePath).FullName);

            cmd.Recursive = true;

            Assert.IsTrue(cmd.Execute());

            FileAssert.AreEqual(backupFilePath, srcFilePath);
        }

    
    }
}
