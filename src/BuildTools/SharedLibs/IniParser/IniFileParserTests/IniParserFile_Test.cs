using System.IO;
using NUnit.Framework;

using IniParser;

namespace IniFileParserTests
{
    [TestFixture, Category("File loading/saving tests")]
    public class IniParserFileTest
    {

        private readonly string StrGoodIniFilePath;
        private readonly string StrBadIniFilePath;
        private readonly string StrEmptyIniFilePath;
        private readonly string StrBadSectionIniFilePath;
        private readonly string StrBadKeysIniFilePath;

        private readonly FileIniDataParser _iniParser = new FileIniDataParser();

        public IniParserFileTest()
        {
            StrGoodIniFilePath = Path.Combine(TestSupport.TestScriptsFolder, "INIFileGOOD.ini");
            StrBadIniFilePath = Path.Combine(TestSupport.TestScriptsFolder, "INIFileBAD.ini");
            StrEmptyIniFilePath = Path.Combine(TestSupport.TestScriptsFolder, "INIFileEmpty.ini");
            StrBadSectionIniFilePath = Path.Combine(TestSupport.TestScriptsFolder, "INIFileBADSection.ini");
            StrBadKeysIniFilePath = Path.Combine(TestSupport.TestScriptsFolder, "INIFileBADKeys.ini");
        }
        
        [SetUp]
        public void SetUp()
        {
        }

        [TearDown]
        public void TearDown()
        {
        }

        [Test, Description("Checks existence of test INI files")]
        public void CheckTestFilesExists()
        {
            Assert.That(File.Exists(StrGoodIniFilePath), "Correct INI file {0} does not exists", StrGoodIniFilePath);
            Assert.That(File.Exists(StrBadIniFilePath), "Invalid INI file {0} does not exists", StrBadIniFilePath);
            Assert.That(File.Exists(StrEmptyIniFilePath), "Empty INI file {0} does not exists", StrEmptyIniFilePath);
        }

        [Test, Description("Checks correct parsing of an empty INI file")]
        public void CheckParseEmptyFileSuccess()
        {
            IniData parsedData = _iniParser.LoadFile(StrEmptyIniFilePath);
            
            Assert.That(parsedData, Is.Not.Null);
        }

        [Test, Description("Checks correct parsing of a well formed INI file")]
        public void CheckParseGoodFileSuccess()
        {
            IniData parsedData = _iniParser.LoadFile(StrGoodIniFilePath);

            Assert.That(parsedData, Is.Not.Null);
        }

        [Test, Description("Checks error when parsing a bad formed INI file")]
        [ExpectedException(typeof(ParsingException))]
        public void CheckParsingFailure()
        {
            _iniParser.LoadFile(StrBadIniFilePath);
        }

        [Test, Description("Checks correct saving of a file")]
        public void CheckCorrectSave()
        {
            string fileString = StrGoodIniFilePath + "_test.ini";

            IniData parsedData = _iniParser.LoadFile(StrGoodIniFilePath);
            _iniParser.SaveFile(fileString, parsedData);

            Assert.That(File.Exists(fileString));
        }

        [Test, Description("Checks bad formed INI file: Two sections with same name")]
        [ExpectedException(typeof(ParsingException))]
        public void CheckCollideSectionNames()
        {
            _iniParser.LoadFile(StrBadSectionIniFilePath);
        }

        [Test, Description("Checks bad formed INI file: Two keys in the same section with same name")]
        [ExpectedException(typeof(ParsingException))]
        public void CheckCollideKeysNames()
        {
            _iniParser.LoadFile(StrBadKeysIniFilePath);
        }

    }
}