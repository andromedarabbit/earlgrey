using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild.Tests
{
    using NUnit.Framework;

    [TestFixture]
    public class SolutionReaderTest : AbstractTest
    {
        [Test]
        public void ReadSolutionFileTest()
        {
            using (var sr = new StreamReader(SolutionFilePath, true))
            {
                string text = sr.ReadToEnd();
                Assert.IsNotEmpty(text);
            }
        }

    }
}
