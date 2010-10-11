using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace UnityBuild.Tests
{
    using NUnit.Framework;

    [TestFixture]
    public class SolutionReaderTest
    {
        private static readonly string SolutionFilePath;

        static SolutionReaderTest()
        {
            SolutionFilePath = Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory
                , @"..\..\..\Earlgrey.sln"
            );
            SolutionFilePath = Path.GetFullPath(SolutionFilePath);

        }

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
