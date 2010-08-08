// $Id: SvnCopyTest.cs 305 2007-02-25 18:11:59Z joshuaflanagan $

using MSBuild.Community.Tasks.Subversion;
using NUnit.Framework;

namespace MSBuild.Community.Tasks.Tests.Subversion
{
    [TestFixture]
    public class SvnCopyTest
    {

        [Test]
        public void SvnCopyExecute()
        {
            SvnCopy task = new SvnCopy();
            task.Message = "Tagging";
            task.SourcePath = "file:///d:/svn/trunk/path";
            task.DestinationPath = "file:///d:/svn/tags/release/path";
            string expectedCommand = "copy \"file:///d:/svn/trunk/path\" \"file:///d:/svn/tags/release/path\" --message \"Tagging\" --non-interactive --no-auth-cache";
            string actualCommand = TaskUtility.GetToolTaskCommand(task);
            Assert.AreEqual(expectedCommand, actualCommand);
        }
    }
}
