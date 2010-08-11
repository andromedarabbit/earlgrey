using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using MSBuild.Earlgrey.Tasks.IO;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.Tests.IO
{
    [TestFixture]
    public class DeleteSharedFolderTest
    {
        [Test]
        public void TreatAsErrorWhenNotExist()
        {
            DeleteSharedFolder delete = new DeleteSharedFolder();
            delete.BuildEngine = new MockBuildEngine();

            delete.Name = "SharedFolderInWonderland";
            delete.TreatAsErrorWhenNotExist = true;

            Assert.IsFalse(delete.Execute());
        }

        [Test]
        public void NotTreatAsErrorWhenNotExist()
        {
            DeleteSharedFolder delete = new DeleteSharedFolder();
            delete.BuildEngine = new MockBuildEngine();

            delete.Name = "SharedFolderInWonderland";
            delete.TreatAsErrorWhenNotExist = false;

            Assert.IsTrue(delete.Execute());
        }
    }
}
