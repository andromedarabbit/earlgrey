using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Earlgrey;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks.IO.Compression;
using MSBuild.Earlgrey.Tasks.IO.Compression.Zip;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.IO.Compression.Zip
{
    [TestFixture]
    public class ZipTest
    {
        private static readonly string TempDir;
        private static readonly string ZipFilePath;

        static ZipTest()
        {
            TempDir = Path.Combine(BuildScripts.TestRootFolder, @"PackTest\Temp");
            ZipFilePath = Path.Combine(TempDir, "PackTest.Zip");
        }

        [SetUp]
        public void SetUp()
        {
            CleanUp();
        }

        [TearDown]
        public void TearDown()
        {
            CleanUp();
        }

        private static void CleanUp()
        {
            if (Directory.Exists(TempDir))
                Directory.Delete(TempDir, true);

            if (File.Exists(ZipFilePath))
                File.Delete(ZipFilePath);
        }

        [Test]
        public void PackWithRootDir()
        {
            PackFolderIo(true);
        }

        [Test]
        public void PackWithoutRootDir()
        {
            PackFolderIo(false);
        }

        [Test]
        public void NotOverwrite()
        {
            string srcFolder = Path.Combine(BuildScripts.TestScriptsFolder, @"IO");
            Assert.IsTrue( PackFolder(srcFolder, true, false) );
            Assert.IsFalse( PackFolder(srcFolder, true, false) );
        }

        [Test]
        public void Overwrite()
        {
            string srcFolder = Path.Combine(BuildScripts.TestScriptsFolder, @"IO");
            Assert.IsTrue(PackFolder(srcFolder, true, false));
            Assert.IsTrue(PackFolder(srcFolder, true, true));
        }

        [Test]
        public void PackWithExcludePatterns()
        {
            string srcFolder = Path.Combine(BuildScripts.TestScriptsFolder, @"IO");
            string[] excludes = new []
                                    {
                                        "IniFile*"
                                    };
            
            const bool containsRootDir = true;
            Assert.IsTrue(PackFolder(srcFolder, containsRootDir, false, excludes));

            Unpack();

            string unzipRootDirExpected = Path.Combine(TempDir, @"IO");
            Assert.IsTrue(Directory.Exists(unzipRootDirExpected));

            foreach(string exclude in excludes)
            {
                Assert.IsFalse(HasSuchFiles(unzipRootDirExpected, exclude));
            }
            Assert.IsTrue(HasSuchFiles(unzipRootDirExpected, "ZipPack.msbuild.xml"));
        }

        private static bool HasSuchFiles(string rootDir, string searchPattern)
        {
            string[] files = Directory.GetFiles(rootDir, searchPattern, SearchOption.AllDirectories);
            return files != null
                   && files.Length > 0
                   ;
        }

        private static void PackFolderIo(bool containsRootDir)
        {
            string srcFolder = Path.Combine(BuildScripts.TestScriptsFolder, @"IO");
            Assert.IsTrue( PackFolder(srcFolder, containsRootDir, false) );

            Unpack();

            string unzipRootDirExpected = TempDir;
            if (containsRootDir)
            {
                unzipRootDirExpected = Path.Combine(TempDir, @"IO");
            }

            Assert.IsTrue(Directory.Exists(unzipRootDirExpected));
        }

        private static bool PackFolder(string srcFolder, bool containsRootDir, bool overwrite)
        {
            return PackFolder(srcFolder, containsRootDir, overwrite, null);
        }

        private static bool PackFolder(string srcFolder, bool containsRootDir, bool overwrite, string[] excludes)
        {
            Pack pack = new Pack();
            pack.BuildEngine = new MockBuildEngine();

            pack.ContainsRootDir = containsRootDir;
            pack.Overwrite = overwrite;

            pack.SrcFolder = new TaskItem(srcFolder);
            pack.ZipFilePath = new TaskItem(ZipFilePath);

            pack.Excludes = excludes;

            if( pack.Execute() == false)
                return false;

            if (containsRootDir)
                Assert.IsTrue(ZipFileHelper.HasRootDir(ZipFilePath));
            else
                Assert.IsFalse(ZipFileHelper.HasRootDir(ZipFilePath));

            return true;
        }

        private static void Unpack()
        {
            Unpack unpack = new Unpack();
            unpack.BuildEngine = new MockBuildEngine();
            unpack.ZipFilePath = new TaskItem(ZipFilePath);
            unpack.TargetDir = new TaskItem(TempDir);
            unpack.Overwrite = true;

            Assert.IsTrue(unpack.Execute());
        }

    }
}
