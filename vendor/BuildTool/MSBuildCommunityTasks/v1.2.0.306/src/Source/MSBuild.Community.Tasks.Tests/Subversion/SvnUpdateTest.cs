// $Id: SvnUpdateTest.cs 301 2007-02-17 22:26:16Z joshuaflanagan $

using System;
using MSBuild.Community.Tasks.Subversion;
using NUnit.Framework;

namespace MSBuild.Community.Tasks.Tests.Subversion
{
    [TestFixture]
    public class SvnUpdateTest
    {
        [Test]
        public void SvnUpdateCommand()
        {
            SvnUpdate task = new SvnUpdate();
            string localPath = @"c:\code";
            task.LocalPath = localPath;

            string expectedCommand = String.Format("update \"{0}\" --non-interactive --no-auth-cache", localPath);
            string actualCommand = TaskUtility.GetToolTaskCommand(task);
            Assert.AreEqual(expectedCommand, actualCommand);
        }
    }
}