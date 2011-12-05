using System;
using System.IO;
using IniParser;

using NUnit.Framework;

namespace IniFileParserTests
{
    [TestFixture]
    public class IssuesTest
    {
        private readonly string StrBadSectionIniFilePath;
        private readonly string StrBadKeysIniFilePath;

        public IssuesTest()
        {
            StrBadSectionIniFilePath = Path.Combine(TestSupport.TestScriptsFolder, "INIFileBADSection.ini");
            StrBadKeysIniFilePath = Path.Combine(TestSupport.TestScriptsFolder, "INIFileBADKeys.ini");
        }


        #region Test Members

        //TODO: Add fields used in the tests

        #endregion

        #region TestFixture SetUp/TearDown

        [TestFixtureSetUp]
        public void TestFixtureSetUp()
        {
            //TODO: Add test fixture set up code
        }

        [TestFixtureTearDown]
        public void TestFixtureTearDown()
        {
            //TODO: Add test fixture tear down code
        }

        #endregion

        #region  Test SetUp/TearDown

        [SetUp]
        public void SetUp()
        {
            //TODO: Add test set up code
        }

        [TearDown]
        public void TearDown()
        {
            //TODO: Add test tear down code
        }

        #endregion

        #region Tests

        [Test, Description("Test for Issue 2: http://code.google.com/p/ini-parser/issues/detail?id=2")]
        public void Issue2Tests()
        {
            const string test = "[ExampleSection]\nkey = value;value\n";

            StringIniParser strParser = new StringIniParser();

            IniData data = strParser.ParseString(test);

            Assert.That(data.Sections.Count, Is.EqualTo(1));
            Assert.That(data.Sections["ExampleSection"], Is.Not.Null);
            Assert.That(data.Sections["ExampleSection"].Count, Is.EqualTo(1));
            Assert.That(data.Sections["ExampleSection"]["key"], Is.EqualTo("value"));

        }

        [Test, Description("Test for Issue 3: http://code.google.com/p/ini-parser/issues/detail?id=3")]
        public void Issue3Tests()
        {
            const string strTest = "[section_issue.3]\nkey.with_dots = value\n";

            IniData data = new StringIniParser().ParseString(strTest);

            Assert.That(data.Sections.Count, Is.EqualTo(1));
            Assert.That(data.Sections["section_issue.3"]["key.with_dots"], Is.Not.Null);

        }

        [Test, Description("Test for Issue 4: http://code.google.com/p/ini-parser/issues/detail?id=4")]
        public void Issue4Tests()
        {
            FileIniDataParser fileParser = new FileIniDataParser();

            // TODO: 예외를 발생시키거나 지금처럼 동일한 이름을 가진 섹션에 속한 키/값 을 모두 합치는 게 맞다고 보는데.
            IniData data = fileParser.LoadFile(StrBadSectionIniFilePath, true);

            Assert.That(data, Is.Not.Null);
            Assert.That(data.Sections.Count, Is.EqualTo(1));
                        
            Assert.That(data.Sections.GetSectionData("seccion1").Keys.Count, Is.EqualTo(2));

            data = fileParser.LoadFile(StrBadKeysIniFilePath, true);

            Assert.That(data, Is.Not.Null);
            Assert.That(data.Sections.GetSectionData("seccion1").Keys.Count, Is.EqualTo(1));
        }


        /// <summary>
        ///     Thanks to h.eriksson@artamir.org for the issue.
        /// </summary>
        [Test, Description("Test for Issue 5: http://code.google.com/p/ini-parser/issues/detail?id=5")]
        public void Issue5Tests()
        {
            IniData inidata = new IniData();
            inidata.Sections.AddSection("TestSection");

            KeyData key = new KeyData("TestKey");
            key.Value = "TestValue";
            key.Comments.Add("This is a comment");
            inidata["TestSection"].SetKeyData(key);

            Assert.That(inidata["TestSection"].GetKeyData("TestKey").Comments[0], Is.EqualTo("This is a comment"));
        }


        /// <summary>
        ///     Thanks to h.eriksson@artamir.org for the issue.
        /// </summary>
        [Test, Description("Test for Issue 6: http://code.google.com/p/ini-parser/issues/detail?id=6")]
        public void Issue6Tests()
        {
            string data = "[data]" + Environment.NewLine +"key = value;";

            IniData inidata = new StringIniParser().ParseString(data);

            Assert.That(inidata["data"]["key"],Is.EqualTo("value") );
        }

        /// <summary>
        ///     Thanks to  for the issue.
        /// </summary>
        [Test, Description("Test for Issue 7: http://code.google.com/p/ini-parser/issues/detail?id=7")]
        public void Issue7Tests1()
        {
            IniData newData = new IniData();

            newData.Sections.AddSection("newSection");
            newData["newSection"].AddKey("newKey1", "value1");

            Assert.That(newData["newSection"]["newKey1"], Is.EqualTo("value1"));
        }

        /// <summary>
        ///     Thanks to  for the issue.
        /// </summary>
        [Test, Description("Test for Issue 7: http://code.google.com/p/ini-parser/issues/detail?id=7")]
        public void Issue7Tests()
        {
            IniData newData = new IniData();

            newData.Sections.AddSection("newSection");
            newData["newSection"].AddKey("newKey1");
            newData["newSection"]["newKey1"] = "value1";

            Assert.That(newData["newSection"]["newKey1"], Is.EqualTo("value1"));
        }
        #endregion
    }
}
