using System;
using System.IO;
using System.Linq;
using Microsoft.Build.Utilities;
using MSBuild.Earlgrey.Tasks.Subversion;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.Subversion
{
    [TestFixture]
    public class SvnDiffTests
    {
        [Test]
        public void ExecuteWithNoArgumentsWillFail()
        {
            SvnDiff diff = new SvnDiff();
            diff.BuildEngine = new MockBuildEngine();
            Assert.IsFalse(diff.Execute());
        }

        [Test]
        public void AddedOrModified()
        {
            SvnDiff diff = new SvnDiff();
            diff.Old = "https://earlgrey.googlecode.com/svn/trunk" + "@456";
            diff.New = "https://earlgrey.googlecode.com/svn/trunk" + "@457";
            diff.BuildEngine = new MockBuildEngine();

            Assert.IsTrue(diff.Execute());

            Assert.AreEqual(19, diff.ItemsAdded.Length);
            Assert.AreEqual(11, diff.FilesAdded.Length);
            Assert.AreEqual(8, diff.FoldersAdded.Length);

            Assert.AreEqual(0, diff.ItemsDeleted.Length);
            Assert.AreEqual(0, diff.FilesDeleted.Length);
            Assert.AreEqual(0, diff.FoldersDeleted.Length);
            
            Assert.AreEqual(9, diff.ItemsModified.Length);
            Assert.AreEqual(9, diff.FilesModified.Length);
            Assert.AreEqual(0, diff.FoldersModified.Length);
        }

        [Test]
        public void HandlePathContainingKoreanLetters()
        {
            SvnDiff diff = new SvnDiff();
            diff.Old = "https://earlgrey.googlecode.com/svn/trunk" + "@459";
            diff.New = "https://earlgrey.googlecode.com/svn/trunk" + "@460";
            diff.BuildEngine = new MockBuildEngine();

            Assert.IsTrue(diff.Execute());
            Assert.AreEqual(2, diff.ItemsAdded.Length);
           
            var paths = from item in diff.ItemsAdded
                        select item.ItemSpec;

            CollectionAssert.Contains(paths,
                                      @"https://earlgrey.googlecode.com/svn/trunk/src/BuildTools/MSBuild.Earlgrey.Tasks.Tests/Subversion/Sample/한글 폴더"
                                      );
            CollectionAssert.Contains(paths,
                                       @"https://earlgrey.googlecode.com/svn/trunk/src/BuildTools/MSBuild.Earlgrey.Tasks.Tests/Subversion/Sample/한글 폴더/한글 파일.txt"
                                       );
        }
    }
}
