using System;

using NUnit.Framework;

using IniParser;

namespace IniFileParserTests
{
    [TestFixture, Category("String parsing/writing tests")]
    public class IniStringParserTest
    {

        private readonly string _iniFileStr =
            ";comment for section1" + Environment.NewLine +
            "[section1]" + Environment.NewLine +
            ";comment for key1" + Environment.NewLine +
            "key1 = value1" + Environment.NewLine +
            "key2 = value5" + Environment.NewLine +
            "[section2] " + Environment.NewLine +
            ";comment for myKey1 " + Environment.NewLine +
            "mykey1 = value1 " + Environment.NewLine;

        private StringIniParser _sip;

        [SetUp]
        public void SetUp()
        {
            _sip = new StringIniParser();
        }

        [Test]
        public void ReadingFromStringTest()
        {
            IniData data = _sip.ParseString(_iniFileStr);

            Assert.That(data, Is.Not.Null);
            Assert.That(data.Sections.Count, Is.EqualTo(2));
        }

        [Test]
        public void WritingTotringTest()
        {
            IniData data = new IniData();

            data.Sections.AddSection("newSection1");
            data.Sections["newSection1"].AddKey("newKey1", "newValue1");
            data.Sections["newSection1"].AddKey("newKey2", "newValue5");

            string result = _sip.WriteString(data);

            Assert.That(result, Is.Not.Empty);
            Assert.That(result.Length, Is.Not.EqualTo(0));

        }

    }
}