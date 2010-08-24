using System.IO;
using NUnit.Framework;

using IniParser;

namespace IniFileParserTests
{
    [TestFixture, Category("File loading/saving tests")]
    public class IniParserFileTest
    {

        private readonly string StrGoodIniFilePath = @"../../INIFileGOOD.ini";
        private readonly string StrBadIniFilePath = @"../../INIFileBAD.ini";
        private readonly string StrEmptyIniFilePath = @"../../INIFileEmpty.ini";
        private readonly string StrBadSectionIniFilePath = @"../../INIFileBADSection.ini";
        private readonly string StrBadKeysIniFilePath = @"../../INIFileBADKeys.ini";

        private readonly FileIniDataParser _iniParser = new FileIniDataParser();

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