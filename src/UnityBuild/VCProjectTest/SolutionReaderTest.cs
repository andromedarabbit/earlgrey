using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace VCProjectTest
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
                , @"..\..\..\..\Earlgrey.sln"
            );
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
