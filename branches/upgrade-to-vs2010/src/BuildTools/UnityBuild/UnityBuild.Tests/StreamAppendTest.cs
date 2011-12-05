using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using NUnit.Framework;

namespace UnityBuild.Tests
{
    [TestFixture]
    public class StreamAppendTest : AbstractTest
    {
        [Test]
        public void MergeTextFileEmptyToNewFile()
        {
            string srcFilePath = Path.Combine(ThisDir, "TextFileEmpty.txt");
            string dstFilePath = Path.Combine(TempDir, "FileAppendTest.txt");

            Assert.IsTrue(File.Exists(srcFilePath));

            if (File.Exists(dstFilePath))
                File.Delete(dstFilePath);

            using (FileStream dst = File.Open(dstFilePath, FileMode.Create))
            {
                using (FileStream src = File.Open(srcFilePath, FileMode.Open))
                {
                    StreamAppend append = new StreamAppend(src, dst);
                    append.Merge();
                }
            }

            FileAssert.AreEqual(srcFilePath, dstFilePath);
        }


        [Test]
        public void MergeTextFileAnsiEncodedToNewFile()
        {
            string srcFilePath = Path.Combine(ThisDir, "TextFileAnsiEncoded1.txt");
            string dstFilePath = Path.Combine(TempDir, "FileAppendTest.txt");

            Assert.IsTrue(File.Exists(srcFilePath));

            if(File.Exists(dstFilePath))
                File.Delete(dstFilePath);

            using (FileStream dst = File.Open(dstFilePath, FileMode.Create))
            {            
                using(FileStream src = File.Open(srcFilePath, FileMode.Open))
                {
                    StreamAppend append = new StreamAppend(src, dst);
                    append.Merge();
                }
            }

            FileAssert.AreEqual(srcFilePath, dstFilePath);
        }

        [Test]
        public void MergeTwoTextFileAnsiEncodedToNewFile()
        {
            string srcFilePath1 = Path.Combine(ThisDir, "TextFileAnsiEncoded1.txt");
            string srcFilePath2 = Path.Combine(ThisDir, "TextFileAnsiEncoded2.txt");
            string dstFilePath = Path.Combine(TempDir, "FileAppendTest.txt");

            Assert.IsTrue(File.Exists(srcFilePath1));
            Assert.IsTrue(File.Exists(srcFilePath2));

            if (File.Exists(dstFilePath))
                File.Delete(dstFilePath);

            using (FileStream dst = File.Open(dstFilePath, FileMode.Create))
            {
                using (FileStream src = File.Open(srcFilePath1, FileMode.Open))
                {
                    StreamAppend append = new StreamAppend(src, dst);
                    append.Merge();
                }

                using (FileStream src = File.Open(srcFilePath2, FileMode.Open))
                {
                    StreamAppend append = new StreamAppend(src, dst);
                    append.Merge();
                }
            }

            string text = File.ReadAllText(dstFilePath, Encoding.Default);
            StringAssert.AreEqualIgnoringCase("베이시스트빅터 우튼", text);
        }
    }
}
