using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace SLNTools.Tests
{
    using NUnit.Framework;
    using CWDev.SLNTools.Core;

    // [TestFixtureAttribute]
    public abstract class AbstractSolutionFileWriterTest
    {
        //private static readonly string SolutionFileFullPath;
        //private static readonly string NewSolutionFileFullPath;

        //static AbstractSolutionFileWriterTest()
        //{
        //    SolutionFileFullPath = Path.Combine(
        //        AppDomain.CurrentDomain.BaseDirectory
        //        , @"..\..\..\Earlgrey.sln"
        //    );
        //    SolutionFileFullPath = Path.GetFullPath(SolutionFileFullPath);

        //    NewSolutionFileFullPath = Path.Combine(
        //        AppDomain.CurrentDomain.BaseDirectory
        //        , @"..\..\..\NewEarlgrey.sln"
        //    );
        //    NewSolutionFileFullPath = Path.GetFullPath(NewSolutionFileFullPath);
        //}

        protected abstract string SolutionFileFullPath
        {
            get;
        }

        protected abstract string NewSolutionFileFullPath
        {
            get;
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

        private void CleanNewSolutionFIle()
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
