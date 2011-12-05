using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace SLNTools.Tests
{
    using NUnit.Framework;
    using CWDev.SLNTools.Core;

    [TestFixtureAttribute]
    public class SolutionFileWriterTest
    {
        private static readonly string SolutionFileFullPath;
        private static readonly string NewSolutionFileFullPath;

        static SolutionFileWriterTest()
        {
            SolutionFileFullPath = Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory
                , @"..\..\..\Earlgrey.sln"
            );
            SolutionFileFullPath = Path.GetFullPath(SolutionFileFullPath);

            NewSolutionFileFullPath = Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory
                , @"..\..\..\NewEarlgrey.sln"
            );
            NewSolutionFileFullPath = Path.GetFullPath(NewSolutionFileFullPath);
        }

        [SetUp]
        public void Init()
        {
            CleanNewSolutionFIle();
        }

        [TearDown]
        public void Cleanup()
        {
            CleanNewSolutionFIle();
        }

        private static void CleanNewSolutionFIle()
        {
            if(File.Exists(NewSolutionFileFullPath))
                File.Delete(NewSolutionFileFullPath);
        }


        [Test]
        public void WriteSolutionFileTest()
        {
            Assert.IsTrue(File.Exists(SolutionFileFullPath));
            Assert.IsFalse(File.Exists(NewSolutionFileFullPath));

            using (var reader = new SolutionFileReader(SolutionFileFullPath))
            {
                SolutionFile slnFile = reader.ReadSolutionFile();
                slnFile.SolutionFullPath = SolutionFileFullPath;

                var newSlnFile = new SolutionFile(NewSolutionFileFullPath, slnFile.Headers, null, null);
                using(var writer = new SolutionFileWriter(NewSolutionFileFullPath))
                {
                    writer.WriteSolutionFile(newSlnFile);
                }
            }

            Assert.IsTrue(File.Exists(NewSolutionFileFullPath));

            string text = File.ReadAllText(NewSolutionFileFullPath);
            Assert.Greater(text.Length, 100);
        }
    }
}
