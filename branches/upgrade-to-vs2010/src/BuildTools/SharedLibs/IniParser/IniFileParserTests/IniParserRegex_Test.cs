using System;
using NUnit.Framework;

using IniParser;

namespace IniFileParserTests
{
    [TestFixture, Category("Regular expression tests")]
    public class IniParserRegexTest
    {
        private readonly StreamIniDataParser _iniParser = new StreamIniDataParser();

        [TestFixtureSetUp]
        public void Init()
        {
            Console.WriteLine(_iniParser.SectionRegexString);
        }

        [Test, Description("Test a regular expression for matching a comment in an INI file")]
        public void TestCommentRegex()
        {
            Console.WriteLine("Regular expression for comments: {0}", _iniParser.CommentRegexString);

            const string strGoodTest1 = ";comment Test";
            const string strGoodTest2 = "   ; comment Test";
            const string strGoodTest3 = "   ; comment Test           ";
            const string strGoodTest4 = " dfasdfasf ; comment Test ";

            Assert.That(strGoodTest1, Is.StringMatching(_iniParser.CommentRegexString));
            Assert.That(strGoodTest2, Is.StringMatching(_iniParser.CommentRegexString));
            Assert.That(strGoodTest3, Is.StringMatching(_iniParser.CommentRegexString));
            Assert.That(strGoodTest4, Is.StringMatching(_iniParser.CommentRegexString));
        }

        [Test, Description("Test a regular expression for matching a section in an INI file")]
        public void TestSectionRegex()
        {
            Console.WriteLine("Regular expresion for sections: {0}", _iniParser.SectionRegexString);


            const string strGoodTest1 = "[section]";
            const string strGoodTest2 = "   [sec-tion]   ";
            const string strGoodTest3 = "[ .section ]";
            const string strGoodTest4 = "[ s_ection ]";

            const string strBadTest1 = "  bad [section]";
            const string strBadTest2 = "[section] bad";

            Assert.That(strGoodTest1, Is.StringMatching(_iniParser.SectionRegexString));
            Assert.That(strGoodTest2, Is.StringMatching(_iniParser.SectionRegexString));
            Assert.That(strGoodTest3, Is.StringMatching(_iniParser.SectionRegexString));
            Assert.That(strGoodTest4, Is.StringMatching(_iniParser.SectionRegexString));

            Assert.That(strBadTest1, !Is.StringMatching(_iniParser.SectionRegexString));
            Assert.That(strBadTest2, !Is.StringMatching(_iniParser.SectionRegexString));
        }

        [Test, Description("Test a regular expression for matching a section in an INI file given an specific delimiter")]
        public void TestNewSectionDelimiter()
        {
            _iniParser.SectionDelimiters = new char[] { '<', '>' };
            Console.WriteLine("Regular expresion for sections: {0}", _iniParser.SectionRegexString);


            const string strGoodTest1 = "<section>";
            const string strGoodTest2 = "   <section>   ";
            const string strGoodTest3 = "< section >";

            const string strBadTest1 = "  bad <section>";
            const string strBadTest2 = "<section> bad";


            Assert.That(strGoodTest1, Is.StringMatching(_iniParser.SectionRegexString));
            Assert.That(strGoodTest2, Is.StringMatching(_iniParser.SectionRegexString));
            Assert.That(strGoodTest3, Is.StringMatching(_iniParser.SectionRegexString));

            Assert.That(strBadTest1, !Is.StringMatching(_iniParser.SectionRegexString));
            Assert.That(strBadTest2, !Is.StringMatching(_iniParser.SectionRegexString));

            //Restore default delimiters
            _iniParser.SectionDelimiters = new char[] { '[', ']' };
        }

        [Test, Description("Test a regular expression for matching a key=value pair in an ini file")]
        public void KeyValuePairRegex()
        {

            Console.WriteLine("Regular expresion for key values: {0}", _iniParser.KeyValuePairRegexString);

            const string strGoodTest1 = "    value   =   hello   ";
            const string strGoodTest2 = "   value=hello world!  ";
            const string strGoodTest3 = "     value=hello world!   = hello world!  ";
            const string strGoodTest4 = "value=hello world!  \" ";
            const string strGoodTest5 = " value value = hello world ";

            const string strBadTest1 = " value value";

            Assert.That(strGoodTest1, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest2, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest3, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest4, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest5, Is.StringMatching(_iniParser.KeyValuePairRegexString));

            Assert.That(strBadTest1, !Is.StringMatching(_iniParser.KeyValuePairRegexString));
        }

        [Test, Description("Test a regular expression for matching a key in a key=value pair")]
        public void KeyRegex()
        {
            const string strGoodTest1 = "    value   =   hello   ";
            const string strGoodTest2 = "   value=hello world!  ";
            const string strGoodTest3 = "     value=hello world!   = hello world!  ";
            const string strGoodTest4 = "value=hello world!  \" ";
            const string strGoodTest5 = " value value = hello world ";

            const string strBadTest1 = " value value";

            Assert.That(strGoodTest1, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest2, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest3, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest4, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest5, Is.StringMatching(_iniParser.KeyValuePairRegexString));

            Assert.That(strBadTest1, !Is.StringMatching(_iniParser.KeyValuePairRegexString));
        }

        [Test, Description("Test a regular expression for matching a value in a key=value pair")]
        public void ValueRegex()
        {
            const string strGoodTest1 = "    value   =   hello   ";
            const string strGoodTest2 = "   value=hello world!  ";
            const string strGoodTest3 = "     value=hello world!   = hello world!  ";
            const string strGoodTest4 = "value=hello world!  \" ";
            const string strGoodTest5 = " value value = hello world ";

            const string strBadTest1 = " value value";

            Assert.That(strGoodTest1, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest2, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest3, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest4, Is.StringMatching(_iniParser.KeyValuePairRegexString));
            Assert.That(strGoodTest5, Is.StringMatching(_iniParser.KeyValuePairRegexString));

            Assert.That(strBadTest1, !Is.StringMatching(_iniParser.KeyValuePairRegexString));
        }
    }
}