using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class VcSolutionFileCopyTest : AbstractTest
    {
        [Test]
        public void Copy()
        {
            // const string filePath = @"d:\Workspace\earlgrey-test\src\Earlgrey.sln";
            VcSolutionFileCopy copy = new VcSolutionFileCopy(SolutionFilePath);
            copy.Copy();
        }
    }
}
