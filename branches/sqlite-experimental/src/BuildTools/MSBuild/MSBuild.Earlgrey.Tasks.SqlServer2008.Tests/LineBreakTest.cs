using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using NUnit.Framework;

namespace MSBuild.Earlgrey.Tasks.SqlServer2008.Tests
{
    [TestFixture]
    public class LineBreakTest
    {
        [Test]
        public void TrimLineBreaksTest1()
        {
            string text = "line\r\n\r\nGO\r\n";
            string result = TrimLineBreaks(text);
            result = result + "\r\nGO\r\n";
            Assert.AreEqual("line\r\nGO\r\n", result);
        }

        [Test]
        public void TrimLineBreaksTest2()
        {
            string text = "line1\r\nline2\r\n\r\nline3\r\n\r\n\r\n\r\nGO\r\n";
            string result = TrimLineBreaks(text);
            result = result + "\r\nGO\r\n";
            Assert.AreEqual("line1\r\nline2\r\n\r\nline3\r\nGO\r\n", result);
        }

        [Test]
        public void TrimLineBreaksTest3()
        {
            string text = "\r\n\r\n";
            string result = TrimLineBreaks(text);
            Assert.AreEqual(string.Empty, result);
        }

        [Test]
        public void TrimLineBreaksTest4()
        {
            string text = "";
            string result = TrimLineBreaks(text);
            Assert.AreEqual(string.Empty, result);
        }

        private static string TrimLineBreaks(string text)
        {
            return GenerateSqlScripts.TrimLineBreaks(text);
        }

    }
}
