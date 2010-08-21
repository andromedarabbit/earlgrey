using System;

using NUnit.Framework;

using NUnit.Framework.SyntaxHelpers;

using IniParser;

namespace IniFileParserTests
{
    [TestFixture, Category("String parsing/writing tests")]
    public class IniStringParser_Test
    {

        private string iniFileStr =
            ";comment for section1" + Environment.NewLine +
            "[section1]" + Environment.NewLine +
            ";comment for key1" + Environment.NewLine +
            "key1 = value1" + Environment.NewLine +
            "key2 = value5" + Environment.NewLine +
            "[section2] " + Environment.NewLine +
            ";comment for myKey1 " + Environment.NewLine +
            "mykey1 = value1 " + Environment.NewLine;

        private StringIniParser sip;

        [SetUp]
        public void SetUp()
        {
            sip = new StringIniParser();
        }

        [Test]
        public void ReadingFromString_Test()
        {
            IniData data = sip.ParseString(iniFileStr);

            Assert.That(data, Is.Not.Null);
            Assert.That(data.Sections.Count, Is.EqualTo(2));
        }

        [Test]
        public void WritingTotring_Test()
        {
            IniData data = new IniData();

            data.Sections.AddSection("newSection1");
            data.Sections["newSection1"].AddKey("newKey1", "newValue1");
            data.Sections["newSection1"].AddKey("newKey2", "newValue5");

            string result = sip.WriteString(data);

            Assert.That(result, Is.Not.Empty);
            Assert.That(result.Length, Is.Not.EqualTo(0));

        }

    }
}