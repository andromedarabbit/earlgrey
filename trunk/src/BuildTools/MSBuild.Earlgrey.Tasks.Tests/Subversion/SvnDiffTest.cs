using System;
using System.IO;

using MSBuild.Earlgrey.Tasks.Subversion;
using NUnit.Framework;


namespace MSBuild.Earlgrey.Tasks.Tests.Subversion
{
    [TestFixture]
    public class SvnDiffWithXmlSummaryTests
    {
        [Test]
        public void ExecuteWithNoArgumentsWillFail()
        {
            SvnDiff info = new SvnDiff();
            info.BuildEngine = new MSBuild.Community.Tasks.Tests.MockBuild();
            Assert.IsFalse(info.Execute());
        }

        [Test]
        public void AddedOrModified()
        {
            SvnDiff info = new SvnDiff();
            info.Old = "https://earlgrey.googlecode.com/svn/trunk" + "@456";
            info.New = "https://earlgrey.googlecode.com/svn/trunk" + "@457";
            info.BuildEngine = new MSBuild.Community.Tasks.Tests.MockBuild();

            Assert.IsTrue(info.Execute());

            Assert.AreEqual(19, info.ItemsAdded.Length);
            Assert.AreEqual(11, info.FilesAdded.Length);
            Assert.AreEqual(8, info.FoldersAdded.Length);

            Assert.AreEqual(0, info.ItemsDeleted.Length);
            Assert.AreEqual(0, info.FilesDeleted.Length);
            Assert.AreEqual(0, info.FoldersDeleted.Length);
            
            Assert.AreEqual(9, info.ItemsModified.Length);
            Assert.AreEqual(9, info.FilesModified.Length);
            Assert.AreEqual(0, info.FoldersModified.Length);
        }
    }
}
