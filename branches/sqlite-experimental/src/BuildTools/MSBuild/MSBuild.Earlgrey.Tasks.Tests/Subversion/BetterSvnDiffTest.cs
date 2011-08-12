using System;
using System.IO;
using System.Linq;
using Earlgrey;
using MSBuild.Earlgrey.Tasks.Subversion;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.Subversion
{
    [TestFixture]
    public class BetterSvnDiffTest
    {
 
        [Test]
        public void SetLocalPathWithoutSettingResolveLocalPaths()
        {
            string localPath = Path.GetFullPath(
                Path.Combine(TaskUtility.ThisAssemblyDirectory, @"..\..\")
                );

            BetterSvnDiff diff = new BetterSvnDiff();
            diff.Old = "https://earlgrey.googlecode.com/svn/trunk" + "@456";
            diff.New = "https://earlgrey.googlecode.com/svn/trunk" + "@457";
            diff.BuildEngine = new MockBuildEngine();
            diff.LocalPath = localPath;
            diff.ResolveLocalPaths = false;
            Assert.IsTrue(diff.Execute());
        }

        [Test]
        public void SetResolveLocalPathsWithoutSettingLocalPath()
        {
            BetterSvnDiff diff = new BetterSvnDiff();
            diff.Old = "https://earlgrey.googlecode.com/svn/trunk" + "@456";
            diff.New = "https://earlgrey.googlecode.com/svn/trunk" + "@457";
            diff.BuildEngine = new MockBuildEngine();
            diff.ResolveLocalPaths = true;
            Assert.IsFalse(diff.Execute());
        }

        [Test]
        public void ResolveLocalPaths()
        {
            string localPath = Path.GetFullPath(
                Path.Combine(TaskUtility.ThisAssemblyDirectory, @"..\..\")
                );

            BetterSvnDiff diff = new BetterSvnDiff();
            diff.Old = "https://earlgrey.googlecode.com/svn/trunk" + "@456";
            diff.New = "https://earlgrey.googlecode.com/svn/trunk" + "@457";
            diff.BuildEngine = new MockBuildEngine();
            diff.LocalPath = localPath;
            diff.ResolveLocalPaths = true;
            diff.OldIsBasePath = false;
            Assert.IsTrue(diff.Execute());

            // 로컬에는 @HEAD 가 있을 테니 파일이 실제로 있는지 확인해봐야 소용 없다.
            Assert.AreEqual(
                diff.FilesAdded.Length,
                diff.FilesAdded.Count(
                    item => item.GetMetadata("LocalPath").Length > 0
                // item => File.Exists(item.GetMetadata("LocalPath")) == true
                    )
                );
        }

    }
}
